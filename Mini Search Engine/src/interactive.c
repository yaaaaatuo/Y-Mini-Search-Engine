#include "../include/searchengine.h"

// 外部声明tokenize的free函数
extern void freeTokens(char **tokens, int count);

/**
 * 显示文件内容
 * @param fileId: 文件ID
 */
void displayFile(int fileId) {
    if (fileId < 0 || fileId >= fileCount) {
        printf("错误：无效的文件ID\n");
        return;
    }

    printf("\n=== 文件名：%s ===\n\n", fileTable[fileId].filename);

    char *content = readFileContent(fileTable[fileId].filename);
    if (content) {
        printf("%s\n", content);
        free(content);
    }
    printf("\n=== 文件结束 ===\n\n");
}

/**
 * 显示搜索结果
 * @param results: 结果数组
 * @param count: 结果数量
 */
void displayResults(SearchResult *results, int count) {
    if (!results || count == 0) {
        printf("未找到相关结果。\n");
        return;
    }

    printf("\n找到 %d 个结果（按相关性排序）：\n\n", count);
    for (int i = 0; i < count && i < 10; i++) {  // 最多显示10个结果
        int fileId = results[i].fileId;
        // 只显示文件名，不包含路径
        char *name = strrchr(fileTable[fileId].filename, '/');
        if (name) name++;
        else name = fileTable[fileId].filename;

        printf("%d. %s (出现 %d 次)\n", i + 1, name, results[i].totalCount);
    }
    printf("\n输入 'open <编号>' 查看文件内容，或输入新关键词继续搜索\n");
}

/**
 * 交互模式
 */
void interactiveMode() {
    char input[256];
    SearchResult *currentResults = NULL;
    int currentResultCount = 0;

    printf("\n支持的命令：\n");
    printf("  <关键词>      - 搜索文件\n");
    printf("  open <编号>   - 打开指定文件\n");
    printf("  exit/quit     - 退出程序\n");
    printf("  help          - 显示帮助\n\n");

    while (1) {
        printf("search> ");
        fflush(stdout);

        if (!fgets(input, sizeof(input), stdin)) {
            break;
        }

        // 去掉换行符
        input[strcspn(input, "\n")] = '\0';

        // 跳过空输入
        if (strlen(input) == 0) {
            continue;
        }

        // 解析命令
        if (strcmp(input, "exit") == 0 || strcmp(input, "quit") == 0) {
            printf("再见！\n");
            break;
        }

        if (strcmp(input, "help") == 0) {
            printf("\n支持的命令：\n");
            printf("  <关键词>      - 搜索文件（支持多个关键词，空格分隔）\n");
            printf("  open <编号>   - 打开指定文件\n");
            printf("  exit/quit     - 退出程序\n");
            printf("  help          - 显示帮助\n\n");
            continue;
        }

        // 检查是否为open命令
        if (strncmp(input, "open ", 5) == 0) {
            int num = atoi(input + 5);
            if (num > 0 && num <= currentResultCount) {
                displayFile(currentResults[num - 1].fileId);
            } else {
                printf("错误：无效的文件编号\n");
            }
            continue;
        }

        // 搜索关键词（支持多个关键词）
        // 释放之前的结果
        if (currentResults) {
            free(currentResults);
            currentResults = NULL;
        }

        // 分词处理输入
        int keywordCount = 0;
        char **keywords = tokenize(input, &keywordCount);

        if (keywordCount == 0) {
            printf("请输入有效的关键词\n");
            continue;
        }

        if (keywordCount == 1) {
            // 单关键词搜索
            currentResults = search(keywords[0], &currentResultCount);
        } else {
            // 多关键词搜索
            const char **kw = (const char **)malloc(keywordCount * sizeof(const char *));
            for (int i = 0; i < keywordCount; i++) {
                kw[i] = keywords[i];
            }
            currentResults = searchMultiple(kw, keywordCount, &currentResultCount);
            free(kw);
        }

        // 释放关键词内存
        for (int i = 0; i < keywordCount; i++) {
            free(keywords[i]);
        }
        free(keywords);

        displayResults(currentResults, currentResultCount);
    }

    // 清理
    if (currentResults) {
        free(currentResults);
    }
}
