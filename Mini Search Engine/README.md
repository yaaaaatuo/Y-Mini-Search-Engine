# Mini Search Engine - 基于C语言的本地文本搜索引擎

## 项目概述

这是一个使用C语言实现的本地文本搜索引擎，支持倒排索引、哈希表优化、多关键词搜索等功能。

## 项目结构

```
.
├── include/
│   └── searchengine.h    # 头文件
├── src/
│   ├── main.c            # 主程序入口
│   ├── file_scanner.c    # 文件扫描模块
│   ├── tokenizer.c       # 分词和文本处理模块
│   ├── index.c           # 倒排索引和哈希表模块
│   ├── search.c          # 搜索和排序模块
│   └── interactive.c     # 交互式命令行模块
├── data/                 # 测试数据目录
│   ├── article1.txt
│   ├── ai_note.txt
│   ├── learning_log.txt
│   └── programming.txt
├── Makefile              # 编译脚本
└── README.md             # 项目说明
```

## 核心功能

1. **文档扫描**：扫描指定目录中的所有 `.txt` 文本文件
2. **分词处理**：将文本拆分为单词，统一转换为小写，过滤标点符号
3. **倒排索引**：建立单词到文件的映射关系
4. **哈希表优化**：使用BKDR哈希算法实现O(1)时间复杂度的查找
5. **搜索排序**：按词频降序排序搜索结果
6. **多关键词搜索**：支持同时搜索多个关键词

## 数据结构设计

```c
// 文件信息
typedef struct {
    int id;
    char filename[MAX_FILENAME_LEN];
} FileInfo;

// 倒排索引节点
typedef struct IndexNode {
    char word[MAX_WORD_LEN];
    WordFreq freqs[MAX_FILES];
    int freqCount;
    struct IndexNode *next;
} IndexNode;

// 哈希表
typedef struct {
    IndexNode **buckets;
    int size;
} HashTable;
```

## 编译运行

### 编译
```bash
make
```

### 运行
```bash
make run
# 或
./bin/search_engine data
```

### 清理
```bash
make clean
```

## 使用说明

启动程序后会自动构建索引，然后进入交互模式：

```
search> chatgpt

找到 X 个结果（按相关性排序）：
1. ai_note.txt (出现 15 次)
2. article1.txt (出现 10 次)
3. learning_log.txt (出现 8 次)

输入 'open <编号>' 查看文件内容，或输入新关键词继续搜索
search> open 1

=== 文件名：data/ai_note.txt ===
...

search> exit
再见！
```

### 支持的命令
- `<关键词>` - 搜索文件（支持多个关键词，空格分隔）
- `open <编号>` - 打开指定文件
- `exit/quit` - 退出程序
- `help` - 显示帮助

## 技术亮点

1. **倒排索引（Inverted Index）**：搜索引擎核心数据结构，从O(n)优化到接近O(1)的查找效率
2. **哈希表优化**：使用BKDR哈希算法处理冲突，高效存储和检索索引
3. **快速排序**：使用qsort对搜索结果按相关度排序
4. **文本清洗**：统一大小写、过滤标点符号，提高搜索准确性

## 版本规划

- ✅ **V1 基础版**：单关键词搜索、倒排索引、哈希表
- ✅ **V2 多关键词搜索**：支持多个关键词同时搜索
- 🔄 **V3 布尔检索**：支持 AND/OR 逻辑运算
- 🔄 **V4 搜索结果高亮**：高亮显示匹配的关键词
- 🔄 **V5 索引持久化**：将索引保存到文件，加速启动

## 学习价值

- C语言工程化开发实践
- 文件系统操作
- 数据结构与算法应用（哈希表、快速排序）
- 搜索引擎基础原理
- 项目模块化设计

