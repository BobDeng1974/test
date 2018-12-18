#!/bin/sh

keypath="/base/audio/codec/phonein_capture"
ucm_params="-c audiocodec -v Record -d PhoneIn"

capture_channels=$(mjson_fetch "${keypath}/capture_channels")
capture_format=$(mjson_fetch "${keypath}/capture_format")
capture_rate=$(mjson_fetch "${keypath}/capture_rate")
capture_duration_sec=$(mjson_fetch "${keypath}/capture_duration_sec")
records_dir=$(mjson_fetch "${keypath}/records_dir")
remove_records=$(mjson_fetch "${keypath}/remove_records")
playback_volume=$(mjson_fetch "${keypath}/playback_volume")

ttrue "Is the phone in ready?"
if [ $? -ne "0" ]; then
    ttips "ERROR: phone in is not ready"
    exit 1
fi

ttrue "Is the playback device (e.g. headphone, speaker, etc.) ready?"
if [ $? -ne "0" ]; then
    ttips "ERROR: playback device is not ready"
    exit 1
fi
alsa-ucm-aw -p enable
alsa-ucm-aw -p setvol $playback_volume
playback_pcm_device=$(alsa-ucm-aw -p get Card)

if [ ! -d "$records_dir" ]; then
    mkdir -p "$records_dir"
    created_dir="$records_dir"
fi

alsa-ucm-aw $ucm_params enable
if [ $? -ne "0" ]; then
    ttips "Failed to enable the UCM device \"PhoneIn\""
    continue
fi
pcm_device=$(alsa-ucm-aw $ucm_params get CapturePCM)
if [ $? -ne "0" ]; then
    ttips "Failed to get the \"CapturePCM\" of \"PhoneIn\""
    continue
fi
record="${records_dir}/record_${i}.wav"
ttips "Start recording with phone in"
arecord -D "$pcm_device" -c "$capture_channels" -f "$capture_format" \
    -r "$capture_rate" -d "$capture_duration_sec" "$record" -q
alsa-ucm-aw $ucm_params disable
ttips "Finish recording, and start playing the record"
aplay -D "$playback_pcm_device" "$record" -q
if [ "x$remove_records" == "xtrue" ]; then
    rm -f "$record"
fi
alsa-ucm-aw -p disable

if [ "x$remove_records" == "xtrue" -a "x$created_dir" != "x" ]; then
    if [ "x$(ls -A $created_dir)" == "x" ]; then
        rm -r "$created_dir"
    fi
fi

ttrue "Can you hear the record of phone in?"
if [ $? -eq "0" ]; then
    exit 0
else
    exit 1
fi
