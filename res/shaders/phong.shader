#shader vertex
#version 450 core

uniform mat4 u_model_matrix;
uniform mat4 u_view_matrix;
uniform mat4 u_projection_matrix;

layout(location = 0) in vec3 l_local_position;
layout(location = 1) in vec3 l_local_normal;

out vec4 v_world_position;
out vec3 v_world_normal;

void main(void) {
    gl_Position = (u_projection_matrix * u_view_matrix * u_model_matrix) * vec4(l_local_position, 1.0f);
    v_world_position = u_model_matrix * vec4(l_local_position, 1.0f);
    v_world_normal = mat3(transpose(inverse(u_model_matrix))) * l_local_normal;
}


#shader fragment
#version 450 core

uniform vec3 u_light_world_position;
uniform vec3 u_light_color;

uniform vec3 u_camera_world_position;

uniform float u_ambient_strength;
uniform float u_diffuse_strength;
uniform float u_specular_strength;

in vec4 v_world_position;
in vec3 v_world_normal;

out vec4 v_color;

void main(void) {
    vec3 object_color = vec3(0.0f, 0.0f, 1.0f);

    // ambient
    vec3 ambient = u_ambient_strength * u_light_color;

    // diffuse
    vec3 light_direction = normalize(u_light_world_position - v_world_position.xyz);
    float diffuse_strength = max(dot(v_world_normal, light_direction), 0.0f);
    vec3 diffuse = u_diffuse_strength * diffuse_strength * u_light_color;


    // specular
    vec3 view_direction = normalize(u_camera_world_position - v_world_position.xyz);
    vec3 reflect_direction = reflect(-light_direction, v_world_normal);
    float specular_strength = pow(max(dot(view_direction, reflect_direction), 0.0f), 32.0f);
    vec3 specular = u_specular_strength * specular_strength * u_light_color;

    vec3 result = (ambient + diffuse + specular) * object_color;

    v_color = vec4(result, 1.0f);
}
