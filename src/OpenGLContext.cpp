#include <map>
#include "OpenGLContext.h"
#include <iostream>

OpenGLContext OpenGLContext::instance;

std::map<GLenum, std::string> severity_map = {
        {GL_DEBUG_SEVERITY_HIGH,         "SEVERITY_HIGH"},
        {GL_DEBUG_SEVERITY_MEDIUM,       "SEVERITY_MEDIUM"},
        {GL_DEBUG_SEVERITY_LOW,          "SEVERITY_LOW"},
        {GL_DEBUG_SEVERITY_NOTIFICATION, "NOTIFICATION"},
};

void message_callback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar *message,
                      const void *user_param) {
    const char *source_str = "UNKNOWN";
    const char *type_str = "UNKNOWN";
    std::string severity_str = "UNKNOWN";

    if (source == GL_DEBUG_SOURCE_API) {
        source_str = "API";
    } else if (source == GL_DEBUG_SOURCE_WINDOW_SYSTEM) {
        source_str = "WINDOW_SYSTEM";
    } else if (source == GL_DEBUG_SOURCE_SHADER_COMPILER) {
        source_str = "SHADER_COMPILER";
    } else if (source == GL_DEBUG_SOURCE_THIRD_PARTY) {
        source_str = "THIRD_PARTY";
    } else if (source == GL_DEBUG_SOURCE_APPLICATION) {
        source_str = "APPLICATION";
    } else if (source == GL_DEBUG_SOURCE_OTHER) {
        source_str = "OTHER";
    }

    if (type == GL_DEBUG_TYPE_ERROR) {
        type_str = "ERROR";
    } else if (type == GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR) {
        type_str = "DEPRECATED BEHAVIOR";
    } else if (type == GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR) {
        type_str = "UNDEFINED BEHAVIOR";
    } else if (type == GL_DEBUG_TYPE_PORTABILITY) {
        type_str = "PORTABILITY";
    } else if (type == GL_DEBUG_TYPE_PERFORMANCE) {
        type_str = "PERFORMANCE";
    } else if (type == GL_DEBUG_TYPE_MARKER) {
        type_str = "MARKER";
    } else if (type == GL_DEBUG_TYPE_PUSH_GROUP) {
        type_str = "PUSH GROUP";
    } else if (type == GL_DEBUG_TYPE_POP_GROUP) {
        type_str = "POP GROUP";
    } else if (type == GL_DEBUG_TYPE_OTHER) {
        type_str = "OTHER";
    }

    if (severity_map.find(severity) != severity_map.end()) {
        severity_str = severity_map[severity];
    }
    if (severity == GL_DEBUG_SEVERITY_NOTIFICATION) {
        std::cout << "\x1b[90;1m" << type_str << "::" << source_str << "::" << severity_str << ": " << message
                  << "\x1b[0m" << std::endl;
    } else {
        std::cerr << type_str << "::OPENGL::" << source_str << "::" << severity_str << ": " << message << std::endl;
    }
}

OpenGLContext::~OpenGLContext() {
    glfwDestroyWindow(this->window);
    glfwTerminate();
}

bool OpenGLContext::should_close() {
    return glfwWindowShouldClose(this->window);
}

void OpenGLContext::swap_buffers() {
    glfwSwapBuffers(this->window);
}

void OpenGLContext::print_version_info() {
    printf("OpenGL Version: %s\n", glGetString(GL_VERSION));
    printf("Using GLEW %s\n", glewGetString(GLEW_VERSION));
    printf("Vendor %s\n", glGetString(GL_VENDOR));
    printf("Renderer %s\n", glGetString(GL_RENDERER));
    printf("GLSL %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));
    int major, minor, revision;
    glfwGetVersion(&major, &minor, &revision);
    printf("Using GLFW %i.%i.%i\n", major, minor, revision);
}

int OpenGLContext::get_screen_width() const {
    int width;
    glfwGetWindowSize(this->window, &width, nullptr);
    return width;
}

int OpenGLContext::get_screen_height() const {
    int height;
    glfwGetWindowSize(this->window, nullptr, &height);
    return height;
}

GLFWwindow *OpenGLContext::get_window() const {
    return this->window;
}

void OpenGLContext::close() {
    printf("Closing window\n");
    glfwSetWindowShouldClose(this->window, GLFW_TRUE);
}

void OpenGLContext::set_viewport(unsigned short screen_width, unsigned short screen_height) {
    glViewport(0, 0, screen_width, screen_height);
}

OpenGLContext &OpenGLContext::get_instance() {
    return OpenGLContext::instance;
}

OpenGLContext &OpenGLContext::init(int screen_width, int screen_height, const char *title) {
    if (!glfwInit()) {
        fprintf(stderr, "ERROR::GLFW3::INIT\n");
        exit(EXIT_FAILURE);
    }

    this->window = glfwCreateWindow(screen_width, screen_height, title, nullptr, nullptr);
    if (!this->window) {
        glfwTerminate();
        fprintf(stderr, "ERROR::GLFW3::WINDOW\n");
        exit(EXIT_FAILURE);
    }

    glfwMakeContextCurrent(this->window);
    glfwSwapInterval(1);

    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        fprintf(stderr, "ERROR::GLEW::INIT\n");
        exit(EXIT_FAILURE);
    }

    glfwGetFramebufferSize(this->window, &screen_width, &screen_height);
    glViewport(0, 0, screen_width, screen_height);

    glDebugMessageCallback(message_callback, nullptr);
    glEnable(GL_DEBUG_OUTPUT);

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);

    glDepthFunc(GL_LESS);
    glEnable(GL_DEPTH_TEST);

    return *this;
}

