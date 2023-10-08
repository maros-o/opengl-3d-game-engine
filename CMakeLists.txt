cmake_minimum_required(VERSION 3.26)
project(cv2)

set(CMAKE_CXX_STANDARD 17)

# Find and configure GLFW
find_package(glfw3 REQUIRED)
include_directories(${GLFW_INCLUDE_DIRS})

# Find and configure GLEW
find_package(GLEW REQUIRED)
include_directories(${GLEW_INCLUDE_DIRS})

# Add GLM include directory
include_directories(libs)

# Add your executable
add_executable(cv2 src/main.cpp
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
        src/Model.cpp
        libs/stb/stb_image.h
        libs/stb/stb_image.cpp
        include/Texture.cpp
        include/Texture.h
)

# Add header files
target_include_directories(cv2 PUBLIC include)

# Link GLFW, GLEW, and OpenGL libraries
target_link_libraries(cv2 glfw GLEW::GLEW -lGL)
