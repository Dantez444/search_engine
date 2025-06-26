project(search_engine_tests)

# Добавление библиотеки Google Test
include(FetchContent)
FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/609281088cfefc76f9d0ce82e1ff6c30cc3591e5.zip
)

# Настройки для корректной работы Google Test с MinGW
set(gtest_disable_pthreads on)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(googletest)

# Включить возможность тестирования
enable_testing()

# Добавление исходных файлов (без main.cpp)
set(SOURCES
    tests.cpp
    ConverterJSON.cpp
    InvertedIndex.cpp
    SearchServer.cpp
)

# Добавление заголовочных файлов
set(HEADERS
    ${INCLUDE_DIR}/ConverterJSON.h
    ${INCLUDE_DIR}/InvertedIndex.h
    ${INCLUDE_DIR}/SearchServer.h
)

# Создание исполняемого файла для тестов
add_executable(search_engine_tests ${SOURCES} ${HEADERS})

# Указание директорий для поиска заголовочных файлов
target_include_directories(search_engine_tests PRIVATE ${INCLUDE_DIR} ${gtest_SOURCE_DIR}/include)

# Подключение библиотек
target_link_libraries(search_engine_tests PRIVATE 
    nlohmann_json::nlohmann_json 
    gtest_main
)

# Настроить обнаружение тестов
include(GoogleTest)
gtest_discover_tests(search_engine_tests) 