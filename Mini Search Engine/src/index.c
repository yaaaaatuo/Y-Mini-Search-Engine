#include "../include/searchengine.h"

/**
 * 哈希函数（BKDR哈希）
 * @param word: 要哈希的单词
 * @param tableSize: 哈希表大小
 * @return: 哈希值
 */
unsigned int hash(const char *word, int tableSize) {
    unsigned int hashValue = 0;
    while (*word) {
        hashValue = hashValue * 131 + (unsigned char)*word;
        word++;
    }
    return hashValue % tableSize;
}

/**
 * 创建哈希表
 * @param size: 哈希表大小
 * @return: 哈希表指针
 */
HashTable *createHashTable(int size) {
    HashTable *table = (HashTable *)malloc(sizeof(HashTable));
    if (!table) return NULL;

    table->buckets = (IndexNode **)calloc(size, sizeof(IndexNode *));
    if (!table->buckets) {
        free(table);
        return NULL;
    }

    table->size = size;
    return table;
}

/**
 * 释放哈希表内存
 * @param table: 哈希表指针
 */
void freeHashTable(HashTable *table) {
    if (!table) return;

    for (int i = 0; i < table->size; i++) {
        IndexNode *node = table->buckets[i];
        while (node) {
            IndexNode *temp = node;
            node = node->next;
            free(temp);
        }
    }

    free(table->buckets);
    free(table);
}

/**
 * 在索引中查找单词
 * @param word: 要查找的单词
 * @return: 找到的索引节点，未找到返回NULL
 */
IndexNode *findInIndex(const char *word) {
    if (!indexTable || !word) return NULL;

    unsigned int h = hash(word, indexTable->size);
    IndexNode *node = indexTable->buckets[h];

    while (node) {
        if (strcmp(node->word, word) == 0) {
            return node;
        }
        node = node->next;
    }

    return NULL;
}

/**
 * 向索引中插入单词
 * @param word: 单词
 * @param fileId: 文件ID
 */
void insertToIndex(const char *word, int fileId) {
    if (!indexTable || !word || strlen(word) == 0) return;

    // 查找是否已存在
    IndexNode *node = findInIndex(word);

    if (node) {
        // 已存在，更新词频
        // 查找该文件是否已有记录
        for (int i = 0; i < node->freqCount; i++) {
            if (node->freqs[i].fileId == fileId) {
                node->freqs[i].count++;
                return;
            }
        }

        // 该文件第一次出现，添加新记录
        if (node->freqCount < MAX_FILES) {
            node->freqs[node->freqCount].fileId = fileId;
            node->freqs[node->freqCount].count = 1;
            node->freqCount++;
        }
    } else {
        // 不存在，创建新节点
        IndexNode *newNode = (IndexNode *)malloc(sizeof(IndexNode));
        if (!newNode) return;

        strncpy(newNode->word, word, MAX_WORD_LEN - 1);
        newNode->word[MAX_WORD_LEN - 1] = '\0';
        newNode->freqs[0].fileId = fileId;
        newNode->freqs[0].count = 1;
        newNode->freqCount = 1;
        newNode->next = NULL;

        // 插入到哈希表
        unsigned int h = hash(word, indexTable->size);
        newNode->next = indexTable->buckets[h];
        indexTable->buckets[h] = newNode;
    }
}

/**
 * 构建倒排索引
 * @param directory: 要扫描的目录
 */
void buildIndex(const char *directory) {
    // 扫描目录
    scanDirectory(directory);

    // 处理每个文件
    for (int i = 0; i < fileCount; i++) {
        // 读取文件内容
        char *content = readFileContent(fileTable[i].filename);
        if (!content) continue;

        // 分词
        int tokenCount = 0;
        char **tokens = tokenize(content, &tokenCount);

        // 添加到索引
        for (int j = 0; j < tokenCount; j++) {
            insertToIndex(tokens[j], fileTable[i].id);
        }

        // 释放内存
        for (int j = 0; j < tokenCount; j++) {
            free(tokens[j]);
        }
        free(tokens);
        free(content);
    }
}
