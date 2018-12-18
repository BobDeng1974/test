#!/bin/sh

check_connect(){
	bt_test  > /tmp/bluetooth.info &
	ttips "Please connect bluetooth name:aw bt test 001"
	while true
	do
		sleep 2
		let count++
		grep "app_avk_callback: avk connected!" /tmp/bluetooth.info && {
			ttips "Connect Success"
			return 0
		}

		if [ $count -eq 90 ]; then
			echo "======== Connection timed out ================="
			echo "======== For details, see /tmp/bluetooth.info ========"
			exit 1
		fi
	done
}

check_connect

ttrue "Please use headset to connect device." \
	-n "then ,Playing music!" \
	-n "Check if you can hear the music from device"
[ $? = 0 ] || exit 1

ttrue "User APP volume key to adjust volme" \
	-n "check device volume up/down key to adjust volume."
[ $? = 0 ] || exit 1

