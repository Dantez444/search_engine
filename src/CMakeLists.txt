project(search_engine)

# Добавление библиотеки nlohmann/json
include(FetchContent)
FetchContent_Declare(
    json
    URL https://github.com/nlohmann/json/releases/download/v3.11.2/json.tar.xz
)
FetchContent_MakeAvailable(json)

# Добавление исходных файлов
set(SOURCES
    main.cpp
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

# Создание исполняемого файла
add_executable(search_engine ${SOURCES} ${HEADERS})

# Указание директорий для поиска заголовочных файлов
target_include_directories(search_engine PRIVATE ${INCLUDE_DIR})

# Подключение библиотек
target_link_libraries(search_engine PRIVATE nlohmann_json::nlohmann_json) 