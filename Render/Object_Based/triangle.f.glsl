varying vec2 f_texcoord;
uniform sampler2D texture;
varying float intensity;
varying vec4 v_color;

int te = 3;
 
void main(void) {
	vec2 flipped_texcoord = vec2(f_texcoord.x, 1.0 - f_texcoord.y);
	if(te == 0){
		gl_FragColor = texture2D(texture, flipped_texcoord);
	}
	else if( te == 1){
		gl_FragColor[0] = flipped_texcoord.x;
		gl_FragColor[1] = flipped_texcoord.y;
		gl_FragColor[2] = flipped_texcoord.x;
		gl_FragColor[3] = 1.0;
	}
	else{

		float color = 1.0f,
			  spec = 1.0f;
		if(intensity > 0.58)
			spec = 1.2;
		if (intensity > 0.75){
			color = 1.0;
		}
		else if (intensity > 0.5){
			color = 0.75;
		}
		else if (intensity > 0.25){
			color = 0.5;
		}
		else
			color = 0.25;
		
		vec4 f_col = v_color;
		gl_FragColor = f_col * /*texture2D(texture, flipped_texcoord) **/ vec4(intensity, intensity, intensity, 1.0)*spec;
	}
}
