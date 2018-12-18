#!/bin/sh

# Repeatedly play multiple PCM wave files to verify whether the sample rate
# configuration of audio codec is correct or not.
#
# Each PCM wave file has specific duration. Repeatedly playing it in a specific
# numbers of times will take a specific amount of time. By judging whether the
# time spent in playing, we can know whether the sample rate configuration is
# correct or not.
#
# The total time spent in playing is (in second):
#       time = playback_duration_sec * playback_repeat_times
# If the actual time spent is in the interval:
#       [time - tolerance_sec, time + tolerance_sec]
# the sample rate configuration is correct, otherwise not.

keypath="/base/audio/codec/playback_rate_verify"
ucm_params="-c audiocodec -v Play -p"

playback_volume=$(mjson_fetch "${keypath}/playback_volume")
playback_duration_sec=$(mjson_fetch "${keypath}/playback_duration_sec")
playback_repeat_times=$(mjson_fetch "${keypath}/playback_repeat_times")
tolerance_sec=$(mjson_fetch "${keypath}/tolerance_sec")
wav_dir=$(mjson_fetch "${keypath}/wav_dir")

alsa-ucm-aw $ucm_params enable
if [ $? -ne "0" ]; then
    ttips "Failed to enable the UCM device for playback"
    exit 1
fi

pcm_device=$(alsa-ucm-aw $ucm_params get PlaybackPCM)
if [ $? -ne "0" ]; then
    ttips "Failed to get the \"PlaybackPCM\""
    exit 1
fi

alsa-ucm-aw $ucm_params setvol $playback_volume

correct_time=$(expr $playback_duration_sec \* $playback_repeat_times)
correct_left=$(expr $correct_time - $tolerance_sec)
correct_right=$(expr $correct_time + $tolerance_sec)
if [ $correct_left -lt "0" ]; then
    correct_left=0
fi

rates="8000 11025 16000 22050 24000 32000 44100 48000 88200 96000 176400 192000"
result="Result:"
exit_code=0
for i in $rates; do
    is_selected=$(mjson_fetch "${keypath}/rate_${i}")
    if [ "x$is_selected" == "xtrue" ]; then
        wav_file=$(mjson_fetch "${keypath}/rate_${i}_wav_file")
        wav_path="${wav_dir}/${wav_file}"
        if [ ! -f "$wav_path" ]; then
            ttips "No such file: \"$wav_path\""
            alsa-ucm-aw $ucm_params disable
            exit 1
        fi
        ttips "Verifying rate $i..."
        time_usec=$(audiotest_wav_play_time -D "$pcm_device" \
            -t $playback_repeat_times -U "$wav_path")
        case "$?" in
            0)
                time_sec=$(expr $time_usec / 1000000)
                if [ $time_sec -ge $correct_left -a \
                        $time_sec -le $correct_right ]; then
                    result=${result}" -n ${i}: correct (spend: ${time_sec}s)"
                else
                    result=${result}" -n ${i}: incorrect (spend: ${time_sec}s)"
                    exit_code=1
                fi
                ;;
            *)
                result=${result}" -n ${i}: error when playing \"$wav_path\""
                ;;
        esac
    fi
done

alsa-ucm-aw $ucm_params disable

ttips $result
exit $exit_code
