varying float intensity;
varying vec4 v_color;

 
void main(void) {
	float color = 1.0f,
		  spec = 1.0f;
	/*if(intensity > 0.58)
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
		color = 0.25;*/
	if(intensity < 0.25)
		spec = 0;
	vec4 f_col = v_color;
	gl_FragColor = f_col * vec4(intensity*spec, intensity*spec, intensity*spec, 1.0);
}
