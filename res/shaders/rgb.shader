#shader vertex
#version 330

uniform mat4 model_matrix;
layout(location=0) in vec3 vp;
layout(location=1) in vec3 vc;
out vec3 color;

void main() {
    gl_Position = model_matrix * vec4(vp, 1.0);
    color = vc;
};


#shader fragment
#version 330

in vec3 color;
out vec4 frag_color;

void main() {
    frag_color = vec4(color, 1.0);
};