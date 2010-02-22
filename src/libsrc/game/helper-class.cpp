#include "helper-class.h"
#include "basedefs.h"

Helper::Helper()
{

}

Helper::~Helper()
{

}

Logical Helper::checkForOverlap(RECT leftRect, RECT rightRect)
{
	Logical rectOverlap = no;
	//from andre lamothe book
	//if(leftRect.left > rightRect.left && leftRect.left < rightRect.right && leftRect.top < rightRect.top && leftRect.top > rightRect.bottom)

//
//	if(leftRect.right >= rightRect.left && leftRect.right <= rightRect.right && leftRect.bottom <= rightRect.top && leftRect.bottom >= rightRect.bottom)
//		rectOverlap = yes;
//	else if(leftRect.left >= rightRect.left && leftRect.left <= rightRect.right && leftRect.bottom <= rightRect.top && leftRect.bottom >= rightRect.bottom)
//		rectOverlap = yes;
//	else if(leftRect.right >= rightRect.left && leftRect.right <= rightRect.right && leftRect.top <= rightRect.top && leftRect.top >= rightRect.bottom)
//		rectOverlap = yes;
//	else if(leftRect.left >= rightRect.left && leftRect.left <= rightRect.right && leftRect.top <= rightRect.top && leftRect.top >= rightRect.bottom)
//		rectOverlap = yes;


	if(leftRect.right >= rightRect.left && leftRect.left <= rightRect.right && leftRect.bottom <= rightRect.top && leftRect.top >= rightRect.bottom)
		rectOverlap = yes;
	return rectOverlap;
}

Logical Helper::checkForOverlapFullyInside(RECT leftRect, RECT rightRect)
{
	Logical rectOverlap = no;

	if(leftRect.left >= rightRect.left && leftRect.right <= rightRect.right && leftRect.top <= rightRect.top && leftRect.bottom >= rightRect.bottom)
		rectOverlap = yes;
	return rectOverlap;
}
