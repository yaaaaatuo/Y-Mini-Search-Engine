#include "../include/searchengine.h"

// 全局变量定义
FileInfo fileTable[MAX_FILES];
int fileCount = 0;
HashTable *indexTable = NULL;

int main(int argc, char *argv[]) {
    const char *dataDir = "data";

    if (argc > 1) {
        dataDir = argv[1];
    }

    printf("=== Mini Search Engine ===\n");
    printf("正在构建索引，请稍候...\n");

    // 初始化哈希表
    indexTable = createHashTable(INDEX_TABLE_SIZE);
    if (!indexTable) {
        printf("错误：无法创建索引表\n");
        return 1;
    }

    // 扫描目录并构建索引
    buildIndex(dataDir);

    printf("索引构建完成！\n");
    printf("共扫描 %d 个文件\n", fileCount);

    // 进入交互模式
    interactiveMode();

    // 清理资源
    freeHashTable(indexTable);

    return 0;
}
