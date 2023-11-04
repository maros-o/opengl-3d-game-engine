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
    v_world_normal = normalize(transpose(inverse(mat3(u_model_matrix))) * l_local_normal);
}


#shader fragment
#version 450 core

// light
struct Light {
    vec4 world_position;
    vec4 color;
    float constant_strength;
    float linear_strength;
    float quadratic_strength;
    float padding_1;
};

layout(std430, binding = 0) buffer LightsSSBO {
    Light ssbo_lights[];
};

// camera
uniform vec3 u_camera_world_position;

// object
uniform vec3 u_object_color;
uniform float u_ambient_strength;
uniform float u_diffuse_strength;
uniform float u_specular_strength;
uniform float u_shininess;

in vec4 v_world_position;
in vec3 v_world_normal;

out vec4 fragColor;

void main() {
    vec3 color_strength = vec3(0.0);

    for (int i = 0; i < ssbo_lights.length(); ++i) {
        vec3 light_world_position = vec3(ssbo_lights[i].world_position) / ssbo_lights[i].world_position.w;
        vec3 light_color = vec3(ssbo_lights[i].color);

        vec3 vertex_world_position = vec3(v_world_position) / v_world_position.w;

        vec3 light_direction = normalize(light_world_position - vertex_world_position);
        float diffuse_strength = max(dot(v_world_normal, light_direction), 0.0);

        if (diffuse_strength > 0.0) {
            vec3 ambient = u_ambient_strength * light_color;
            vec3 diffuse = u_diffuse_strength * diffuse_strength * light_color;

            vec3 view_direction = normalize(u_camera_world_position - vertex_world_position);
            vec3 halfway_direction = normalize(light_direction + view_direction);
            float specular_strength = pow(max(dot(v_world_normal, halfway_direction), 0.0), u_shininess);
            vec3 specular = u_specular_strength * specular_strength * light_color;

            float distance = length(light_world_position - vertex_world_position);
            float attenuation = 1.0 / (ssbo_lights[i].constant_strength + ssbo_lights[i].linear_strength * distance + ssbo_lights[i].quadratic_strength * (distance * distance));
            vec3 attenuated_light_color = light_color * attenuation;

            color_strength += (ambient + diffuse + specular) * attenuated_light_color;
        }
    }

    fragColor = vec4(color_strength * u_object_color, 1.0);
}
