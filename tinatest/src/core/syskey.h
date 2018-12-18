#ifndef __MJSON_SYSKEY_H
#define __MJSON_SYSKEY_H
/* 详细说明见 tinatest/doc/CONFIG.txt */

/* global */
#define SYSKEY_GLOBAL_LIMIT_RUN_CNT_UP_TO "run_cnt_up_to"
#define SYSKEY_GLOBAL_LIMIT_TINATEST_RUN_TIME "tinatest_run_time"
// common
#define SYSKEY_GLOBAL_INFO_COLLECTD_INTERVAL_SEC "collectd_interval_sec"
#define SYSKEY_GLOBAL_INFO_COLLECTD_OUTDIR "outdir"
#define SYSKEY_GLOBAL_INFO_COLLECTD_ENABLE "enable"
#define SYSKEY_GLOBAL_INFO_COLLECTD_REPORT_TO_ABSOLUTE "report_to_absolute"
#define SYSKEY_GLOBAL_INFO_COLLECTD_REPORT_TO_PERCENTAGE "report_to_percentage"
#define SYSKEY_GLOBAL_INFO_COLLECTD_MATCH_SELLECT_OR_IGNORE "sellect_or_ignore"
#define SYSKEY_GLOBAL_INFO_COLLECTD_MATCH "match"
#define SYSKEY_GLOBAL_INFO_COLLECTD_MATCH_ALL "ALL"
#define SYSKEY_GLOBAL_INFO_COLLECTD_MATCH_SELLECT "sellect"
#define SYSKEY_GLOBAL_INFO_COLLECTD_MATCH_IGNORE "ignore"

// personal
//  测试项名
#define SYSKEY_GLOBAL_INFO_COLLECTD_CSV "collectd_csv"
#define SYSKEY_GLOBAL_INFO_COLLECTD_RRDTOOL "collectd_rrdtool"
#define SYSKEY_GLOBAL_INFO_COLLECTD_CPU "collectd_cpu"
#define SYSKEY_GLOBAL_INFO_COLLECTD_MEMORY "collectd_memory"
#define SYSKEY_GLOBAL_INFO_COLLECTD_DF "collectd_df"
#define SYSKEY_GLOBAL_INFO_COLLECTD_DISK "collectd_disk"
#define SYSKEY_GLOBAL_INFO_COLLECTD_FILECOUNT "collectd_filecount"
#define SYSKEY_GLOBAL_INFO_COLLECTD_PING "collectd_ping"
// 测试项私人属性
#define SYSKEY_GLOBAL_INFO_COLLECTD_DF_MATCH_DEVICE "device"
#define SYSKEY_GLOBAL_INFO_COLLECTD_DF_MATCH_MOUNTPOINT "mountpoint"
#define SYSKEY_GLOBAL_INFO_COLLECTD_DF_MATCH_FSTYPE "fstype"
#define SYSKEY_GLOBAL_INFO_COLLECTD_DISK_MATCH_DISK "disk_regular_expression"
#define SYSKEY_GLOBAL_INFO_COLLECTD_FILECOUNT_DIRECTORY "directory"
#define SYSKEY_GLOBAL_INFO_COLLECTD_FILECOUNT_INCLUDE_HIDDEN "include_hidden"
#define SYSKEY_GLOBAL_INFO_COLLECTD_FILECOUNT_INCLUDE_SUBDIR "include_subdir"
#define SYSKEY_GLOBAL_INFO_COLLECTD_FILECOUNT_MATCH_NAME "name"
#define SYSKEY_GLOBAL_INFO_COLLECTD_FILECOUNT_MATCH_SIZE "size"
#define SYSKEY_GLOBAL_INFO_COLLECTD_FILECOUNT_MATCH_MTIME "mtime"
#define SYSKEY_GLOBAL_INFO_COLLECTD_PING_HOST "host"
#define SYSKEY_GLOBAL_INFO_COLLECTD_PING_SEND_INTERVAL_SEC "send_interval_sec"
#define SYSKEY_GLOBAL_INFO_COLLECTD_PING_TIMEOUT_SEC "timeout_sec"
#define SYSKEY_GLOBAL_INFO_COLLECTD_PING_MAX_TTL "max_ttl"

/* local */
#define SYSKEY_LOCAL_LIMIT_RUN_TIMES "run_times"
#define SYSKEY_LOCAL_LIMIT_RUN_ALONE "run_alone"
#define SYSKEY_LOCAL_LIMIT_RUN_PARALLEL "run_parallel"
#define SYSKEY_LOCAL_LIMIT_MAY_REBOOT "may_reboot"
#define SYSKEY_LOCAL_LIMIT_TESTCASE_RUN_ONCE_TIME "testcase_run_once_time"
#define SYSKEY_LOCAL_LIMIT_TESTCASE_RUN_TIME "testcase_run_time"
#define SYSKEY_LOCAL_LIMIT_TIMEOUT_WITH_PASS "timeout_with_pass"
#define SYSKEY_LOCAL_LIMIT_TIMEOUT_WITH_FAILED "timeout_with_failed"
#define SYSKEY_LOCAL_LIMIT_EXIT_ONCE_FAILED "exit_once_failed"
#define SYSKEY_LOCAL_INFO_DATE "date"
#define SYSKEY_LOCAL_INFO_RESOURCE "resource"
#define SYSKEY_LOCAL_INFO_REAL_TIME_LOG "real_time_log"

/*任务属性配置关键字*/
#define SYSKEY_TASK_COMMAND "command"
#define SYSKEY_TASK_ENABLE "enable"
#define SYSKEY_TASK_STDIN "stdin"
#define SYSKEY_TASK_FSTDIN "fstdin"

//默认值(无则默认为0)
#define DEFAULT_GLOBAL_LIMIG_RUN_CNT_UP_TO 10
#define DEFAULT_GLOBAL_LIMIG_TINATEST_RUN_TIME_SEC (356 * 24 * 60 * 60)
#define DEFAULT_GLOBAL_OUTPATH "/mnt/UDISK/collectd"
#define DEFAULT_GLOBAL_COLLECTD_INTERVAL_SEC 2
#define DEFAULT_LOCAL_LIMIT_RUN_TIMES 1
#define DEFAULT_LOCAL_TESTCASE_RUN_ONCE_TIME_SEC (24 * 60 * 60)
#define DEFAULT_LOCAL_TESTCASE_RUN_TIME_SEC (24 * 60 * 60)

#endif
