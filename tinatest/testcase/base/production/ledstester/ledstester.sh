#####################################################################
# File Describe:ledstester.sh                                       |
# Author: flyranchao                                                |
# Email:  flyranchao@allwinnertech.com                              |
# Created Time:2017年10月25日 星期三 13时44分12秒                   |
#====================================================================
#!/bin/sh
ttips "Please look at the leds"
sleep 3
#record the init value of led's trigger
init_trigger=`cat /sys/class/leds/*/trigger | sed -r 's/.*\[(.*)\].*/\1/'`

#use timer to trigger the led ,make leds flickr 5 second
echo timer | tee /sys/class/leds/*/trigger &>/dev/null
sleep 5
echo "-----the set_trigger's  value of leds-----"
cat /sys/class/leds/*/trigger | sed -r 's/.*\[(.*)\].*/\1/'

#restore the init value of leds
echo "-----the init_trigger's value of leds-----"
count=1
for led in `ls /sys/class/leds`
do
	echo -n ${init_trigger} | cut -d ' ' -f ${count} | tee /sys/class/leds/${led}/trigger
	count=$(($count+1))
done
ttrue "All the leds Twinkle 5 times?"
