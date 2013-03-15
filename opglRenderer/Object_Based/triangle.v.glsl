attribute vec3 position;

attribute vec2 texcoord;
varying vec2 f_texcoord;

attribute vec4 color;
varying vec4 v_color;

attribute vec3 normal;

uniform mat4 m_transform;


vec4 lightDir = vec4(0.0,0.0,-0.7, 0.0);
varying float intensity; 
 
void main(void) {
	vec4 coord = vec4(position, 1.0);
	gl_Position = m_transform * coord;
	
	intensity = dot(lightDir, (m_transform * vec4(normal, 0.0)));
	
	f_texcoord = texcoord;
	v_color = color;
}
