#ifndef HELPER_CLASS_H
#define HELPER_CLASS_H

#include "basedefs.h"
#include "windows.h"
class EXPORT Helper
{
public:
	Helper();
	~Helper();
	static Logical checkForOverlap(RECT leftRect, RECT rightRect);
	static Logical checkForOverlapFullyInside(RECT leftRect, RECT rightRect);
};

#endif
