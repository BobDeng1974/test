#!/bin/sh

write_wpa_conf(){
if [ "x$2" = "xNONE" ];then
	key="key_mgmt=NONE"
else
	key="psk=\"$2\""
fi

(cat <<EOF
ctrl_interface=/etc/wifi/sockets
disable_scan_offload=1
update_config=1

network={
   	ssid="$1"
   	scan_ssid=1
   	$key
   	priority=1
}
EOF
) >/etc/wifi/wpa_supplicant.conf
}

check_wlan(){
	ifconfig -a | grep wlan0 || {
		echo "===========Not Found wlan0========="
		exit 1
	}
}

check_ping(){
	result=`ping $1 -c 2 | grep "2 packets transmitted" \
		| awk '{printf ("%s %s %s", $7, $8, $9) }'`
	if [ -z "${result}" ];then
		echo "======== CHECKED PING FAILED ======="
		exit 1
	else
		echo "========================================="
		echo "PING result:"
		echo "2 packets transmitted,$result"
		echo "========================================="
		return 0
	fi
}

connect(){
    ps | grep wpa_supplicant | awk '{print $5}' | grep wpa_supplicant && {
		killall wpa_supplicant
		sleep 1
	}

	ps | grep udhcpc | awk '{print $5}' | grep udhcpc && {
		killall udhcpc
		sleep 1
	}

	wpa_supplicant -iwlan0 -Dnl80211 -c/etc/wifi/wpa_supplicant.conf > /tmp/wifi.log &

	sleep 1

	while true
	do
		usleep 500
		let count++
		grep "CTRL-EVENT-CONNECTED" /tmp/wifi.log && {
			ip=`/sbin/udhcpc -i wlan0 | awk '($2~/^setting/&&$3~/^default/) {print $5}'`
			check_ping $ip
			exit 0;
		}
		grep "pre-shared key may be incorrect" /tmp/wifi.log && {
			echo "================= PASSWARD INCORRECT ============="
			exit 1
		}
		if [ $count -eq 1000 ]; then
			echo "======== Connection timed out ================="
			echo "======== For details, see /tmp/wifi.log========"
			exit 1
		fi
	done
}

enter_info(){
	local number=0
	until [ $number -ge 8 ];do
		ssid=$(task "Please enter the WIFI SSID")
		password=$(task "Please enter WIFI password")
		number=$(echo $password | wc -c)

		[ "$password" = "NONE" ] && break
		[ $number -lt 8 ] && {
			ttips "The password must be greater than 8 digits,Please enter again."
		}
	done
	write_wpa_conf $ssid $password
}

check_wificonf(){
	if [ -f /etc/wifi/wpa_supplicant.conf ];then
		ssid=$(grep "\bssid=" /etc/wifi/wpa_supplicant.conf | sed 's/^[ \t]*//g')
		if [ -z "$ssid" ];then
			enter_info
		else
			password=$(grep "\bpsk=" /etc/wifi/wpa_supplicant.conf | sed 's/^[ \t]*//g')
			if [ -z $password ];then
				ttips "$ssid | No password"
			else
				ttips "$ssid | $password"
			fi

			ttrue "Whether to use the above default wifi configuration"
			[ $? = 0 ] || enter_info
		 fi
	 else
		 enter_info
	 fi
}

check_wlan
check_wificonf
connect
