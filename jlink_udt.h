/*
	dont define like #ifndef ... #define ... #endif routes
	because the stdio.h must be last referance at this file
*/

#include <stdio.h>

extern FILE* __UDT_BLUE;
#ifdef stdin
#undef stdin
#endif
#define stdin	__UDT_BLUE


