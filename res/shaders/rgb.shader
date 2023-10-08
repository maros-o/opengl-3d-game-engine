#shader vertex
#version 330

uniform mat4 uni_MVP_matrix;
layout(location=0) in vec3 var_vertex_position;
layout(location=1) in vec3 var_fragment_color;
out vec3 var_color;

void main() {
    gl_Position = uni_MVP_matrix * vec4(var_vertex_position, 1.0);
    var_color = var_fragment_color;
};


#shader fragment
#version 330

in vec3 var_color;
out vec4 var_fragment_color;

void main() {
    var_fragment_color = vec4(var_color, 1.0);
};