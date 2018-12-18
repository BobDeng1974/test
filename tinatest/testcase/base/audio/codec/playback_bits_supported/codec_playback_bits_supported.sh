#!/bin/sh

keypath="/base/audio/codec/playback_bits_supported"
ucm_params="-c audiocodec -v Play -p"

playback_volume=$(mjson_fetch "${keypath}/playback_volume")

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

formats="s16_le s24_3le"
result="Result:"
for i in $formats; do
    is_selected=$(mjson_fetch "${keypath}/${i}")
    if [ "x$is_selected" == "xtrue" ]; then
        wav_path=$(mjson_fetch "${keypath}/${i}_wav_path")
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
