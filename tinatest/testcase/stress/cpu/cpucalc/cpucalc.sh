#!/bin/ash
echo "===$0 start!==="
echo "Default execut: stress-ng --all 5 --timeout 10m"

if [ $# = 0 -o x$1 = "x--help" ] ; then
	echo "you can execute : 'stress-ng --help' to view more parameters !"
	echo "here I've only done a few simple combination tests."
	echo "pi:Generating 5 worker for circumference algorithm to Generate pressure."
	echo "sock:Generate 5 worker calling socket correlation function to generate pressure."
	echo "sequential:To run 5 simultaneous instances of all the stressors sequentially one by one."
	echo "all:To run 5 instances of all the stressors for 10 minutes."
	exit
fi

cacl_test()
{
	case "$1" in
		pi)
			stress-ng --cpu 5 --cpu-method pi
			;;
		sock)
			stress-ng --sock 5
			;;
		sequential)
			stress-ng --sequential 5
			;;
		all)
			stress-ng --cpu 5 --cpu-method all --timeout 10m
			;;
		*)
			echo "Without this combination."
			return
			;;
	esac

}
cacl_test $1

echo "===$0 success==="
