#shader vertex
#version 330

uniform mat4 uni_model_matrix;
uniform mat4 uni_view_matrix;
uniform mat4 uni_projection_matrix;
layout(location=0) in vec3 var_vertex_position;

void main() {
    gl_Position = uni_projection_matrix * uni_view_matrix * uni_model_matrix * vec4(var_vertex_position, 1.0);
};


#shader fragment
#version 330

out vec4 var_frag_color;

void main() {
    var_frag_color = vec4(0.0, 1.0, 0.0, 1.0);
};