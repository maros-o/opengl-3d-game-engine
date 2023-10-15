cmake_minimum_required(VERSION 3.26)
project(proj)

set(CMAKE_CXX_STANDARD 17)

find_package(glfw3 REQUIRED)
include_directories(${GLFW_INCLUDE_DIRS})

find_package(GLEW REQUIRED)
include_directories(${GLEW_INCLUDE_DIRS})

find_package(assimp REQUIRED)
include_directories(${ASSIMP_INCLUDE_DIRS})

include_directories(libs)

add_executable(proj src/main.cpp
        include/OpenGLContext.h
        include/ShaderProgram.h
        include/VAO.h
        src/OpenGLContext.cpp
        src/ShaderProgram.cpp
        src/VAO.cpp
        src/Renderer.cpp
        include/RenderObject.h
        include/VBO.h
        src/VBO.cpp
        include/Renderer.h
        src/RenderObject.cpp
        include/EBO.h
        src/EBO.cpp
        include/Model.h
        include/InputManager.h
        include/Engine.h
        src/Engine.cpp
        res/vertices.h
        libs/stb/stb_image.h
        libs/stb/stb_image.cpp
        src/Texture.cpp
        include/Texture.h
        res/indices.h
        src/InputManager.cpp
        include/Transformable.h
        include/TransformableComposite.h
        src/TransformableComposite.cpp
        include/Camera.h
        src/Camera.cpp
        include/CameraObserver.h
        include/Mesh.h
        res/3d_models/plain.h
        res/3d_models/sphere.h
        res/3d_models/suzi_flat.h
        res/3d_models/suzi_smooth.h
)

target_include_directories(proj PUBLIC include)

target_link_libraries(proj glfw GLEW::GLEW -lGL ${ASSIMP_LIBRARIES})