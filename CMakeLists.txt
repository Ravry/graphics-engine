cmake_minimum_required(VERSION 3.31)
project(graphics-engine)

set(CMAKE_CXX_STANDARD 17)

set(GLFW_DIR "${CMAKE_CURRENT_SOURCE_DIR}/libs/glfw")
set(GLAD_DIR "${CMAKE_CURRENT_SOURCE_DIR}/libs/glad")
set(GLM_DIR "${CMAKE_CURRENT_SOURCE_DIR}/libs/glm")
set(STB_DIR "${CMAKE_CURRENT_SOURCE_DIR}/libs/stb")
set(ASSIMP_DIR "${CMAKE_CURRENT_SOURCE_DIR}/libs/assimp")
set(FREETYPE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/libs/freetype")

find_package(OpenGL REQUIRED)

add_library(glad STATIC ${GLAD_DIR}/src/glad.c)
target_include_directories(glad PUBLIC ${GLAD_DIR}/include)

file(GLOB_RECURSE SRC_FILES "src/*.cpp")
add_executable(${PROJECT_NAME} ${SRC_FILES})

target_include_directories(${PROJECT_NAME} PRIVATE
    includes 
    ${GLFW_DIR}/include
    ${GLAD_DIR}/include
    ${GLM_DIR}
    ${STB_DIR}
    ${ASSIMP_DIR}/include
    ${FREETYPE_DIR}/include/freetype2/
)

target_link_directories(${PROJECT_NAME} PRIVATE 
    ${GLFW_DIR}/lib-mingw-w64
    ${FREETYPE_DIR}/lib
    ${ASSIMP_DIR}/lib
)

target_link_libraries(${PROJECT_NAME} PRIVATE
    glad
    glfw3
    OpenGL::GL
    freetype
    assimp
)

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    "${GLFW_DIR}/lib-mingw-w64/glfw3.dll"
    "${ASSIMP_DIR}/bin/libassimp-5.dll"
    $<TARGET_FILE_DIR:${PROJECT_NAME}>
)