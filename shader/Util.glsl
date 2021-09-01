
#version 130

/*******************************************************************************

*******************************************************************************/

// TODO: As there is no include mechanism in GLSL, include guards are useless

#ifndef _UTIL_GLSL_
#define _UTIL_GLSL_

#ifndef UTIL_GLSL_EXTENSIONS
#define UTIL_GLSL_EXTENSIONS			1
#endif

#ifndef UTIL_GEOMETRY
#define UTIL_GEOMETRY					1
#endif

/******************************************************************************/

#if UTIL_GLSL_EXTENSIONS

/******************************************************************************/

// Return y s.th. (x1 - x) / (x1 - x0) == (y1 - y) / (y1 - y0)
float map(float x, float x0, float x1, float y0, float y1) {
	return y1 - (x1 - x) * (y1 - y0) / (x1 - x0);
}

// Return y s.th. (x1 - x) / (x1 - x0) == (y1 - y) / (y1 - y0)
vec2 map(vec2 x, vec2 x0, vec2 x1, vec2 y0, vec2 y1) {
	return vec2(
		map(x.x, x0.x, x1.x, y0.x, y1.x), 
		map(x.y, x0.y, x1.y, y0.y, y1.y));
}

// Return y s.th. (x1 - x) / (x1 - x0) == (y1 - y) / (y1 - y0)
vec3 map(vec3 x, vec3 x0, vec3 x1, vec3 y0, vec3 y1) {
	return vec3(
		map(x.x, x0.x, x1.x, y0.x, y1.x), 
		map(x.y, x0.y, x1.y, y0.y, y1.y), 
		map(x.z, x0.z, x1.z, y0.z, y1.z));
}

// Return y s.th. (x1 - x) / (x1 - x0) == (y1 - y) / (y1 - y0)
vec4 map(vec4 x, vec4 x0, vec4 x1, vec4 y0, vec4 y1) {
	return vec4(
		map(x.x, x0.x, x1.x, y0.x, y1.x), 
		map(x.y, x0.y, x1.y, y0.y, y1.y), 
		map(x.z, x0.z, x1.z, y0.z, y1.z), 
		map(x.w, x0.w, x1.w, y0.w, y1.w));
}

uint hash(uint n) {
	n = (n + 0x7ed55d16u) + (n << 12);
	n = (n ^ 0xc761c23cu) ^ (n >> 19);
	n = (n + 0x165667b1u) + (n << 5);
	n = (n + 0xd3a2646cu) ^ (n << 9);
	n = (n + 0xfd7046c5u) + (n << 3);
	n = (n ^ 0xb55a4f09u) ^ (n >> 16);
	return n;
}

/******************************************************************************/

#endif /* UTIL_GLSL_EXTENSIONS */

/******************************************************************************/

#if UTIL_GEOMETRY

/******************************************************************************/

// Intersect an infinite ray with a solid box
bool IntersectInfiniteRayBox(vec3 rayPos, vec3 rayDir, vec3 boxMin, vec3 boxMax, out float t0, out float t1) {
	vec3 rayDirInv = 1.0 / rayDir;

	vec3 slabMin = (boxMin - rayPos) * rayDirInv;
	vec3 slabMax = (boxMax - rayPos) * rayDirInv;

	vec3 absMin = min(slabMin, slabMax);
	vec3 absMax = max(slabMin, slabMax);

	t0 = max(absMin.x, max(absMin.y, absMin.z));
	t1 = min(absMax.x, min(absMax.y, absMax.z));

	return t0 <= t1;
}

// Intersect a ray with a solid box
bool IntersectRayBox(vec3 rayPos, vec3 rayDir, vec3 boxMin, vec3 boxMax, out float t0, out float t1) {
	vec3 rayDirInv = 1.0 / rayDir;

	vec3 slabMin = (boxMin - rayPos) * rayDirInv;
	vec3 slabMax = (boxMax - rayPos) * rayDirInv;

	vec3 absMin = min(slabMin, slabMax);
	vec3 absMax = max(slabMin, slabMax);

	t0 = max(0.0, max(absMin.x, max(absMin.y, absMin.z)));
	t1 = min(absMax.x, min(absMax.y, absMax.z));

	return t0 <= t1;
}

bool IntersectRayBox(vec3 rayPos, vec3 rayDir, vec3 boxMin, vec3 boxMax, out float t) {
	float t0, t1;
	bool hit = IntersectInfiniteRayBox(rayPos, rayDir, boxMin, boxMax, t0, t1);

	t = t0;
	if (t <= 0.0) {
		t = t1;
	}

	return hit && t > 0.0;
}

bool IntersectRayBox(vec3 rayPos, vec3 rayDir, vec3 boxMin, vec3 boxMax) {
	float t0, t1;
	return IntersectRayBox(rayPos, rayDir, boxMin, boxMax, t0, t1);
}

/******************************************************************************/

#endif /* UTIL_GEOMETRY */

/******************************************************************************/

#endif /* _UTIL_GLSL_ */
