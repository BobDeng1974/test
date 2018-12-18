#!/bin/sh
##############################################################################
start_brcm() {
    echo "====== start brcm ======"
    echo 0 > /sys/class/rfkill/rfkill0/state
    sleep 2
    echo 1 > /sys/class/rfkill/rfkill0/state
    sleep 2

	brcm_patchram_plus  --tosleep=200000 --no2bytes --enable_hci --scopcm=0,2,0,0,0,0,0,0,0,0 \
	                    --baudrate ${baud_rate} --patchram ${module_path}  ${device_node} &
	sleep 10
    echo "====== start end ======"
}

check_rfkill() {
    echo "====== CHECKING rfkill ======"
    for rfkill_path in $(ls /sys/class/rfkill)
	do
        [ "rfkill0" = "${rfkill_path}" ] && continue
		rfkill_path="/sys/class/rfkill/${rfkill_path}"
		if [ -d "${rfkill_path}" ]; then
			if cat "${rfkill_path}/type" | grep -q bluetooth ; then
                echo "found bt in ${rfkill_path}"
				state_path="${rfkill_path}/state"
				echo 1 > $state_path
				sleep 1
                echo "====== rfkill PASS ======"
				return 0
			fi
		fi
	done
    echo "====== rfkill FAILED ======"
    return 1
}

kill_brcm() {
	killall brcm_patchram_plus > /dev/null
	sleep 3
}

up_and_check_hci0() {
    echo "====== CHECKING up hci0 ======"
	hciconfig hci0 up \
        && echo "======= up hci0 PASS ======" \
        && return 0

    echo "======= up hci0 FAILED ======"
	return 1
}

down_hci0() {
	hciconfig hci0 down > /dev/null
	sleep 1
}

check_devdevice() {
    echo "====== CHECKING dev ======"
	dev_device=`hcitool dev`
	[ "Devices:" = "${dev_device:0:8}" ] \
        && echo ${dev_device} \
        && return 0

    echo "====== get device FAILED ======"
	return 1
}

check_scanresult() {
    echo "====== CHECKING scan ======"
    scan_result="$(hcitool scan)"
	[ ! "Scan" = "${scan_result:0:4}" ] \
        && echo "====== scan FAILED ======" \
        && return 1

    scan_result="$(echo "${scan_result}" | sed '/Scanning/d')"
    [ -z "${scan_result}" ] \
        && echo "====== do scan, but nothing scanned ======" \
        && return 1

    echo "${scan_result}"
    echo "====== scan PASS ======"
	return 0
}

check_chipId() {
    echo "====== CHECKING chip ID ======"
    id="$(find /sys/devices/ -name hci0 -type d | head -n 1)"
    [ -z "${id}" ] \
        && echo "====== get chip ID FAILED ======" \
        && return 1

    cat ${id}/name
    echo "====== CHECKING chip ID PASS ======"
	return 0
}

check_hci0_down() {
    echo "====== CHECKING down hci0 ======"
	hciconfig hci0 down \
        && echo "====== down hci0 PASS ======" \
        && return 0

    echo "====== CHECKING down hci0 FAILED ======"
    return 1
}

check_bluetooth() {

    for cnt in $(seq 1 ${loop_times})
    do
        echo
        echo "-------------------------- start ${cnt} times  -----------------------------------"
        start_brcm

        ! check_rfkill && kill_brcm && continue
        ! up_and_check_hci0 && down_hci0 && kill_brcm && continue
        ! check_devdevice && down_hci0 && kill_brcm && continue
        ! check_scanresult && down_hci0 && kill_brcm && continue
        ! check_chipId && down_hci0 && kill_brcm && continue
        ! check_hci0_down && down_hci0 && kill_brcm && continue

        kill_brcm
        return 0
    done
    return 1
}

#start test
module_path="$(mjson_fetch /base/production/bttester/module_path)"
device_node="$(mjson_fetch /base/production/bttester/device_node)"
baud_rate="$(mjson_fetch /base/production/bttester/baud_rate)"
loop_times="$(mjson_fetch /base/production/bttester/max_test_times)"

echo target : $(get_target)
echo module_path : ${module_path}
echo device_node : ${device_node}
echo baud_rate : ${baud_rate}
echo max_test_times : ${loop_times}

[ -z "${module_path}" -o \
    -z "${device_node}" -o \
    -z "${baud_rate}" ] \
    && echo "Miss Setting" && exit 1

check_bluetooth && echo "bt test pass"  && exit 0

echo "bt test failed"
exit 1
