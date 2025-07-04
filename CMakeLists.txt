cmake_minimum_required(VERSION 3.28)
project(PlutoEngine)

set(CMAKE_CXX_STANDARD 17)

include(FetchContent)

# GLFW
FetchContent_Declare(
    glfw
    GIT_REPOSITORY https://github.com/glfw/glfw.git
    GIT_TAG 3.4
)
FetchContent_MakeAvailable(glfw)

# Manually added GLAD files
add_library(glad STATIC
    external/glad/src/gl.c
)
target_include_directories(glad PUBLIC
    external/glad/include
)

add_library(stb INTERFACE)
target_include_directories(stb INTERFACE ${CMAKE_SOURCE_DIR}/external)

file(GLOB_RECURSE SRC_FILES
        src/*.cpp
        render/*.cpp
        input/*.cpp
        app/*.cpp
        scene/*.cpp
        PlutoMath/*.cpp
        util/*.cpp
)

# Your executable
add_executable(${PROJECT_NAME}
    ${SRC_FILES}
)

target_include_directories(${PROJECT_NAME} PRIVATE
    external/glad/include
)

# Link everything
target_link_libraries(${PROJECT_NAME}
    glad
    glfw
    stb
    ${CMAKE_DL_LIBS}
)

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${CMAKE_SOURCE_DIR}/shaders $<TARGET_FILE_DIR:${PROJECT_NAME}>/shaders
    COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${CMAKE_SOURCE_DIR}/res $<TARGET_FILE_DIR:${PROJECT_NAME}>/res)