// #shader vertex
#version 460 core

uniform mat4 u_model_matrix;
uniform mat4 u_view_matrix;
uniform mat4 u_projection_matrix;

layout (location = 0) in vec3 l_local_position;
layout (location = 1) in vec3 l_local_normal;
layout (location = 2) in vec2 l_texture_position;

out vec4 v_world_position;
out vec3 v_world_normal;
out vec2 v_texture_position;

void main(void) {
    gl_Position = (u_projection_matrix * u_view_matrix * u_model_matrix) * vec4(l_local_position, 1.0f);

    v_world_position = u_model_matrix * vec4(l_local_position, 1.0f);
    v_world_normal = normalize(transpose(inverse(mat3(u_model_matrix))) * l_local_normal);
    v_texture_position = l_texture_position;
}


// #shader fragment
#version 460 core

// lights
struct DirectionalLight {
    vec4 direction;
    vec4 color;
    float strength;
    float padding_1;
    float padding_2;
    float padding_3;
};
struct PointLight {
    vec4 world_position;
    vec4 color;
    float constant_strength;
    float linear_strength;
    float quadratic_strength;
    float padding_1;
};
struct SpotLightSSBO {
    vec4 world_position;
    vec4 color;
    vec4 direction;
    float constant_strength;
    float linear_strength;
    float quadratic_strength;
    float cut_off;
};

layout (std430, binding = 0) buffer DirectionalLightsSSBO {
    DirectionalLight directional_lights[];
};
layout (std430, binding = 1) buffer PointLightsSSBO {
    PointLight point_lights[];
};
layout (std430, binding = 2) buffer SpotLightsSSBO {
    SpotLightSSBO spot_lights[];
};

// camera
uniform vec3 u_camera_world_position;

// object
uniform vec3 u_object_color;
uniform float u_ambient_strength;
uniform float u_diffuse_strength;
uniform float u_specular_strength;
uniform float u_shininess;

// texture
uniform sampler2D u_texture_sampler;
uniform float u_texture_repeat;

in vec4 v_world_position;
in vec3 v_world_normal;
in vec2 v_texture_position;

out vec4 fragColor;

vec3 dehomogenize(vec4 v) {
    return vec3(v) / v.w;
}

float calc_diffuse_strength(vec3 world_normal, vec3 light_direction) {
    return max(dot(world_normal, light_direction), 0.0);
}

float calc_specular_strength(vec3 world_normal, vec4 world_position, vec3 light_direction, vec3 camera_world_position, float shininess) {
    vec3 view_direction = normalize(camera_world_position - vec3(world_position));
    vec3 halfway_direction = normalize(light_direction + view_direction);
    return pow(max(dot(world_normal, halfway_direction), 0.0), shininess);
}

void main() {
    vec3 color_strength = vec3(0.0);

    // directional lights
    for (int i = 0; i < directional_lights.length(); ++i) {
        vec3 light_direction = -normalize(vec3(directional_lights[i].direction));
        float diffuse_strength = calc_diffuse_strength(v_world_normal, light_direction);

        if (diffuse_strength > 0.0) {
            vec3 ambient = u_ambient_strength * vec3(directional_lights[i].color);
            vec3 diffuse = u_diffuse_strength * diffuse_strength * vec3(directional_lights[i].color);

            vec3 specular = vec3(0.0);
            float specular_strength = calc_specular_strength(v_world_normal, v_world_position, light_direction, u_camera_world_position, u_shininess);
            if (specular_strength > 0.0) {
                specular = u_specular_strength * specular_strength * vec3(directional_lights[i].color);
            }

            color_strength += (ambient + diffuse + specular) * directional_lights[i].strength;
        }
    }

    // point lights
    for (int i = 0; i < point_lights.length(); ++i) {
        vec3 light_world_position = dehomogenize(point_lights[i].world_position);
        vec3 light_color = vec3(point_lights[i].color);

        vec3 vertex_world_position = dehomogenize(v_world_position);

        vec3 light_direction = normalize(light_world_position - vertex_world_position);
        float diffuse_strength = calc_diffuse_strength(v_world_normal, light_direction);

        if (diffuse_strength > 0.0) {
            vec3 ambient = u_ambient_strength * light_color;
            vec3 diffuse = u_diffuse_strength * diffuse_strength * light_color;

            float specular_strength = calc_specular_strength(v_world_normal, v_world_position, light_direction, u_camera_world_position, u_shininess);
            vec3 specular = u_specular_strength * specular_strength * light_color;

            float distance = length(light_world_position - vertex_world_position);
            float attenuation = 1.0 / (point_lights[i].constant_strength + point_lights[i].linear_strength * distance + point_lights[i].quadratic_strength * (distance * distance));
            vec3 attenuated_light_color = light_color * attenuation;

            color_strength += (ambient + diffuse + specular) * attenuated_light_color;
        }
    }

    // spot lights
    for (int i = 0; i < spot_lights.length(); ++i) {
        vec3 light_world_position = dehomogenize(spot_lights[i].world_position);
        vec3 light_color = vec3(spot_lights[i].color);

        vec3 vertex_world_position = dehomogenize(v_world_position);

        vec3 light_direction = normalize(light_world_position - vertex_world_position);
        float diffuse_strength = calc_diffuse_strength(v_world_normal, light_direction);

        if (diffuse_strength > 0.0) {
            vec3 ambient = u_ambient_strength * light_color;
            vec3 diffuse = u_diffuse_strength * diffuse_strength * light_color;

            float specular_strength = calc_specular_strength(v_world_normal, v_world_position, light_direction, u_camera_world_position, u_shininess);
            vec3 specular = u_specular_strength * specular_strength * light_color;

            float distance = length(light_world_position - vertex_world_position);
            float attenuation = 1.0 / (spot_lights[i].constant_strength + spot_lights[i].linear_strength * distance + spot_lights[i].quadratic_strength * (distance * distance));
            vec3 attenuated_light_color = light_color * attenuation;

            float theta = dot(light_direction, normalize(vec3(spot_lights[i].direction)));
            float epsilon = spot_lights[i].cut_off;
            float intensity = clamp((theta - epsilon) / (1.0 - epsilon), 0.0, 1.0);

            color_strength += (ambient + diffuse + specular) * attenuated_light_color * intensity;
        }
    }

    vec2 repeated_texture_pos = v_texture_position * u_texture_repeat;
    fragColor = vec4(color_strength * u_object_color, 1.0f) * texture(u_texture_sampler, repeated_texture_pos);
}
