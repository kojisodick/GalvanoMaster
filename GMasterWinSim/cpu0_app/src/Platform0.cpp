
#ifdef __arm__
#include "Platform0.h"
typedef Platform0Zynq Platform0Type;
#elif defined(_WIN32)
#include "Platform0Win.h"
typedef Platform0Win Platform0Type;
#endif

Platform0 * Platform0::obj()
{
	static Platform0Type obj;
	return &obj;
}

