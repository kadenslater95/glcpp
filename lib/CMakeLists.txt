
add_library(shader SHARED
    shader.cpp
)

target_include_directories(shader PUBLIC
    ${PROJECT_SOURCE_DIR}/include
)

target_link_libraries(shader PUBLIC
    GLEW::GLEW
)
