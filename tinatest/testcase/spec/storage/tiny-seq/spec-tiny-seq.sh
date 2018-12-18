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

get_time()
{
    local min="$(awk '/real/{print $2}' $1 | sed 's/m//')"
    local sec="$(awk '/real/{print $3}' $1 | sed 's/[s\.]//g' | sed 's/^0*//')"

    echo $(( ${min} * 60 * 100 + ${sec} ))
}

get_r_speed()
{
    [ -z "${sec}" ] && sec=$(get_time $1)

    [ "${sec}" -le "0" ] \
        && echo "Get Speed Failed" \
        && exit 1

    echo $(( ${test_k} * 100 / ${sec} ))
}

get_w_speed()
{
    [ -z "${sec}" ] && sec=$(get_time $1)

    [ "${sec}" -le "0" ] \
        && echo "Get Speed Failed" \
        && exit 1

    echo $(( ${test_k} * 100 / ${sec} ))
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

do_tiny_test()
{
    in="$1"
    out="$2"
    bs="${blk}k"
    count="$(( ${test_k} / ${blk} ))"
    [ "${out}" = "/dev/null" ] \
        && args="if=${in} of=${out} bs=${bs} count=${count}" \
        || args="if=${in} of=${out} bs=${bs} count=${count} conv=fsync"
    echo dd ${args}
    time dd ${args}
}

# ====================== begin here ======================
tt_base="/spec/storage/tiny-seq"
dir=`mjson_fetch ${tt_base}/check_directory`
blk=`mjson_fetch ${tt_base}/block_size_kb`

ddr_k="$(get_ddr)"
[ "${ddr_k}" -ge "$(( 32 * 1024 ))" ] \
    && echo "This testcase is work for ddr less than 32M" \
    && echo "You can use /spec/storage/seq instead" \
    && exit 1
[ -z "${dir}" ] \
    && echo "Miss directory, set default: /mnt/SDCARD" \
    && dir="/mnt/SDCARD"
[ -z "${blk}" ] \
    && echo -n "Miss block size, set default: 512K" \
    && blk=512
[ "${blk}" -gt "$(( 4 * 1024 ))" ] \
    && echo -n "block size should not larger than 4m, use 512k instead" \
    && blk=512

total_k="$(get_total)"
free_k="$(get_free)"
# test size depends on free flash and total ddr.
test_k="$(( $(get_test) / ${blk} * ${blk} ))"
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

log="/tmp/spec-tiny-seq.log"
# do test for write
do_tiny_test "/dev/zero" "${dir}/dd.tmp" 2>&1 | tee ${log}.w
echo
# free memory
echo sync
time sync
echo
echo free caches
echo 3 > /proc/sys/vm/drop_caches
echo
# do test for read
do_tiny_test "${dir}/dd.tmp" "/dev/null" 2>&1 | tee ${log}.r
echo

# analyse speed
r_speed_k="$(get_r_speed ${log}.r)"
w_speed_k="$(get_w_speed ${log}.w)"
rm -f ${log}.r ${log}.w ${dir}/dd.tmp
echo
[ -z ${r_speed_k} -o -z ${w_speed_k} ] \
    && echo "Get Speed Failed" \
    && exit 1
echo "read speed: ${r_speed_k} KB/s"
echo "write speed: ${w_speed_k} KB/s"

ttips "read speed: ${r_speed_k} KB/s" \
   -n "write speed: ${w_speed_k} KB/s"
