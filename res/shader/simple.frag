layout(location=4) uniform sampler2D texMap;

in vec4 ex_color;
smooth in vec2 texCoordsInterpolated;

layout(location = 0) out vec4 color;

void main(){
	if(textureSize(texMap, 0).x > 1)
		color = ex_color * texture(texMap, texCoordsInterpolated);
	else
		color = ex_color;
}
