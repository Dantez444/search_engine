cmake_minimum_required(VERSION 3.31)
project(search_engine)

set(CMAKE_CXX_STANDARD 20)

# Добавление библиотеки nlohmann/json
include(FetchContent)
FetchContent_Declare(
    json
    URL https://github.com/nlohmann/json/releases/download/v3.11.2/json.tar.xz
)

# Добавление библиотеки Google Test
FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/609281088cfefc76f9d0ce82e1ff6c30cc3591e5.zip
)

# Настройки для корректной работы Google Test с MinGW
set(gtest_disable_pthreads on)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

# Сделать все настройки доступными для проекта
FetchContent_MakeAvailable(json googletest)

# Включить возможность тестирования
enable_testing()

# Добавить исходные файлы в библиотеку
include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

# Добавить исполняемый файл
add_executable(search_engine 
    main.cpp 
    ConverterJSON.cpp 
    ConverterJSON.h 
    InvertedIndex.cpp 
    InvertedIndex.h 
    SearchServer.cpp
    SearchServer.h
    tests.cpp
)

# Подключить библиотеки
target_link_libraries(search_engine PRIVATE nlohmann_json::nlohmann_json gtest_main)

# Настроить обнаружение тестов
include(GoogleTest)
gtest_discover_tests(search_engine)
