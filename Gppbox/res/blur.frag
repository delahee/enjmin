#version 120
uniform sampler2D texture;

uniform int		samples;
uniform float	kernel[64 * 4];
uniform vec2	offsets[64 * 4];
uniform vec4	srcMul;

vec4 blurGauss(sampler2D image, vec2 uv){
	vec4 color = vec4(0.0);
	for (int i = 0; i < samples; i++) {
		vec4 c = texture2D(image, uv + offsets[i]);
		c*=srcMul;
		color += c * vec4(kernel[i]);
	}
	return color;
}

void main() {
    vec2 coord = gl_TexCoord[0].xy;

	vec4 pixel_color = blurGauss(texture, coord);

	gl_FragColor = pixel_color * gl_Color;
}
