varying vec2 f_texcoord;
uniform sampler2D texture;

bool te = true;
 
void main(void) {
	vec2 flipped_texcoord = vec2(f_texcoord.x, 1.0 - f_texcoord.y);
	if(te){
		gl_FragColor = texture2D(texture, flipped_texcoord);
	}
	else{
		gl_FragColor[0] = flipped_texcoord.x;
		gl_FragColor[1] = flipped_texcoord.y;
		gl_FragColor[2] = flipped_texcoord.x;
		gl_FragColor[3] = 1.0;
	}
}
