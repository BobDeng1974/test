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
    grep "kB *reclen" -A 1 $1 | tail -n 1 | awk '{print $3}'
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
    # (in case of vfat, which support the most large 4G)
    [ "${size_k}" -ge $(( 4 * 1024 * 1024 )) ] \
        && size_k=$(( 3 * 1024 * 1024 + 921 * 1024 ))

    echo ${size_k}
}

# ====================== begin here ======================
tt_base="/spec/storage/seq"
dir=`mjson_fetch ${tt_base}/check_directory`
blk=`mjson_fetch ${tt_base}/block_size_kb`

ddr_k="$(get_ddr)"
[ -z "${dir}" ] \
    && echo "Miss directory, set default: /mnt/UDISK" \
    && dir="/mnt/UDISK"
[ -z "${blk}" ] && {
    echo -n "Miss block size, set default: "
    if [ "${ddr_k}" -lt "$(( 32 * 1024 ))" ]; then
        echo "512k"
        blk=512
    else
        echo "4m"
        blk=4096
    fi
}

total_k="$(get_total)"
free_k="$(get_free)"
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

log="/tmp/spec-seq.log"
# do test
iozone -a -i 0 -i 1 -s ${test_k}k -r ${blk}k -f ${dir}/iozone.tmp \
    | tee ${log}

# analyse speed
r_speed_k="$(get_r_speed ${log})"
w_speed_k="$(get_w_speed ${log})"
rm ${log}
echo
echo "read speed: ${r_speed_k} KB/s"
echo "write speed: ${w_speed_k} KB/s"

ttips "read speed: ${r_speed_k} KB/s" \
   -n "write speed: ${w_speed_k} KB/s"
