#include <stdint.h>

#ifndef __VEC_H__
#define __VEC_H__

typedef struct {
	union {
		int32_t x;
		int32_t w;
		int32_t width;
	};
	union {
		int32_t y;
		int32_t h;
		int32_t height;
	};
} __attribute__((packed)) i32vec2_t;
typedef i32vec2_t ivec2_t;

// t is the type
#define vec2(x, y, t) (t){x, y}

#define ivec2(x, y) (ivec2_t){x, y}

#endif
