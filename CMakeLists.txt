cmake_minimum_required(VERSION 3.18)
project(OpenGLGraphicsPortfolio LANGUAGES CXX)

find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)

add_executable(birthday-card projects/birthday-card/main.cpp)
add_executable(campus-scene projects/campus-scene/main.cpp)

foreach(scene IN ITEMS birthday-card campus-scene)
    target_compile_features(${scene} PRIVATE cxx_std_11)
    target_link_libraries(${scene} PRIVATE OpenGL::GL OpenGL::GLU GLUT::GLUT)
endforeach()

add_custom_command(TARGET campus-scene POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_CURRENT_SOURCE_DIR}/projects/campus-scene/texture"
        "$<TARGET_FILE_DIR:campus-scene>/texture"
    VERBATIM)
