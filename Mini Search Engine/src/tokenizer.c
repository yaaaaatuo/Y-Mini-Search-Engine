#include "../include/searchengine.h"

#define MAX_TOKENS 10000

/**
 * 将单词转换为小写并过滤标点符号
 * @param word: 要清洗的单词
 */
void cleanWord(char *word) {
    int i, j = 0;
    int len = strlen(word);

    for (i = 0; i < len; i++) {
        // 保留字母、数字和空格
        if (isalnum((unsigned char)word[i])) {
            word[j++] = tolower((unsigned char)word[i]);
        }
    }
    word[j] = '\0';
}

/**
 * 读取文件内容
 * @param filename: 文件名
 * @return: 文件内容字符串，需要调用者释放
 */
char *readFileContent(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        printf("错误：无法打开文件 %s\n", filename);
        return NULL;
    }

    // 获取文件大小
    fseek(file, 0, SEEK_END);
    long size = ftell(file);
    fseek(file, 0, SEEK_SET);

    // 分配内存
    char *content = (char *)malloc(size + 1);
    if (!content) {
        fclose(file);
        return NULL;
    }

    // 读取文件内容
    size_t readSize = fread(content, 1, size, file);
    content[readSize] = '\0';

    fclose(file);
    return content;
}

/**
 * 分词函数
 * @param text: 要分词的文本
 * @param tokenCount: 输出参数，返回分词数量
 * @return: 分词数组，需要调用者释放
 */
char **tokenize(const char *text, int *tokenCount) {
    if (!text || !tokenCount) return NULL;

    // 分配内存存储所有词
    char **tokens = (char **)malloc(MAX_TOKENS * sizeof(char *));
    if (!tokens) return NULL;

    // 复制文本以便处理
    char *textCopy = strdup(text);
    if (!textCopy) {
        free(tokens);
        return NULL;
    }

    *tokenCount = 0;
    char *p = textCopy;

    // 逐个字符处理
    char currentWord[MAX_WORD_LEN];
    int wordLen = 0;

    while (*p) {
        // 如果是字母或数字，添加到当前单词
        if (isalnum((unsigned char)*p)) {
            if (wordLen < MAX_WORD_LEN - 1) {
                currentWord[wordLen++] = tolower((unsigned char)*p);
            }
        } else {
            // 遇到分隔符，保存当前单词
            if (wordLen > 0) {
                currentWord[wordLen] = '\0';
                tokens[*tokenCount] = strdup(currentWord);
                (*tokenCount)++;
                wordLen = 0;

                if (*tokenCount >= MAX_TOKENS) break;
            }
        }
        p++;
    }

    // 处理最后一个单词
    if (wordLen > 0) {
        currentWord[wordLen] = '\0';
        tokens[*tokenCount] = strdup(currentWord);
        (*tokenCount)++;
    }

    free(textCopy);
    return tokens;
}

/**
 * 释放分词数组内存
 * @param tokens: 分词数组
 * @param count: 分词数量
 */
void freeTokens(char **tokens, int count) {
    if (!tokens) return;
    for (int i = 0; i < count; i++) {
        free(tokens[i]);
    }
    free(tokens);
}
