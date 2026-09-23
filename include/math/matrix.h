#include <debug.h>
#include <math.h>
#include <math/vec.h>
#include <math/quaternion.h>

#ifndef __MATRIX_H__
#define __MATRIX_H__

typedef float matrix32_4x4[16];

// Translate standard matrix index notation to matrix array index.
#define m44idx(row, column) ( ( ( (column) - 1) * 4) + ( (row) - 1) )

void matrix32_4x4_copy(matrix32_4x4 out, matrix32_4x4 in) {
	for (int i = 0; i < 16; i++)
		out[i] = in[i];
}

/*
 * Creates a perspective projection matrix.
 *
 * You can't assign this to a matrix32_4x4 on the fly, it will only work
 * on initializations. In code, you will have to use this macro's
 * function version. The function version has the suffix "_assign".
 */
#define matrix_projection_perspective(left, right, bottom, top, near, far) \
	((matrix32_4x4) { \
		(2 * ((float)near)) / (((float)right) - ((float)left)), 0, 0, 0, \
		0, (2 * ((float)near)) / (((float)top) - ((float)bottom)), 0, 0, \
		(((float)right) + ((float)left)) / (((float)right) - ((float)left)), (((float)top) + ((float)bottom)) / (((float)top) - ((float)bottom)), (((float)near) + ((float)far)) / (((float)near) - ((float)far)), -1, \
		0, 0, (-2 * ((float)far) * ((float)near)) / (((float)far) - ((float)near)), 0 \
	})

/*
 * Creates a perspective projection matrix, then assigns it to destination.
 */
void matrix_projection_perspective_assign(float left, float right, float bottom, float top, float near, float far, matrix32_4x4 destination) {
	matrix32_4x4 swap = (matrix32_4x4) {
		(2 * ((float)near)) / (((float)right) - ((float)left)), 0, 0, 0,
		0, (2 * ((float)near)) / (((float)top) - ((float)bottom)), 0, 0,
		(((float)right) + ((float)left)) / (((float)right) - ((float)left)), (((float)top) + ((float)bottom)) / (((float)top) - ((float)bottom)), (((float)near) + ((float)far)) / (((float)near) - ((float)far)), -1,
		0, 0, (-2 * ((float)far) * ((float)near)) / (((float)far) - ((float)near)), 0
	};
	for (int i = 0; i < 16; i++)
		destination[i] = swap[i];
}

/*
 * Creates a perspective projection matrix with less variables.
 *
 * You can't assign this to a matrix32_4x4 on the fly, it will only work
 * on initializations. In code, you will have to use this macro's
 * function version. The function version has the suffix "_assign".
 */
#define matrix_projection_perspective_simple(width, height, near, far) \
	((matrix32_4x4) { \
		(2 * ((float)near)) / ((float)width),0,0,0, \
		0,(2 * ((float)near)) / ((float)height),0,0, \
		0,0,(((float)near) + ((float)far)) / (((float)near) - ((float)far)),-1, \
		0,0,(-2 * ((float)far) * ((float)near)) / (((float)far) - ((float)near)),0 \
	})

/*
 * Creates a perspective projection matrix with less variables, then assigns it to destination.
 */
void matrix_projection_perspective_simple_assign(int width, int height, float near, float far, matrix32_4x4 destination) {
	matrix32_4x4 swap = (matrix32_4x4) {
		(2 * ((float)near)) / ((float)width),0,0,0,
		0,(2 * ((float)near)) / ((float)height),0,0,
		0,0,(((float)near) + ((float)far)) / (((float)near) - ((float)far)),-1,
		0,0,(-2 * ((float)far) * ((float)near)) / (((float)far) - ((float)near)),0
	};
	for (int i = 0; i < 16; i++)
		destination[i] = swap[i];
}

// What, there ain't no cot functions in math.h????
#define cotf(rad) (cosf(rad) / sinf(rad))

/*
 * Creates a perspective projection matrix with more conventional inputs,
 * then assigns it to destination.
 */
