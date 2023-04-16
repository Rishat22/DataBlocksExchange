cmake_minimum_required(VERSION 3.5)

set(PROJECT_NAME DataBlocksExchange)
set(CLIENT_NAME client)
set(TEST_NAME test_${PROJECT_NAME})
set(PATHS include/ include/data/ include/network/)

if(DEFINED ENV{GITHUB_RUN_NUMBER})
    project(${PROJECT_NAME} VERSION 0.0.$ENV{GITHUB_RUN_NUMBER})
else()
    project(${PROJECT_NAME})
endif()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB HRDS
    include/*.h
    include/data/*.h
    include/network/*.h
    )
file(GLOB SRCS
    src/*.cpp
    src/data/*.cpp
    src/network/*.cpp
    )

add_executable( ${PROJECT_NAME}  ${HRDS} ${SRCS} main.cpp)

target_include_directories(${PROJECT_NAME} PUBLIC
    ${CMAKE_BINARY_DIR}
    ${CMAKE_CURRENT_LIST_DIR}/include
    )
target_include_directories(${PROJECT_NAME} PRIVATE ${PATHS})

target_link_libraries(${PROJECT_NAME} PUBLIC pthread)


file(GLOB TEST_SRCS
    tests/*.cpp
    )
find_package(Boost COMPONENTS unit_test_framework REQUIRED)

add_executable(${TEST_NAME}
    ${SRCS}
    ${TEST_SRCS}
    )

target_include_directories(${TEST_NAME} PUBLIC
    ${CMAKE_BINARY_DIR}
    ${CMAKE_CURRENT_LIST_DIR}/include
    )
target_include_directories(${TEST_NAME} PRIVATE
    ${PATHS}
    ${CMAKE_BINARY_DIR}
)

add_executable(${CLIENT_NAME} ${SRCS} client.cpp)
target_link_libraries(${CLIENT_NAME} PUBLIC pthread)
target_include_directories(${CLIENT_NAME} PUBLIC
    ${CMAKE_BINARY_DIR}
    ${CMAKE_CURRENT_LIST_DIR}/include
    )
target_include_directories(${CLIENT_NAME} PRIVATE
    ${PATHS}
    ${CMAKE_BINARY_DIR}
    )

target_include_directories(${TEST_NAME} PUBLIC
    ${CMAKE_BINARY_DIR}
    ${CMAKE_CURRENT_LIST_DIR}/include
    )
target_include_directories(${TEST_NAME} PRIVATE
    ${PATHS}
    ${CMAKE_BINARY_DIR}
    )
target_link_libraries(${TEST_NAME} PUBLIC ${Boost_LIBRARIES})
target_link_libraries(${TEST_NAME} PRIVATE ${CMAKE_THREAD_LIBS_INIT})
target_link_libraries(${TEST_NAME} PRIVATE pthread)


if (MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE
        /W4
        )
    target_compile_options(${TEST_NAME} PRIVATE
        /W4
        )
    target_compile_options(${CLIENT_NAME} PRIVATE
        /W4
        )
else ()
    target_compile_options(${PROJECT_NAME} PRIVATE
        -Wall -Wextra -pedantic -Werror
        )
    target_compile_options(${TEST_NAME} PRIVATE
        -Wall -Wextra -pedantic -Werror
        )
    target_compile_options(${TEST_NAME} PRIVATE
        -Wall -Wextra -pedantic -Werror
        )
endif()

install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION bin)
install(TARGETS ${TEST_NAME} RUNTIME DESTINATION bin)

set(CPACK_GENERATOR DEB)
set(CPACK_PACKAGE_CONTACT example@example.com)

include(CPack)
enable_testing()

add_test(${TEST_NAME} ${TEST_NAME})
