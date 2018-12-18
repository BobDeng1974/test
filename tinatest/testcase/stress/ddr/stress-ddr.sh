#!/bin/sh

##############################################################################
# \version    1.0.0
# \date       2017-12-25
# \author     liaoweixiong <liaoweixiong@allwinnertech.com>
# \default command
#             memtester 524288/1024 * 30/100 1
# \descriptions
#             <memtester dram_total_size_mb*test_percent loop_times>
#             implement loop_times memtester test test_size_mb
##############################################################################

# test_percent = 0: means nothing
test_percent=`mjson_fetch /stress/ddr/test_percent`
test_mb=`mjson_fetch /stress/ddr/test_size_mb`
loop_times=`mjson_fetch /stress/ddr/loop_times`

# get memory info
total_mb="$(( $(awk '/^MemTotal/{print $2}' /proc/meminfo) / 1024 ))"
free_mb="$(( $(awk '/^MemFree/{print $2}' /proc/meminfo) / 1024 ))"
buffers_mb="$(( $(awk '/^Buffers/{print $2}' /proc/meminfo) / 1024 ))"
cached_mb="$(( $(awk '/^Cached/{print $2}' /proc/meminfo) / 1024 ))"
freq_khz="$(cat /sys/class/devfreq/*/cur_freq 2>/dev/null | head -n 1)"

# check percent
[ "${test_percent}" -ge "100" -o "${test_percent}" -lt "0" ] \
    && echo "ERROR: Invalid percent ${test_percent}%" \
    && exit 1

# get test_size_mb from test_percent
if [ "${test_percent}" -ne "0" ]; then
    run_parallel=`mjson_fetch /stress/ddr/run_parallel`
    run_times=`mjson_fetch /stress/ddr/run_times`
    [ "${run_parallel}" = "true" ] \
        && [ $(( ${test_percent} * ${run_times} )) -ge 100 ] \
        && echo "ERROR: too large for ${test_percent}% with ${run_times} times in parallel" \
        && exit 1

    test_mb="$(( ${total_mb} * ${test_percent} / 100 ))"
fi

echo "---------- memory info -----------"
echo "total size: ${total_mb}MB"
echo "free size: ${free_mb}MB"
echo "cached size: ${cached_mb}MB"
echo "buffer size: ${buffers_mb}MB"
[ -n "${freq_khz}" ] && echo "dram freq: $(( ${freq_khz} / 1000 ))MHz"
[ ${test_percent} -gt 0 ] && echo "test percent: ${test_percent}%"
echo "test size: ${test_mb}MB"
echo "loop times: ${loop_times}"
echo
echo "---------- begin memtester -----------"
memtester ${test_mb}m ${loop_times}
