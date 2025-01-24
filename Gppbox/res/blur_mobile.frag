//#version 110
precision mediump float;
uniform sampler2D texture;
uniform vec2	uv;

uniform int		samples;//not fast path
uniform float	kernel[64 * 4];
uniform vec2 	offsets[64 * 4];
uniform vec4	srcMul;
uniform vec4	glColor;

void main() {
    vec2 coord = uv;
	vec4 color= vec4(0);
	for (int i = 0; i < samples; i++) {//true conditionnal NFP
		vec4 c = texture2D(texture, uv + offsets[i]);// offsets sur les uv NFP
		color += smoothstep(kernel[i],0.2,0.6) * c  * srcMul;
		//color += kernel[i] * c  * srcMul;
	}
	gl_FragColor = color * glColor;
}
