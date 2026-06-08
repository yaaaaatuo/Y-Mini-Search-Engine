#include "../include/searchengine.h"

/**
 * 搜索结果比较函数（用于qsort降序排序）
 * @param a: 结果a
 * @param b: 结果b
 * @return: 比较结果
 */
int compareSearchResult(const void *a, const void *b) {
    const SearchResult *ra = (const SearchResult *)a;
    const SearchResult *rb = (const SearchResult *)b;
    return rb->totalCount - ra->totalCount;  // 降序
}

/**
 * 搜索关键词
 * @param keyword: 关键词
 * @param resultCount: 输出结果数量
 * @return: 搜索结果数组，需要调用者释放
 */
SearchResult *search(const char *keyword, int *resultCount) {
    if (!keyword || !resultCount || !indexTable) return NULL;

    // 清洗关键词
    char cleanKeyword[MAX_WORD_LEN];
    strncpy(cleanKeyword, keyword, MAX_WORD_LEN - 1);
    cleanKeyword[MAX_WORD_LEN - 1] = '\0';

    // 转换为小写
    for (int i = 0; cleanKeyword[i]; i++) {
        cleanKeyword[i] = tolower((unsigned char)cleanKeyword[i]);
    }

    // 在索引中查找
    IndexNode *node = findInIndex(cleanKeyword);
    if (!node) {
        *resultCount = 0;
        return NULL;
    }

    // 分配结果数组
    SearchResult *results = (SearchResult *)malloc(node->freqCount * sizeof(SearchResult));
    if (!results) {
        *resultCount = 0;
        return NULL;
    }

    // 复制结果
    *resultCount = node->freqCount;
    for (int i = 0; i < node->freqCount; i++) {
        results[i].fileId = node->freqs[i].fileId;
        results[i].totalCount = node->freqs[i].count;
    }

    // 按词频排序（降序）
    qsort(results, *resultCount, sizeof(SearchResult), compareSearchResult);

    return results;
}

/**
 * 多关键词搜索（OR关系）
 * @param keywords: 关键词数组
 * @param keywordCount: 关键词数量
 * @param resultCount: 输出结果数量
 * @return: 搜索结果数组
 */
SearchResult *searchMultiple(const char **keywords, int keywordCount, int *resultCount) {
    if (!keywords || keywordCount <= 0 || !resultCount || !indexTable) return NULL;

    // 临时存储每个文件的出现次数
    int fileScores[MAX_FILES] = {0};
    int filesFound[MAX_FILES] = {0};
    int uniqueFiles = 0;

    // 对每个关键词进行搜索
    for (int k = 0; k < keywordCount; k++) {
        char cleanKeyword[MAX_WORD_LEN];
        strncpy(cleanKeyword, keywords[k], MAX_WORD_LEN - 1);
        cleanKeyword[MAX_WORD_LEN - 1] = '\0';

        for (int i = 0; cleanKeyword[i]; i++) {
            cleanKeyword[i] = tolower((unsigned char)cleanKeyword[i]);
        }

        IndexNode *node = findInIndex(cleanKeyword);
        if (node) {
            for (int i = 0; i < node->freqCount; i++) {
                int fileId = node->freqs[i].fileId;
                if (fileScores[fileId] == 0) {
                    filesFound[uniqueFiles++] = fileId;
                }
                fileScores[fileId] += node->freqs[i].count;
            }
        }
    }

    if (uniqueFiles == 0) {
        *resultCount = 0;
        return NULL;
    }

    // 创建结果数组
    SearchResult *results = (SearchResult *)malloc(uniqueFiles * sizeof(SearchResult));
    if (!results) {
        *resultCount = 0;
        return NULL;
    }

    *resultCount = uniqueFiles;
    for (int i = 0; i < uniqueFiles; i++) {
        results[i].fileId = filesFound[i];
        results[i].totalCount = fileScores[filesFound[i]];
    }

    // 排序
    qsort(results, *resultCount, sizeof(SearchResult), compareSearchResult);

    return results;
}
