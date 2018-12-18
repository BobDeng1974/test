#!/bin/sh

get_total()
{
    df -k ${dir} 2>/dev/null | tail -n 1 | awk '{print $2}'
}

get_free()
{
    df -k ${dir} 2>/dev/null | tail -n 1 | awk '{print $4}'
}

get_ddr()
{
    awk '/^MemTotal/{print $2}' /proc/meminfo
}

get_flash()
{
    df -k ${dir} 2>/dev/null | tail -n 1 | awk '{print $1}'
}

get_fs()
{
    df -T ${dir} 2>/dev/null | tail -n 1 | awk '{print $2}'
}

get_r_speed()
{
    grep "kB *reclen" -A 1 $1 | tail -n 1 | awk '{print $5}'
}

get_w_speed()
{
    grep "kB *reclen" -A 1 $1 | tail -n 1 | awk '{print $6}'
}

# test size depends on free flash and total ddr.
# 2 times bigger than free
get_test()
{
    local size_k
    # support 2 * ddr, or 95% of free.
    [ "$(( ${ddr_k} * 2 ))" -lt "${free_k}" ] \
        && size_k=$(( ${ddr_k} * 2 )) \
        || size_k=$(( ${free_k} * 95 / 100 ))

    # if larger than 4G, use around 3.9G
    [ "${size_k}" -ge $(( 4 * 1024 * 1024 )) ] \
        && size_k=$(( 3 * 1024 * 1024 + 921 * 1024 ))

    echo ${size_k}
}

# ====================== begin here ======================
tt_base="/spec/storage/rand"
dir=`mjson_fetch ${tt_base}/check_directory`
blk=`mjson_fetch ${tt_base}/block_size_kb`

[ -z "${dir}" ] \
    && echo "Miss directory, set default: /mnt/UDISK" \
    && dir="/mnt/UDISK"
[ -z "${blk}" ] \
    && echo "Miss block size, set default: 4K" \
    && blk=4

total_k="$(get_total)"
free_k="$(get_free)"
ddr_k="$(get_ddr)"
# test size depends on free flash and total ddr.
test_k="$(get_test)"
flash="$(get_flash)"
fs="$(get_fs)"

echo
echo "flash: ${flash}"
echo "filesystem: ${fs}"
echo "flash total size: ${total_k} KB"
echo "flash free size: ${free_k} KB"
echo "flash test size: ${test_k} KB"
echo "ddr tital size: ${ddr_k} KB"
echo "test directory: ${dir}"
echo "test block size: ${blk} KB"
echo

log="/tmp/spec-rand.log"
# do test
iozone -a -i 0 -i 2 -s ${test_k}k -r ${blk}k -f ${dir}/iozone.tmp \
    | tee ${log}

# analyse speed
r_speed_k="$(get_r_speed ${log})"
w_speed_k="$(get_w_speed ${log})"
r_iops="$(( ${r_speed_k} / ${blk} ))"
w_iops="$(( ${r_speed_k} / ${blk} ))"
rm ${log}
echo
echo "read speed: ${r_speed_k} KB/s"
echo "write speed: ${w_speed_k} KB/s"
echo "read iops: ${r_iops}"
echo "write iops: ${w_iops}"

ttips "read speed: ${r_speed_k} KB/s" \
   -n "write speed: ${w_speed_k} KB/s" \
   -n "read iops: ${r_iops}" \
   -n "write iops: ${w_iops}"
