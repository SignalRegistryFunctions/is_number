#ifndef SRFUN_IS_NUMBER
#define SRFUN_IS_NUMBER

// Standard C libraries
#include <stdbool.h>

// External libraries

#ifndef FLOAT
#define FLOAT double
#endif


#ifndef FUNCTION
#define FUNCTION "anonymous"
#endif

#ifndef VERSION
#define VERSION "0.0.0"
#endif


bool is_number(const char *str);

#endif
