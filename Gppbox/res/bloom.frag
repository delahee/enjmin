#version 120
uniform sampler2D	texture;
uniform float		bloomPass;
uniform vec4		bloomMul;

vec4 bloom(vec4 color){
	float 	alpha = color.a;
		
	vec3	lumVector = vec3(0.299, 0.587, 0.114);
	float	luminance = dot(lumVector,color.rgb);
		
	luminance = max(0.001, luminance - bloomPass);
		
	color.rgb *= sign(luminance);
	color *= bloomMul;
	return color;
}

void main() {
    vec2 coord = gl_TexCoord[0].xy;
	vec4 pixel_color = bloom( texture2D(texture, coord) * gl_Color );
	gl_FragColor = pixel_color;
}
