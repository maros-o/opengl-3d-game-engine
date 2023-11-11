// #shader vertex
#version 460 core

uniform mat4 u_model_matrix;
uniform mat4 u_view_matrix;
uniform mat4 u_projection_matrix;

layout (location = 0) in vec3 l_local_position;

void main(void) {
    gl_Position = u_projection_matrix * u_view_matrix * u_model_matrix * vec4(l_local_position, 1.0f);
}


// #shader fragment
#version 460 core

uniform vec3 u_object_color;

out vec4 v_frag_color;

void main() {
    v_frag_color = vec4(u_object_color, 1.0);
}