void matrix_projection_perspective_fov_assign(float fov, float render_distance, int width, int height, matrix32_4x4 destination) {
	float near = 2 * ( ((float)width / 2) * cotf(fov / 2) );
	matrix32_4x4 swap = matrix_projection_perspective_simple(width, height, near, render_distance + near);
	for (int i = 0; i < 16; i++)
		destination[i] = swap[i];
}

// TODO: Write doc
void matrix_rotation_intrinsic_assign(float alpha_z, float beta_y, float gamma_x, matrix32_4x4 destination) {
	matrix32_4x4 swap = {
		cosf(alpha_z) * cosf(beta_y), sinf(alpha_z) * cosf(beta_y), - sinf(beta_y), 0,
		(cosf(alpha_z) * sinf(beta_y) * sinf(gamma_x)) - (sinf(alpha_z) * cosf(gamma_x)), (sinf(alpha_z) * sinf(beta_y) * sinf(gamma_x)) + (cosf(alpha_z) * cosf(gamma_x)), cosf(beta_y) * sinf(gamma_x), 0,
		(cosf(alpha_z) * sinf(beta_y) * cosf(gamma_x)) + (sinf(alpha_z) * sinf(gamma_x)), (sinf(alpha_z) * sinf(beta_y) * cosf(gamma_x)) - (cosf(alpha_z) * sinf(gamma_x)), cosf(beta_y) * cosf(gamma_x), 0,
		0, 0, 0, 1
	};
	for (int i = 0; i < 16; i++)
		destination[i] = swap[i];
}

// TODO: Write doc
void matrix_rotation_extrinsic_assign(float alpha_x, float beta_y, float gamma_z, matrix32_4x4 destination) {
	matrix32_4x4 swap = {
		cosf(beta_y) * cosf(gamma_z), (cosf(alpha_x) * sinf(gamma_z)) + (sinf(alpha_x) * sinf(beta_y) * cosf(gamma_z)), (sinf(alpha_x) * sinf(gamma_z)) - (cosf(alpha_x) * sinf(beta_y) * cosf(gamma_z)), 0,
		-cosf(beta_y) * sinf(gamma_z), (cosf(alpha_x) * cosf(gamma_z)) - (sinf(alpha_x) * sinf(beta_y) * sinf(gamma_z)), (sinf(alpha_x) * cosf(gamma_z)) + (cosf(alpha_x) * sinf(beta_y) * sinf(gamma_z)), 0,
		sinf(beta_y), -sinf(alpha_x) * cosf(beta_y), cosf(alpha_x) * cosf(beta_y), 0,
		0, 0, 0, 1
	};
	for (int i = 0; i < 16; i++)
		destination[i] = swap[i];
}

/*
 * Attaches position offsets to a rotation matrix, finalizing a
 * full transformation matrix.
 *
 * The offsets attached here are processed after the rotations.
 * Use this to translate model coordinates to world coordinates,
 * or world coordinates to eye coordinates.
 *
 * Modifies the contents of rotation_matrix.
 */
#define matrix_rotation_attach_position(rotation_matrix, pos_offset) \
	(rotation_matrix)[m44idx(1,4)] = (pos_offset).x; \
	(rotation_matrix)[m44idx(2,4)] = (pos_offset).y; \
	(rotation_matrix)[m44idx(3,4)] = (pos_offset).z;

#define square(x) ( (x) * (x) )

// TODO: Write doc
void matrix_rotation_quaternion_assign(quaternion32* in, matrix32_4x4 destination) {
	matrix32_4x4 swap = {
		1 - (2 * ( square(in->c) + square(in->d)) ), 2 * ( (in->b * in->c) + (in->a * in->d) ), 2 * ( (in->b * in->d) - (in->a * in->c) ), 0,
		2 * ( (in->b * in->c) - (in->a * in->d) ), 1 - (2 * ( square(in->b) + square(in->d)) ), 2 * ( (in->c * in->d) - (in->a * in->b) ), 0,
		2 * ( (in->b * in->d) + (in->a * in->c) ), 2 * ( (in->c * in->d) - (in->a * in->b) ), 1 - (2 * ( square(in->b) + square(in->c)) ), 0,
		0, 0, 0, 1
	};
	for (int i = 0; i < 16; i++)
		destination[i] = swap[i];
}

#endif
