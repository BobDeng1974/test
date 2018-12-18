#!/bin/sh

keypath="/base/audio/codec/speaker_volume_adjust"
ucm_params="-c audiocodec -v Play -d Speaker"

playback_repeat_times=$(mjson_fetch "${keypath}/playback_repeat_times")
volume_adjust_step=$(mjson_fetch "${keypath}/volume_adjust_step")
volume_adjust_interval_sec=$(mjson_fetch "${keypath}/volume_adjust_interval_sec")
wav_dir=$(mjson_fetch "${keypath}/wav_dir")
wav_file=$(mjson_fetch "${keypath}/wav_file")

wav_path="${wav_dir}/${wav_file}"

ttrue "Is the speaker ready?"
if [ $? -ne "0" ]; then
    ttips "ERROR: speaker is not ready"
    exit 1
fi

ttrue "Is the WAV file \"$wav_path\" ready?"
if [ $? -eq "0" ]; then
    if [ ! -f "$wav_path" ]; then
        ttips "No such file: \"$wav_path\""
        exit 1
    fi
else
    ttips "The WAV file is not ready"
    exit 1
fi

alsa-ucm-aw $ucm_params enable
if [ $? -ne "0" ]; then
    ttips "Failed to enable the UCM device \"Speaker\""
    exit 1
fi

pcm_device=$(alsa-ucm-aw $ucm_params get PlaybackPCM)
if [ $? -ne "0" ]; then
    ttips "Failed to get the \"PlaybackPCM\" of \"Speaker\""
    exit 1
fi

is_increase=1
volume=$(alsa-ucm-aw $ucm_params -p getvol)
volume_min=$(alsa-ucm-aw $ucm_params get PlaybackVolumeMin)
volume_max=$(alsa-ucm-aw $ucm_params get PlaybackVolumeMax)

ttips "Start playing with speaker (repeat $playback_repeat_times times)"
for i in $(seq 1 $playback_repeat_times); do
    aplay -D "$pcm_device" "$wav_path" -q
done &

while true; do
    if [ $is_increase -eq "1" ]; then
        volume=$(expr $volume + $volume_adjust_step)
        if [ $volume -ge $volume_max ]; then
            volume=$volume_max
            is_increase=0
        fi
    else
        volume=$(expr $volume - $volume_adjust_step)
        if [ $volume -le $volume_min ]; then
            volume=$volume_min
            is_increase=1
        fi
    fi
    alsa-ucm-aw $ucm_params -p setvol $volume
    ttips "Current volume: $volume"
    background=$(jobs)
    background=$(echo "$background" | grep "Running")
    if [ "x$background" == "x" ]; then
        break
    fi
    sleep $volume_adjust_interval_sec
done

alsa-ucm-aw $ucm_params disable

ttrue "Does speaker volume adjustment work well?"
if [ $? -eq "0" ]; then
    exit 0
else
    exit 1
fi
