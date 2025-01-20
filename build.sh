#!/bin/bash

if [ $# -eq 0 ]; then
    echo "Ошибка: укажите имя файла для компиляции."
    echo "Пример использования: ./build.sh main.cc"
    exit 1
fi

SOURCE_FILE=$1

g++ -Wall -Werror -std=c++17 -Wpedantic -O2 -o app "$SOURCE_FILE"

