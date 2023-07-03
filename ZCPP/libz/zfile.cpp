#include "zfile.h"
#include <cassert>


JNIEXPORT const ZFile* ZOpen(const char* path, const char* mode) {
    ZFile* zfile = new ZFile();
    zfile->fd = fopen(path, mode);
    if(zfile->fd) {
        LOGI("open %s succeed", path);
    }
    return zfile;
}

JNIEXPORT const int ZLoadHeader(ZFile* zfile) {
    fread(&zfile->header.headerSize, sizeof(zfile->header.headerSize), 1, zfile->fd);
    fread(&zfile->header.nBlockNumber, sizeof(zfile->header.nBlockNumber), 1, zfile->fd);
    int nBlockNumber = zfile->header.headerSize / sizeof(ZBlockIndex);    
    assert(nBlockNumber == zfile->header.nBlockNumber);
    LOGI("nBlockNumber = %d", nBlockNumber);
    for(int i=0; i < zfile->header.nBlockNumber; i++) {        
        ZBlockIndex block{};
        fread(&block, sizeof(ZBlockIndex), 1, zfile->fd);
        LOGI("0x%lx 0x%x 0x%x", block.xx32, block.offset, block.size);
        zfile->header.blockIndexMap[block.xx32] = block;
    }
    return 0;
}

JNIEXPORT const uint64_t ZGetBlockSize(ZFile* zfile, int key1, int key2) {
    uint64_t key = ((uint64_t)key1 << 32) + key2;
    LOGI("key %lx key1 %x key2 %x", key, key1, key2);
    if(zfile->header.blockIndexMap.find(key) != zfile->header.blockIndexMap.end()) {
        ZBlockIndex block = zfile->header.blockIndexMap[key];
        return block.size;
    }
    return -1;
}

JNIEXPORT const uint64_t ZGetBlock(ZFile* zfile, int key, char* buffer, int size) {
    if(zfile->header.blockIndexMap.find(key) != zfile->header.blockIndexMap.end()) {
        ZBlockIndex block = zfile->header.blockIndexMap[key];
        fseek(zfile->fd, block.offset,SEEK_SET);
        assert(size == block.size);
        return fread(buffer, block.size, 1, zfile->fd);
    }
    return -1;
}

JNIEXPORT const int ZClose(ZFile* zfile) {
    if(zfile->fd) {
        fclose(zfile->fd);
        return 0;
    }
    return -1;
}