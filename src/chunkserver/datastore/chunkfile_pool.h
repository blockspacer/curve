/*
 * Project: curve
 * File Created: Monday, 10th December 2018 9:54:34 am
 * Author: tongguangxun
 * Copyright (c)￼ 2018 netease
 */

#ifndef CHUNKSERVER_FILEPOOL_H
#define CHUNKSERVER_FILEPOOL_H

#include <glog/logging.h>

#include <set>
#include <mutex>  // NOLINT
#include <vector>
#include <string>
#include <memory>
#include <deque>
#include <atomic>

#include "src/fs/local_filesystem.h"
#include "include/curve_compiler_specific.h"

using curve::fs::LocalFileSystem;
namespace curve {
namespace chunkserver {

// chunkfilepool 配置选项
struct ChunkfilePoolOptions {
    // 开关，是否从chunkfile pool取chunk
    bool        getChunkFromPool;

    // chunkfilepool 文件夹路径,当getChunkFromPool为false的时候，需要设置该选项
    char        chunkFilePoolDir[256];

    // 配置文件的chunk大小
    uint32_t    chunkSize;

    // 配置文件中的metapage大小
    uint32_t    metaPageSize;

    // chunkfilepool meta文件地址
    char        metaPath[256];

    // cpmetafilesize是chunkfilepool的 metafile长度
    uint32_t    cpMetaFileSize;

    // GetChunk重试次数
    uint16_t    retryTimes;

    ChunkfilePoolOptions() {
        getChunkFromPool = true;
        cpMetaFileSize = 4096;
        chunkSize = 0;
        metaPageSize = 0;
        retryTimes = 5;
        ::memset(metaPath, 0, 256);
        ::memset(chunkFilePoolDir, 0, 256);
    }

    ChunkfilePoolOptions& operator=(const ChunkfilePoolOptions& other) {
        getChunkFromPool   = other.getChunkFromPool;
        cpMetaFileSize     = other.cpMetaFileSize;
        chunkSize    = other.chunkSize;
        retryTimes   = other.retryTimes;
        metaPageSize = other.metaPageSize;
        ::memcpy(metaPath, other.metaPath, 256);
        ::memcpy(chunkFilePoolDir, other.chunkFilePoolDir, 256);
        return *this;
    }

    ChunkfilePoolOptions(const ChunkfilePoolOptions& other) {
        getChunkFromPool   = other.getChunkFromPool;
        cpMetaFileSize     = other.cpMetaFileSize;
        chunkSize    = other.chunkSize;
        retryTimes   = other.retryTimes;
        metaPageSize = other.metaPageSize;
        ::memcpy(metaPath, other.metaPath, 256);
        ::memcpy(chunkFilePoolDir, other.chunkFilePoolDir, 256);
    }
};

class CURVE_CACHELINE_ALIGNMENT ChunkfilePool {
 public:
    // fsptr 本地文件系统.
    explicit ChunkfilePool(std::shared_ptr<LocalFileSystem> fsptr);
    virtual ~ChunkfilePool() = default;

    /**
     * 初始化函数
     * @param: cfop是配置选项
     */
    virtual bool Initialize(const ChunkfilePoolOptions& cfop);
    /**
     * datastore通过GetChunk接口获取新的chunk，GetChunk内部会将metapage原子赋值后返回。
     * @param: chunkpath是新的chunkfile路径
     * @param: metapage是新的chunk的metapage信息
     */
    virtual int GetChunk(const std::string& chunkpath, char* metapage);
    /**
     * datastore删除chunk直接回收，不真正删除
     * @param: chunkpath是需要回收的chunk路径
     */
    virtual int RecycleChunk(const std::string& chunkpath);
    /**
     * 获取当前chunkfile pool大小
     */
    virtual size_t Size();
    /**
     * 析构,释放资源
     */
    virtual void UnInitialize();

 private:
    // 从chunkfile pool目录中遍历预分配的chunk信息
    bool ScanInternal();
    // 检查chunkfile pool预分配是否合法
    bool CheckValid();
    /**
     * 为新的chunkfile进行metapage赋值
     * @param: sourcepath为要写入的文件路径
     * @param: page为要写入的metapage信息
     * @return: 成功返回0，否则返回小于0
     */
    int WriteMetaPage(const std::string& sourcepath, char* page);
    /**
     * 直接分配chunk，不从chunkfilepool获取
     * @param: chunkpath为datastore中chunk文件的路径
     * @return: 成功返回0，否则返回小于0
     */
    int AllocateChunk(const std::string& chunkpath);

 private:
    // 保护tmpChunkvec_
    std::mutex mtx_;

    // 当前chunkfilepool的预分配文件，文件夹路径
    std::string currentdir_;

    // chunkserver端封装的底层文件系统接口，提供操作文件的基本接口
    std::shared_ptr<LocalFileSystem> fsptr_;

    // 内存中持有的chunkfile pool中的文件名的数字格式
    std::vector<uint64_t> tmpChunkvec_;

    // 当前最大的文件名数字格式
    std::atomic<uint64_t> currentmaxfilenum_;

    // chunkfilepool配置选项
    ChunkfilePoolOptions chunkPoolOpt_;
};
}   // namespace chunkserver
}   // namespace curve

#endif