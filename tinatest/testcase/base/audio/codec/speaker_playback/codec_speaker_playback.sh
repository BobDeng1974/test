#!/bin/sh

keypath="/base/audio/codec/speaker_playback"
ucm_params="-c audiocodec -v Play -d Speaker"

speaker_volume=$(mjson_fetch "${keypath}/speaker_volume")
playback_repeat_times=$(mjson_fetch "${keypath}/playback_repeat_times")
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

alsa-ucm-aw $ucm_params -p setvol $speaker_volume

ttips "Starting playing with speaker (repeat $playback_repeat_times times)"
for i in $(seq 1 $playback_repeat_times); do
    aplay -D "$pcm_device" "$wav_path" -q
done
alsa-ucm-aw $ucm_params disable
ttrue "Finish playing. Can you hear the sound from speaker?"
if [ $? -eq "0" ]; then
    exit 0
else
    exit 1
fi
