#include "OpenGLContext.h"

static void GLAPIENTRY
message_callback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar *message,
                 const void *user_param) {
    const char *source_str = "UNKNOWN";
    const char *type_str = "UNKNOWN";

    if (source == GL_DEBUG_SOURCE_API) {
        source_str = "API";
    } else if (source == GL_DEBUG_SOURCE_WINDOW_SYSTEM) {
        source_str = "WINDOW SYSTEM";
    } else if (source == GL_DEBUG_SOURCE_SHADER_COMPILER) {
        source_str = "SHADER COMPILER";
    } else if (source == GL_DEBUG_SOURCE_THIRD_PARTY) {
        source_str = "THIRD PARTY";
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

    fprintf(stderr, "%s::OPENGL::%s::0x%X: %s\n", type_str, source_str,
            severity,
            message);

    exit(1);
}

OpenGLContext::OpenGLContext(int width, int height, const char *title) {
    if (!glfwInit()) {
        fprintf(stderr, "ERROR: could not start GLFW3\n");
        exit(EXIT_FAILURE);
    }

    this->window = glfwCreateWindow(width, height, title, nullptr, nullptr);
    if (!this->window) {
        glfwTerminate();
        fprintf(stderr, "ERROR: could not open window with GLFW3\n");
        exit(EXIT_FAILURE);
    }

    glfwMakeContextCurrent(this->window);
    glfwSwapInterval(1);

    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        fprintf(stderr, "ERROR: could not start GLEW\n");
        exit(EXIT_FAILURE);
    }

    glfwGetFramebufferSize(this->window, &width, &height);
    glViewport(0, 0, width, height);

    glDebugMessageCallback(message_callback, nullptr);
    glEnable(GL_DEBUG_OUTPUT);

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);
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

void OpenGLContext::poll_events() {
    glfwPollEvents();
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

int OpenGLContext::get_width() const {
    int width;
    glfwGetWindowSize(this->window, &width, nullptr);
    return width;
}

int OpenGLContext::get_height() const {
    int height;
    glfwGetWindowSize(this->window, nullptr, &height);
    return height;
}
