# ZFile
快速索引文件，包含Header和原始内容。初次加载只加载Header，跟进索引快速加载内容。
优点：代码简单，不支持多线程， 内存仅为Header
缺点：有一次文件IO

# LZFile
快速索引文件，包含Header和LZ4Compressed内容。初次加载Header+CompressedBlock，跟进索引快速加载内容。
优点：代码简单，支持多线程，内存为Header+CompressedBlock大小

# Bfile
快速索引文件，包含Header和Bin内容。初次加载只加载Header+BinBlock，根据索引快速加载内容。
优点：代码简单，支持多线程，内存为Header+Bin内容


