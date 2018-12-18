#!/bin/sh
path_in_config_tree="/stress/wifi"
connect_max=`mjson_fetch $path_in_config_tree/connect_count`
connect_count=0
fail_count=0
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

#	sleep 1

	while true
	do
		sleep 0.5
		let count++
		grep "CTRL-EVENT-CONNECTED" /tmp/wifi.log >/dev/null && {
			ip=$(/sbin/udhcpc -i wlan0 | awk '($2~/^setting/&&$3~/^default/) {print $5}');
			if [ -z ${ip} ];then
				exit 1
			else
				ttips "Test $connect_count times:Success " \
				-n "Routers IP:$ip"
				return 0
			fi
			break;
		}
		grep "pre-shared key may be incorrect" /tmp/wifi.log && {
			ttips "Test $connect_count times:FAILED" \
				-n  "PASSWARD INCORRECT"
			exit 1
		}
		if [ $count -eq 10000 ]; then
			ttips "Test $connect_count times:FAILED" \
				-n  "================= Connection timed out ============="
			return 0
		fi
	done
}

disconnect(){
	killall wpa_supplicant
	killall udhcpc
	ifconfig wlan0 down
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

check_connect_max(){
	ttrue "Connect $connect_max times, Please confirm."
	[ $? = 0 ] || {
		connect_max=$(task "Please enter the number of connections")
	}
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
	fi
}

check_wlan
check_wificonf
check_connect_max
for i in $(seq 1 $connect_max)
do
	let connect_count++
	connect
done
echo "==============================================="
echo "TEST $connect_count times"
echo "Success:$((connect_count-fail_count))"
echo "Failed:$fail_count"
echo "==============================================="
exit 0
