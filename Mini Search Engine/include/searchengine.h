#ifndef SEARCHENGINE_H
#define SEARCHENGINE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <dirent.h>
#include <sys/stat.h>

#define MAX_FILES 100
#define MAX_WORD_LEN 50
#define MAX_FILENAME_LEN 100
#define INDEX_TABLE_SIZE 10007

// 文件信息结构体
typedef struct {
    int id;
    char filename[MAX_FILENAME_LEN];
} FileInfo;

// 词频统计节点
typedef struct {
    int fileId;
    int count;
} WordFreq;

// 词条节点（倒排索引）
typedef struct IndexNode {
    char word[MAX_WORD_LEN];
    WordFreq freqs[MAX_FILES];
    int freqCount;
    struct IndexNode *next;
} IndexNode;

// 哈希表结构
typedef struct {
    IndexNode **buckets;
    int size;
} HashTable;

// 搜索结果结构体
typedef struct {
    int fileId;
    int totalCount;
} SearchResult;

// 全局变量
extern FileInfo fileTable[MAX_FILES];
extern int fileCount;
extern HashTable *indexTable;

// 文件扫描模块
int scanDirectory(const char *path);

// 分词和文本处理模块
char **tokenize(const char *text, int *tokenCount);
void cleanWord(char *word);
char *readFileContent(const char *filename);

// 哈希表和倒排索引模块
unsigned int hash(const char *word, int tableSize);
HashTable *createHashTable(int size);
void insertToIndex(const char *word, int fileId);
IndexNode *findInIndex(const char *word);
void buildIndex(const char *directory);
void freeHashTable(HashTable *table);

// 搜索模块
SearchResult *search(const char *keyword, int *resultCount);
SearchResult *searchMultiple(const char **keywords, int keywordCount, int *resultCount);
int compareSearchResult(const void *a, const void *b);

// 交互模块
void interactiveMode();
void displayFile(int fileId);

#endif
