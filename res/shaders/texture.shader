#shader vertex
#version 330

uniform mat4 model_matrix;
layout(location=0) in vec3 vertex_position;
layout(location=1) in vec2 texture_position;

out vec2 v_texture_coord;

void main() {
    gl_Position = model_matrix * vec4(vertex_position, 1.0);
    v_texture_coord = texture_position;
};


#shader fragment
#version 330

uniform sampler2D texture_sampler;
in vec2 v_texture_coord;
out vec4 frag_color;

void main() {
    frag_color = texture(texture_sampler, v_texture_coord);
};