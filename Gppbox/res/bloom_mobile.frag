//#version 120
precision mediump float;
uniform sampler2D	texture;
uniform float		bloomPass;
uniform vec4		bloomMul;
uniform vec4		glColor;
uniform vec2		glTexCoord;

vec4 bloom(vec4 color){
	vec3	lumVector = vec3(0.299, 0.587, 0.114);
	float	luminance = dot(lumVector,color.rgb);
		
	luminance = max(0.001, luminance - bloomPass);
		
	color.rgb *= sign(luminance);//try removing .rgb you lose 1 cycle because alpha channel processing is separate !
	color *= bloomMul;
	return color;
}

// rgb & alpha separé
// MADD ADDM => 1 instruction mul et un add ( et vice versa )

// (c0 * c1 + c ) => 1 instr
// (c0 * c1) + c => 2 instr 
// poid * mat + ... 
// 



void main() {
    vec2 coord = glTexCoord.xy;
	vec4 pixel_color = bloom( texture2D(texture, coord) * glColor );//4
	gl_FragColor = pixel_color;//2
}
