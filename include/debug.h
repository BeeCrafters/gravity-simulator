#include <stdio.h>
#include <stdlib.h>

#ifndef __DEBUG_H__
#define __DEBUG_H__

#ifdef __DEBUG__



// Returns r if x equals NULL after printing debug information
#define debug_return(x, r) \
	if (!(x)) { \
		printf("debug_return assertion failed\n\t%s:%i: %s: assertion \"%s\" failed.\nAborted.\n", __FILE__, __LINE__, __ASSERT_FUNCTION, #x); \
		return (r); \
	}

// Prints debug information if x equals NULL
#define debug_test(x) \
	if (!(x)) { \
		printf("debug_test assertion failed\n\t%s:%i: %s: assertion \"%s\" failed.\nAborted.\n", __FILE__, __LINE__, __ASSERT_FUNCTION, #x); \
	}

// Exits with code c if x equals NULL after printing debug information
#define debug_exit(x, c) \
	if (!(x)) { \
		printf("debug_test assertion failed\n\t%s:%i: %s: assertion \"%s\" failed.\nAborted.\n", __FILE__, __LINE__, __ASSERT_FUNCTION, #x); \
		exit(c); \
	}

// Prints toprint to stdout
#define debug_print(toprint) \
	if (!(x)) { \
		printf("debug_test assertion failed\n\t%s:%i: %s: assertion \"%s\" failed.\nAborted.\n", __FILE__, __LINE__, __ASSERT_FUNCTION, #x); \
		exit(c); \
	}



#else



// Returns r if x equals NULL
#define debug_return(x, r) if (!(x)) return (r);

// Does nothing, intentionally.
#define debug_test(x)

// Exits with code c if x equals NULL
#define debug_exit(x, c) if (!(x)) exit(c);

// Does nothing, intentionally.
#define debug_print(toprint)



#endif



// This here macro shouldn't be in debug.h, but I couldn't really find somewhere else.
// Calculates the maximum number of different values a type can store.
#define MAX_VALUE_NUMBER(type) \
	( ((unsigned __int128) 1) << (8 * sizeof(type)) )



#endif
