
#version 150 compatibility

/******************************************************************************/

// External function declarations, cf. Util.glsl

float map(float x, float x0, float x1, float y0, float y1);
vec3 map(vec3 x, vec3 x0, vec3 x1, vec3 y0, vec3 y1);

bool IntersectRayBox(vec3 rayPos, vec3 rayDir, vec3 boxMin, vec3 boxMax, out float t0, out float t1);

/******************************************************************************/

// Volume bounding box
uniform vec3 BoundsMin;
uniform vec3 BoundsMax;

// Volume density
uniform sampler3D DensityTexture;
uniform float     DensityMin;
uniform float     DensityMax;

// Transfer function
uniform sampler1D TransferTexture;

// Primary ray
in vec3 RayPos;
in vec3 RayDir;

uniform int RaySegments;
uniform bool FrontToBack;

// Sample the 3D textur and return the interpolated density value
float SampleDensity(vec3 texCoords) {
	return texture3D(DensityTexture, texCoords).x;
}

// Transfer function
vec4 f(float d) {
	
	 // TODO: Fetch transfer texture size and texture coordinates of outer texels
	 return vec4(5*d);
}

void main() {

	bool hit = false;

	vec3 rayPos = RayPos;
	vec3 rayDir = normalize(RayDir);
	
	// TODO: Test primary ray against bounding volume
	//hit = IntersectRayBox(...);
	//if (!hit) ...


	// The result color
	vec4 result = vec4(0.0);

	if(hit) {

		// TODO: Implement ray marching from t0 to t1
		//for (int i = 0; i <= RaySegments; i++) {
		//    ...
		//    float density = SampleDensity(...);
		//    ...
		//}

	}

	gl_FragColor = result;
}
