#include "lzfile.h"
#include "lz4.h"
#include <cassert>


JNIEXPORT const LZFile* LZOpen(const char* path, const char* mode) {
    LZFile* zfile = new LZFile();
    zfile->fd = fopen(path, mode);
    if(zfile->fd) {
        LOGI("open %s succeed", path);
    }
    return zfile;
}

JNIEXPORT const int LZLoadHeader(LZFile* zfile) {
    fread(&zfile->header.headerSize, sizeof(zfile->header.headerSize), 1, zfile->fd);
    fread(&zfile->header.nBlockNumber, sizeof(zfile->header.nBlockNumber), 1, zfile->fd);
    int nBlockNumber = zfile->header.headerSize / sizeof(LZBlockIndex);
    assert(nBlockNumber == zfile->header.nBlockNumber);
    LOGI("nBlockNumber = %d", nBlockNumber);
    for(int i=0; i < zfile->header.nBlockNumber; i++) {
        LZBlockIndex block{};
        fread(&block, sizeof(LZBlockIndex), 1, zfile->fd);
        // LOGI("0x%lx 0x%x 0x%x", block.xx32, block.offset, block.size);
        zfile->header.blockIndexMap[block.xx32] = block;
    }
    return 0;
}

JNIEXPORT const uint64_t LZGetBlockSize(LZFile* zfile, int key1, int key2) {
    uint64_t key = ((uint64_t)key1 << 32) + key2;
    //LOGI("key %lx key1 %x key2 %x", key, key1, key2);
    if(zfile->header.blockIndexMap.find(key) != zfile->header.blockIndexMap.end()) {
        LZBlockIndex block = zfile->header.blockIndexMap[key];
        return block.size;
    }
    return -1;
}

JNIEXPORT const uint64_t LZGetBlockUZSize(LZFile* zfile, int key1, int key2) {
    uint64_t key = ((uint64_t)key1 << 32) + key2;
    //LOGI("key %lx key1 %x key2 %x", key, key1, key2);
    if(zfile->header.blockIndexMap.find(key) != zfile->header.blockIndexMap.end()) {
        LZBlockIndex block = zfile->header.blockIndexMap[key];
        return block.uz_size;
    }
    return -1;
}

JNIEXPORT const uint64_t LZGetBlock(LZFile* zfile, int key1, int key2, char* buffer, int uz_size) {
    uint64_t key = ((uint64_t)key1 << 32) + key2;
    if(zfile->header.blockIndexMap.find(key) != zfile->header.blockIndexMap.end()) {
        LZBlockIndex block = zfile->header.blockIndexMap[key];
        fseek(zfile->fd, block.offset,SEEK_SET);
        assert(uz_size == block.uz_size);
        char* compress_data;
        fread(compress_data, block.size, 1, zfile->fd);
        return LZ4_decompress_safe(compress_data, buffer,block.size, uz_size);
    }
    return -1;
}

JNIEXPORT const int LZClose(LZFile* zfile) {
    if(zfile->fd) {
        fclose(zfile->fd);
        return 0;
    }
    return -1;
}