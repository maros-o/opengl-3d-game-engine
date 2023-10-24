#shader vertex
#version 450 core

uniform mat4 uni_model_matrix;
uniform mat4 uni_view_matrix;
uniform mat4 uni_projection_matrix;

layout(location=0) in vec3 var_vertex_position;
layout(location=1) in vec2 var_texture_position;

out vec2 var_texture_coord;

void main() {
    gl_Position = uni_projection_matrix * uni_view_matrix * uni_model_matrix * vec4(var_vertex_position, 1.0);
    var_texture_coord = var_texture_position;
}


#shader fragment
#version 450 core

uniform sampler2D uni_texture_sampler;
in vec2 var_texture_coord;
out vec4 var_fragment_color;

void main() {
    var_fragment_color = texture(uni_texture_sampler, var_texture_coord);
}