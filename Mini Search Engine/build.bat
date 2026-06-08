@echo off
chcp 65001 >nul
echo Building Mini Search Engine...

if not exist obj mkdir obj
if not exist bin mkdir bin

gcc -Wall -Wextra -std=c99 -I./include -c src/main.c -o obj/main.o
gcc -Wall -Wextra -std=c99 -I./include -c src/file_scanner.c -o obj/file_scanner.o
gcc -Wall -Wextra -std=c99 -I./include -c src/tokenizer.c -o obj/tokenizer.o
gcc -Wall -Wextra -std=c99 -I./include -c src/index.c -o obj/index.o
gcc -Wall -Wextra -std=c99 -I./include -c src/search.c -o obj/search.o
gcc -Wall -Wextra -std=c99 -I./include -c src/interactive.c -o obj/interactive.o

gcc obj/main.o obj/file_scanner.o obj/tokenizer.o obj/index.o obj/search.o obj/interactive.o -o bin/search_engine.exe

echo Build complete!
echo.
echo Run with: bin\search_engine.exe data
pause
