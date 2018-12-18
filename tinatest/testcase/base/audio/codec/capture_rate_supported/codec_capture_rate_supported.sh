#!/bin/sh

keypath="/base/audio/codec/capture_rate_supported"
ucm_params="-c audiocodec -v Record -r"

capture_volume=$(mjson_fetch "${keypath}/capture_volume")
capture_channels=$(mjson_fetch "${keypath}/capture_channels")
capture_format=$(mjson_fetch "${keypath}/capture_format")
capture_duration_sec=$(mjson_fetch "${keypath}/capture_duration_sec")
records_dir=$(mjson_fetch "${keypath}/records_dir")
remove_records=$(mjson_fetch "${keypath}/remove_records")

alsa-ucm-aw $ucm_params enable
if [ $? -ne "0" ]; then
    ttips "Failed to enable the UCM device for capture"
    exit 1
fi

pcm_device=$(alsa-ucm-aw $ucm_params get CapturePCM)
if [ $? -ne "0" ]; then
    ttips "Failed to get the \"CapturePCM\""
    exit 1
fi

alsa-ucm-aw $ucm_params setvol $capture_volume

if [ ! -d "$records_dir" ]; then
    mkdir -p "$records_dir"
    created_dir="$records_dir"
fi

rates="8000 11025 16000 22050 24000 32000 44100 48000 88200 96000 176400 192000"
result="Result:"
for i in $rates; do
    is_selected=$(mjson_fetch "${keypath}/rate_${i}")
    if [ "x$is_selected" == "xtrue" ]; then
        record="${records_dir}/record_${i}.wav"
        ttips "Testing rate $i..."
        arecord -D "$pcm_device" -r "$i" -c "$capture_channels" \
            -f "$capture_format" -d "$capture_duration_sec" "$record" -q
        case "$?" in
            0)
                result=${result}" -n ${i}: support"
                ;;
            *)
                result=${result}" -n ${i}: not support"
                ;;
        esac
        if [ "x$remove_records" == "xtrue" ]; then
            rm -f "$record"
        fi
    fi
done

if [ "x$remove_records" == "xtrue" -a "x$created_dir" != "x" ]; then
    if [ "x$(ls -A $created_dir)" == "x" ]; then
        rm -r "$created_dir"
    fi
fi

alsa-ucm-aw $ucm_params disable

result=${result}" -n Is the result consistent with specifications?"
ttrue $result
if [ $? -eq "0" ]; then
    exit 0
else
    exit 1
fi
