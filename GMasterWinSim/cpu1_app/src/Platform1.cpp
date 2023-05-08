#include "Platform1.h"

#ifdef __arm__
#include "Platform1Zynq.h"
typedef Platform1Zynq PlatformType;
#elif defined(WIN32)
#include "Platform1Win.h"
typedef Platform1Win PlatformType;
#endif


Platform1* Platform1::obj()
{
	static PlatformType obj;
	return &obj;
}