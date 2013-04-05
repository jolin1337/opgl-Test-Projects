attribute vec3 coord3d;
attribute vec3 v_normal;
uniform mat3 m_3x3_inv_transp;
varying vec4 color;

uniform mat4 m_transform;
struct lightSource {
	vec4 position;
	vec4 diffuse;
	float constantAttenuation, linearAttenuation, quadraticAttenuation;
	float spotCutoff, spotExponent;
	vec3 spotDirection;
};
lightSource light0 = lightSource(
	vec4(0.0, 1.0, 2.0, 1.0),
	vec4(1.0, 1.0, 1.0, 1.0),
	0.0, 1.0, 0.0,
	100.0, 20.0,
	vec3(-1.0, -0.5, -1.0)
);
 
struct material {
	vec4 diffuse;
};
material mymaterial = material(vec4(1.0, 0.8, 0.8, 1.0));
 
 
void main(void) {
	vec3 normalDirection = normalize(m_3x3_inv_transp * v_normal);
	vec3 lightDirection;
	float attenuation;

	if (light0.position.w == 0.0) {// directional light 
		attenuation = 1.0; // no attenuation
		lightDirection = normalize(vec3(light0.position));
	}
	else { // point or spot light (or other kind of light)
		vec3 vertexToLightSource = vec3(light0.position - m_transform * vec4(coord3d,1.0));
		float distance = length(vertexToLightSource);
		lightDirection = normalize(vertexToLightSource);
		attenuation = 1.0 / (light0.constantAttenuation	+ light0.linearAttenuation * distance
								+ light0.quadraticAttenuation * distance * distance);

		if (light0.spotCutoff <= 90.0) {// spotlight
			float clampedCosine = max(0.0, dot(-lightDirection, normalize(light0.spotDirection)));
			if (clampedCosine < cos(light0.spotCutoff * 3.14159 / 180.0)) {// outside of spotlight cone
				attenuation = 0.0;
			}
			else {
				attenuation = attenuation * pow(clampedCosine, light0.spotExponent);
			}
		}
	}
	vec3 diffuseReflection = attenuation * vec3(light0.diffuse) * vec3(mymaterial.diffuse) * 
								max(0.0, dot(normalDirection, lightDirection));

	color = vec4(diffuseReflection, 1.0);
	gl_Position = m_transform * vec4(coord3d,1.0);
}