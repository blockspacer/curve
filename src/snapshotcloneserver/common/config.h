/*************************************************************************
 > File Name: config.h
 > Author:
 > Created Time: Wed Nov 21 11:33:46 2018
    > Copyright (c) 2018 netease
 ************************************************************************/

#ifndef SRC_SNAPSHOTCLONESERVER_COMMON_CONFIG_H_
#define SRC_SNAPSHOTCLONESERVER_COMMON_CONFIG_H_


#include<string>
#include <vector>

namespace curve {
namespace snapshotcloneserver {
// curve client options
struct CurveClientOptions {
    // config path
    std::string configPath;
    // mds root user
    std::string mdsRootUser;
    // mds root password
    std::string mdsRootPassword;
    // 调用client方法的重试总时间
    uint64_t clientMethodRetryTimeSec;
    // 调用client方法重试间隔时间
    uint64_t clientMethodRetryIntervalMs;
};

// snapshotcloneserver options
struct SnapshotCloneServerOptions {
    // snapshot&clone server address
    std::string  addr;
    // 调用client异步方法重试总时间
    uint64_t clientAsyncMethodRetryTimeSec;
    // 调用client异步方法重试时间间隔
    uint64_t clientAsyncMethodRetryIntervalMs;
    // 快照工作线程数
    int snapshotPoolThreadNum;
    // 快照后台线程扫描等待队列和工作队列的扫描周期(单位：ms)
    uint32_t snapshotTaskManagerScanIntervalMs;
    // 转储chunk分片大小
    uint64_t chunkSplitSize;
    // CheckSnapShotStatus调用间隔
    uint32_t checkSnapshotStatusIntervalMs;
    // 最大快照数
    uint32_t maxSnapshotLimit;
    // snapshotcore threadpool threadNum
    uint32_t snapshotCoreThreadNum;
    // mdsSessionTimeUs
    uint32_t mdsSessionTimeUs;
    // ReadChunkSnapshot同时进行的异步请求数量
    uint32_t readChunkSnapshotConcurrency;

    // 用于Lazy克隆元数据部分的线程池线程数
    int stage1PoolThreadNum;
    // 用于Lazy克隆数据部分的线程池线程数
    int stage2PoolThreadNum;
    // 用于非Lazy克隆和删除克隆等其他管控面的请求的线程池线程数
    int commonPoolThreadNum;
    // CloneTaskManager 后台线程扫描间隔
    uint32_t cloneTaskManagerScanIntervalMs;
    // clone chunk分片大小
    uint64_t cloneChunkSplitSize;
    // 克隆临时目录
    std::string cloneTempDir;
    // mds root user
    std::string mdsRootUser;
    // CreateCloneChunk同时进行的异步请求数量
    uint32_t createCloneChunkConcurrency;
    // RecoverChunk同时进行的异步请求数量
    uint32_t recoverChunkConcurrency;
};

}  // namespace snapshotcloneserver
}  // namespace curve
#endif  // SRC_SNAPSHOTCLONESERVER_COMMON_CONFIG_H_
