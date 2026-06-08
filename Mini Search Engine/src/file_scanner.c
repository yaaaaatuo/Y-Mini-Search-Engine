#include "../include/searchengine.h"

/**
 * 扫描目录中的所有文本文件
 * @param path: 要扫描的目录路径
 * @return: 成功返回扫描到的文件数量，失败返回-1
 */
int scanDirectory(const char *path) {
    DIR *dir;
    struct dirent *entry;
    struct stat statbuf;
    char fullPath[256];
    int count = 0;

    dir = opendir(path);
    if (!dir) {
        printf("错误：无法打开目录 %s\n", path);
        return -1;
    }

    // 遍历目录中的所有文件
    while ((entry = readdir(dir)) != NULL && fileCount < MAX_FILES) {
        // 跳过当前目录和父目录
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
            continue;
        }

        // 构建完整路径
        snprintf(fullPath, sizeof(fullPath), "%s/%s", path, entry->d_name);

        // 获取文件状态
        if (stat(fullPath, &statbuf) == -1) {
            continue;
        }

        // 只处理普通文件，并且扩展名为.txt
        if (S_ISREG(statbuf.st_mode)) {
            char *ext = strrchr(entry->d_name, '.');
            if (ext && strcmp(ext, ".txt") == 0) {
                // 添加到文件表
                fileTable[fileCount].id = fileCount;
                strncpy(fileTable[fileCount].filename, fullPath, MAX_FILENAME_LEN - 1);
                fileTable[fileCount].filename[MAX_FILENAME_LEN - 1] = '\0';
                fileCount++;
                count++;
            }
        }
    }

    closedir(dir);
    return count;
}
