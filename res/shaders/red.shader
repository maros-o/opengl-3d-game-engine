#shader vertex
#version 330

uniform mat4 model_matrix;
layout(location=0) in vec3 vp;

void main() {
    gl_Position = model_matrix * vec4(vp, 1.0);
};


#shader fragment
#version 330

out vec4 frag_color;

void main() {
    frag_color = vec4(1.0, 0.0, 0.0, 1.0);
};