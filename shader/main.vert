
#version 150 compatibility

// Inverse view matrix
uniform mat4 iV;

// Inverse projection matrix
uniform mat4 iP;

// Volume bounding box
uniform vec3 BoundsMin;
uniform vec3 BoundsMax;

// Primary ray
out vec3 RayPos;
out vec3 RayDir;

void main() {
	// Positions on near/far plane in clip space
	vec4 pCS0 = gl_Vertex - vec4(0.0, 0.0, 1.0, 0.0);
	vec4 pCS1 = gl_Vertex + vec4(0.0, 0.0, 1.0, 0.0);

	// Position on near/far plane in world space
	vec4 p0 = iV * iP * pCS0;
	vec4 p1 = iV * iP * pCS1;

	// Dehomogenize positions
	p0 /= p0.w;
	p1 /= p1.w;

	// Output ray
	RayPos = p0.xyz;
	RayDir = p1.xyz - p0.xyz;

	// Output the position in clip space
	gl_Position = gl_Vertex;
}
