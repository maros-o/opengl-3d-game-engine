#shader vertex
#version 330

uniform mat4 u_model_matrix;
uniform mat4 u_view_matrix;
uniform mat4 u_projection_matrix;
uniform mat3 u_normal_matrix;

layout(location=0) in vec3 i_local_position;
layout(location=1) in vec3 i_local_normal;

out vec3 v_world_position;
out vec3 v_world_normal;

void main(void){
    gl_Position = (u_projection_matrix * u_view_matrix * u_model_matrix) * vec4(i_local_position, 1.0f);
    v_world_position = u_model_matrix * vec4(i_local_position, 1.0f);
    v_world_normal = u_normal_matrix * i_local_normal;
}


#shader fragment
#version 330

uniform vec3 u_light_world_position;
uniform vec3 u_light_color;

in vec3 v_world_position;
in vec3 v_world_normal;

out vec4 v_color;

void main(void){
    vec3 light_direction = normalize(u_light_world_position - v_world_position);
    float diffuse = max(dot(v_world_normal, light_direction), 0.0f);
    v_color = vec4(u_light_color * diffuse, 1.0f);
}