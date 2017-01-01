// SHARED HEADER BETWEEN CPU AND GPU
#ifdef __cplusplus
#include "dataTypes.hpp"
#endif

struct LoadConstants
{
	uint elementsMask;
	uint writeIndex;		// Always 0xffffffff (= never write). But the compiler doesn't know this :)

	uint2 padding;
};

