attribute vec3 coord3d;
attribute vec2 texcoord;
varying vec2 f_texcoord;

//varying vec4 v_color;

uniform mat4 m_transform;

 
 
void main(void) {
	gl_Position = m_transform * vec4(coord3d, 1.0);
	f_texcoord = texcoord;
}