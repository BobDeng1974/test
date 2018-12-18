#!/bin/sh

keypath="/base/audio/codec/playback_rate_supported"
ucm_params="-c audiocodec -v Play -p"

playback_volume=$(mjson_fetch "${keypath}/playback_volume")
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

rates="8000 11025 16000 22050 24000 32000 44100 48000 88200 96000 176400 192000"
result="Result:"
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
        ttips "Testing rate $i..."
        aplay -D "$pcm_device" "$wav_path" -q
        case "$?" in
            0)
                result=${result}" -n ${i}: support"
                ;;
            *)
                result=${result}" -n ${i}: not support"
                ;;
        esac
    fi
done

alsa-ucm-aw $ucm_params disable

result=${result}" -n Is the result consistent with specifications?"
ttrue $result
if [ $? -eq "0" ]; then
    exit 0
else
    exit 1
fi
