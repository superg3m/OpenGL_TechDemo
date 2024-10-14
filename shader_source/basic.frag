#version 330 core
out vec4 FragColor;

in vec3 ourColor;
in vec2 TexCoord;

// texture samplers
uniform sampler2D textures[2];

void main() {
	// linearly interpolate between both textures (80% container, 20% awesomeface)
	FragColor = vec4(ourColor, 1.0) * mix(texture(textures[0], TexCoord), texture(textures[1], TexCoord), 0.2);
}

