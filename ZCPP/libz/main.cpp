#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include "lzfile.h"
#include "xxhash.h"

using namespace std;

vector<vector<string>> readCSV(const string& filename) {
    vector<vector<string>> data;
    ifstream file(filename);

    if (!file.is_open()) {
        cout << "Failed to open file: " << filename << endl;
        return data;
    }

    string line;
    while (getline(file, line)) {
        vector<string> row;
        stringstream ss(line);
        string cell;

        while (getline(ss, cell, ',')) {
            row.push_back(cell);
        }

        data.push_back(row);
    }

    file.close();
    return data;
}

// int main(int argc, const char** argv) {    

//     string input_file = string(argv[1]);
//     ifstream file(input_file);
//     if (!file.is_open()) {
//         cout << "Failed to open file: " << input_file << endl;
//         return -1;
//     }
//     cout << input_file.c_str() << std::endl;
//     //calc header
//     std::vector<ZBlock> BlockList;
//     string line;
//     int i = 0;
//     while (getline(file, line)) {                
//         if (i == 0)
//         {
//             i++;
//             continue;
//         }        
//         std::istringstream iss(line);
//         std::string key1;
//         std::getline(iss, key1, ',');
//         std::string key2;
//         std::getline(iss, key2, ',');
//         uint64_t key = (stoull(key1) << 32) + stoull(key2);
//         cout << std::hex << key << " " << key1 << " " << key2 << std::endl;
//         ZBlock block;
//         block.xx32 = key;
//         block.line = line;

//         BlockList.push_back(block);
//     }
//     cout << "Block Number:" << BlockList.size() << std::endl;
//     ZHeader header;
//     ZBlockIndex* pBlockIndexs = new ZBlockIndex[BlockList.size()];
//     auto out = input_file + ".zbin";
//     cout << "Out File:" << out << std::endl;
//     auto fd = fopen(out.c_str(), "wb");
//     if (!fd) {
//         cout << "Failed to open file: " << out << endl;    
//         return -1; 
//     }
//     auto blockOffset = sizeof(header.headerSize) + sizeof(header.nBlockNumber) + BlockList.size() * sizeof(ZBlockIndex);
//     //write header    
//     auto offset = blockOffset;    
//     fseek(fd, offset, SEEK_SET);
//     for (int i = 0; i < BlockList.size(); i++) {
//         offset += i * sizeof(ZBlock);
//         cout << "Key:" << std::hex << BlockList[i].xx32 << "Block Offset:" << std::hex << offset << std::endl;
//         pBlockIndexs[i].xx32 = BlockList[i].xx32;
//         pBlockIndexs[i].offset = offset;
//         pBlockIndexs[i].size = BlockList[i].line.size();        
//         fwrite(BlockList[i].line.c_str(), BlockList[i].line.size(), 1, fd);        
//     }

//     fseek(fd, 0, SEEK_SET);
//     header.headerSize = sizeof(ZBlockIndex) * BlockList.size();
//     header.nBlockNumber = BlockList.size();
//     fwrite(&header.headerSize, sizeof(header.headerSize), 1, fd);
//     fwrite(&header.nBlockNumber, sizeof(header.nBlockNumber), 1, fd);
//     for (int i = 0; i < BlockList.size(); i++) {
//         fwrite(&pBlockIndexs[i], sizeof(ZBlockIndex), 1, fd);
//     }
//     fclose(fd);
//     return 0;
// }

int main(int argc, const char** argv) {    

    string input_file = string(argv[1]);
    ifstream file(input_file);
    if (!file.is_open()) {
        cout << "Failed to open file: " << input_file << endl;
        return -1;
    }
    cout << input_file.c_str() << std::endl;
    //calc header
    std::vector<LZBlock> BlockList;
    string line;
    int i = 0;

    // 读取文件
    while (getline(file, line)) {                
        if (i == 0)
        {
            i++;
            continue;
        }

        // 读取单行   
        std::istringstream iss(line);
        std::string id;
        std::getline(iss, id, ',');
        std::string level;
        std::getline(iss, level, ',');
        uint64_t key = (stoull(id) << 32) + stoull(level);
        cout << std::hex << key << " " << id << " " << level << std::endl;

        // 压缩block
        int src_size = line.length();
        int max_dst_size = LZ4_compressBound(src_size);
        char* compress_line = new char[max_dst_size];
        int dst_size = LZ4_compress_default(line.c_str(), compress_line, line.length(), max_dst_size);

        LZBlock block;
        block.xx32 = key;
        block.line = compress_line;
        block.uz_size = src_size;
        BlockList.push_back(block);
    }
    cout << "Block Number:" << BlockList.size() << std::endl;
    LZHeader header;
    
    LZBlockIndex* pBlockIndexs = new LZBlockIndex[BlockList.size()];

    auto out = input_file + ".lzbin";
    cout << "Out File:" << out << std::endl;
    auto fd = fopen(out.c_str(), "wb");
    if (!fd) {
        cout << "Failed to open file: " << out << endl;    
        return -1; 
    }
    
    auto blockOffset = sizeof(header.headerSize) + sizeof(header.nBlockNumber) + BlockList.size() * sizeof(LZBlockIndex);
    
    //write header    
    auto offset = blockOffset;    
    fseek(fd, offset, SEEK_SET);
    for (int i = 0; i < BlockList.size(); i++) {
        offset += i * sizeof(LZBlock);
        cout << "Key:" << std::hex << BlockList[i].xx32 << "Block Offset:" << std::hex << offset << std::endl;
        pBlockIndexs[i].xx32 = BlockList[i].xx32;
        pBlockIndexs[i].offset = offset;
        pBlockIndexs[i].size = BlockList[i].line.size();
        pBlockIndexs[i].uz_size = BlockList[i].uz_size;
        fwrite(BlockList[i].line.c_str(), BlockList[i].line.size(), 1, fd);
    }

    fseek(fd, 0, SEEK_SET);
    header.headerSize = sizeof(LZBlockIndex) * BlockList.size();
    header.nBlockNumber = BlockList.size();
    fwrite(&header.headerSize, sizeof(header.headerSize), 1, fd);
    fwrite(&header.nBlockNumber, sizeof(header.nBlockNumber), 1, fd);
    for (int i = 0; i < BlockList.size(); i++) {
        fwrite(&pBlockIndexs[i], sizeof(LZBlockIndex), 1, fd);
    }
    fclose(fd);
    return 0;
}