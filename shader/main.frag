
#version 150 compatibility

/******************************************************************************/

// External function declarations
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

const float EXTINCTION = 2.f;

// Sample the 3D textur and return the interpolated density value
float SampleDensity(vec3 texCoords) {
	return texture3D(DensityTexture, texCoords).x;
}

// Transfer function
vec4 f(float d) {
	
	// Fetch transfer texture size and texture coordinates of outer texels
	int w = textureSize(TransferTexture, 0);
	float s0 = 0.5 * (1.0 / w);
	float s1 = 1.0 - s0;

	// Compute texture coordinate
	float s =  map(d, DensityMin, DensityMax, s0, s1);

	// Sample the texture
	return texture1D(TransferTexture, s);
}

void main() {

	bool hit = false;

	vec3 rayPos = RayPos;
	vec3 rayDir = normalize(RayDir);

	// Test primary ray against bounding volume
	float t0, t1;
	hit = IntersectRayBox(rayPos, rayDir, BoundsMin, BoundsMax, t0, t1);

	if(!hit)
		discard;

	// The result color
	vec4 result = vec4(0.0);

	if(hit) {

		// Compute ray parameter and increments along the ray
		float t, dt;

		// Stepping depends on whether we are going towards or away from the camera
		if (FrontToBack) {
			// Start at the first hitpoint and do positive increments
			dt = (t1 - t0) / float(RaySegments);
			t = t0;
			result.a = 0;
		} else {
			// Start at the last hitpoint and do negative increments
			dt = (t0 - t1) / float(RaySegments);
			t = t1;
			result.a = 0;											// For correct blending with the background
		}

		// Compute starting position and position increment
		vec3 dp = dt * rayDir;
		vec3 p = rayPos + t * rayDir;

		// March along the ray
		for (int i = 0; i <= RaySegments; i++) {
			// Compute texture coordinates
			vec3 texCoords = map(p, BoundsMin, BoundsMax, vec3(0.0), vec3(1.0));

			// Fetch density from texture
			float density = SampleDensity(texCoords);
			
			// Apply transfer function
			vec4 color = f(density);
			
			// Interpret values from transfer function
			vec3 L_e      = color.rgb;									// Emission density L_e [0, inf] in radiance/m units
			float sigma_t = color.a;									// Extinction coefficient sigma_t [0, inf] in 1/m units

			// Compute emission
			vec3 b = L_e; //* abs(dt);

			// Compute transparency
			float theta = exp(-sigma_t * abs(dt) * EXTINCTION);

			// Compute opacity
			float alpha = 1.0 - theta;

			// Premultiply color
			b = b * alpha;

			// Compositing
			if (FrontToBack) {

				result.rgb = result.rgb + (1 - result.a) * b;
                // If we would accumulate transparency:
                // result.theta = result.theta * theta
                // Written as opacity:
				result.a   = 1 - ((1 - result.a) * theta);
			} else {
				result.rgb = (1 - alpha) * result.rgb + b;
				result.a   = 1 - ((1 - result.a) * theta);
			}

			// Step along the ray
			t += dt;
			p += dp;
		}
	}

	gl_FragColor = result;
}
