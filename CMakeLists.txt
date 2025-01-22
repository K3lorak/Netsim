cmake_minimum_required(VERSION 3.13)
project(netsim)

# Ustaw standard C++
set(CMAKE_CXX_STANDARD 17)

# Dodaj katalog zawierający nagłówki
include_directories(
        include
)

# Pliki źródłowe aplikacji
set(SOURCE_FILES
        src/factory.cpp
        src/package.cpp
        src/storage_types.cpp
        src/helpers.cpp
        src/nodes.cpp
)

# Pliki testowe (tylko do celu testowego)
set(SOURCE_FILES_TESTS
        test/tests.cpp
)

# === Cel dla aplikacji ===
add_executable(tsp ${SOURCE_FILES} main.cpp)

# === Konfiguracja dla testów ===
if(EXISTS ${PROJECT_SOURCE_DIR}/test)
    # Nazwa celu testowego
    set(EXEC_TEST ${PROJECT_NAME}__test)

    # Dodaj cel testowy
    add_executable(${EXEC_TEST}
            ${SOURCE_FILES}         # Pliki aplikacji potrzebne w testach
            ${SOURCE_FILES_TESTS}   # Pliki testowe
            test/main_gtest.cpp     # Plik główny testów
    )

    # Ścieżka do Google Test
    set(GTEST_ROOT ${PROJECT_SOURCE_DIR}/../googletest-master)#tutaj moze byc konflikt bo nie wiem czy tak moze byc, jesli nie
    #to dajcie folder do folderu roboczego i najwyzej dodamy go do gitignore czy cus 

    # Dodaj nagłówki Google Test
    target_include_directories(${EXEC_TEST} PUBLIC
            ${GTEST_ROOT}/googlemock/include
            ${GTEST_ROOT}/googletest/include
    )

    # Dodaj Google Test jako podprojekt
    add_subdirectory(${GTEST_ROOT} googletest-master)

    # Połącz bibliotekę Google Mock z celem testowym
    target_link_libraries(${EXEC_TEST} gmock)
endif()