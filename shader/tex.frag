#version 330 core

in vec2 texPos;

out vec4 fragColor;

uniform sampler2D tex;
uniform mat3 trans1;
uniform mat3 trans2;

uniform float width;
uniform float height;

void main()
{
	if (texPos.x > 0.5f)
	{
		vec3 pos = vec3(texPos.x, texPos.y, 0.0f);
		pos = trans1 * pos;
		fragColor = texture(tex, vec2(pos));
	}
	else
	{
		vec3 pos = vec3(texPos.x, texPos.y, 0.0f);
		pos = trans2 * pos;
		fragColor = texture(tex, vec2(pos));
	}
}