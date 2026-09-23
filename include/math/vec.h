#include <stdint.h>
#include <math.h>

#ifndef __VEC_H__
#define __VEC_H__

typedef struct {
	int32_t x;
	int32_t y;
} __attribute__((packed)) i32vec2;
typedef i32vec2 ivec2;

typedef struct {
	int32_t x;
	int32_t y;
	int32_t z;
} __attribute__((packed)) i32vec3;
typedef i32vec3 ivec3;



typedef struct {
	float x;
	float y;
} __attribute__((packed)) f32vec2;
typedef f32vec2 fvec2;

typedef struct {
	float x;
	float y;
	float z;
} __attribute__((packed)) f32vec3;
typedef f32vec3 fvec3;

typedef struct {
	float x;
	float y;
	float z;
	float w;
} __attribute__((packed)) f32vec4;
typedef f32vec4 fvec4;

#define vec3_len_squared(vector3) ( ( (vector3).x * (vector3).x ) + ( (vector3).y * (vector3).y ) + ( (vector3).z * (vector3).z ) )
#define vec3_len(vector3) (sqrt(vec3_len_squared(vector3)))
#define fvec3_len(vector3) (sqrtf(vec3_len_squared(vector3)))

#define vec3_scale(vector3, scaler) { ((vector3).x * (scaler)), ((vector3).y * (scaler)), ((vector3).z * (scaler)) }
#define vec3_scale_inverse(vector3, scaler) { ((vector3).x / (scaler)), ((vector3).y / (scaler)), ((vector3).z / (scaler)) }

/*
 * Scales the vector to a length of one.
 */
fvec3 fvec3_normalize(fvec3 in) {
	return (fvec3)vec3_scale_inverse(in, fvec3_len(in));
}

// Constructors for 2d vector types.
#define vec2(x, y, type) ((type) {x, y})
#define ivec2(x, y) ((ivec2) {x, y})
#define fvec2(x, y) ((fvec2) {x, y})

// Constructors for 3d vector types.
#define vec3(x, y, z, type) ((type) {x, y, z})
#define ivec3(x, y, z) ((ivec3) {x, y, z})
#define fvec3(x, y, z) ((fvec3) {x, y, z})

// Constructors for 4d vector types.
#define vec4(x, y, z, w, type) ((type) {x, y, z, w})
#define fvec4(x, y, z, w) ((fvec4) {x, y, z, w})

#endif
