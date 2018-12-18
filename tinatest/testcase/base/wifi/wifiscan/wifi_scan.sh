#!/bin/sh
/bin/wifi_scan_results_test  > /tmp/wifi_scan.info

grep "Wifi get_scan_results: Success!" /tmp/wifi_scan.info

if [ $? -eq 0 ];then
	sed -n '/bssid/,$p' /tmp/wifi_scan.info | \
		awk '{print}'
	exit 0
else
	echo "======== WIFI SCAN FAILED ==========================="
	echo "======== For details, see /tmp/wifi_scan.info ========"
	exit 1
fi
