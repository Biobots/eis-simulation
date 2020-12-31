#version 330 core

in vec2 texPos;

out vec4 fragColor;

uniform sampler2D tex;
uniform mat3 trans0, trans1, trans2, trans3, trans4, trans5, trans6, trans7, trans8, trans9;

uniform float width;
uniform float height;

void main()
{
	vec3 pos = vec3(texPos.x, texPos.y, 0.0f);
	if (texPos.x > 0.9f) pos = trans0 * pos;
	else if (texPos.x > 0.8f) pos = trans1 * pos;
	else if (texPos.x > 0.7f) pos = trans2 * pos;
	else if (texPos.x > 0.6f) pos = trans3 * pos;
	else if (texPos.x > 0.5f) pos = trans4 * pos;
	else if (texPos.x > 0.4f) pos = trans5 * pos;
	else if (texPos.x > 0.3f) pos = trans6 * pos;
	else if (texPos.x > 0.2f) pos = trans7 * pos;
	else if (texPos.x > 0.1f) pos = trans8 * pos;
	else pos = trans9 * pos;
	fragColor = texture(tex, vec2(pos));
}