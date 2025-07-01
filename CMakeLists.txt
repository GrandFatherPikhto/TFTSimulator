cmake_minimum_required(VERSION 3.10)
project(LCD)

# Тестирование надо включать на самом верхнем уровне, иначе работать не будет
include(CTest)
enable_testing()

# Добавляем подпроекты
add_subdirectory(TFTSimulator)


