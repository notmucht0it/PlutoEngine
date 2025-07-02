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

# Your executable
add_executable(${PROJECT_NAME}
    src/main.cpp
)

target_include_directories(${PROJECT_NAME} PRIVATE
    external/glad/include
)

# Link everything
target_link_libraries(${PROJECT_NAME}
    glad
    glfw
    ${CMAKE_DL_LIBS}
)