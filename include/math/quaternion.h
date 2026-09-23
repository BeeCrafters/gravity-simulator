#include <math.h>
#include <math/vec.h>

#ifndef __QUATERNION_H__
#define __QUATERNION_H__

typedef struct {
	float a;
	float b;
	float c;
	float d;
} __attribute__((packed)) quaternion32;
typedef quaternion32 quaternion;

// Constructs a pure quaternion from a 3 dimensional vector
#define __quat_purify(vector3) ((quaternion32) {0, (vector3).x, (vector3).y, (vector3).z})

// Constructs a pure quaternion from an fvec3, writes into destination.
void quat_purify_fvec3(fvec3 vector, quaternion* destination) {
	*destination = __quat_purify(vector);
}

/*
 * Multiplies two quaternions. Since quaternion multiplication is non commutative,
 * the order of the quaternions matter. Parameters are named accordingly.
 */
void quat_mul(quaternion* left, quaternion* right, quaternion* out_result) {
	out_result->a = (left->a * right->a) + (- left->b * right->b) + (- left->c * right->c) + (- left->d * right->d);
	out_result->b = (left->a * right->b) + (left->b * right->a) + (left->c * right->d) + (- left->d * right->c);
	out_result->c = (left->a * right->c) + (left->c * right->a) + (- left->b * right->d) + (left->d * right->b);
	out_result->d = (left->a * right->d) + (left->d * right->a) + (left->b * right->c) + (- left->c * right->b);
}

/*
 * This macro is just a rename of quat_mul with easier parameter names.
 * First rotations should be at the right of the multiplication.
 *
 * Multiplies two quaternions. Since quaternion multiplication is non commutative,
 * the order of the quaternions matter. Parameters are named accordingly. (from quat_mul)
 */
#define quat_rotate(first_rotation, second_rotation, out) quat_mul((second_rotation), (first_rotation), (out))

/*
 * Constructs a quaternion from the angle and the axis of rotation
 *
 * The function doesn't check whether the axis vector is normalized or not.
 *
 * For rotation quaternions, also called unit quaternions, a length of 1 is required,
 * for this reason wrapping the axis vector in fvec3_normalize() from math/vec.h
 * is a good idea.
 */
void quat_rotation_construct(float angle, fvec3 axis, quaternion* destination) {
	destination->a = cosf(angle/2);
	float swap = sinf(angle/2);
	destination->b = swap * axis.x;
	destination->c = swap * axis.y;
	destination->d = swap * axis.z;
}

#endif
