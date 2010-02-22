/*
 *  copy.cpp
 *  
 *
 *  Created by waqqas sharif on Sun Feb 18 2007.
 *  Copyright (c) 2007 __MyCompanyName__. All rights reserved.
 *
 */

#include "copy.h"
#include <stdio.h>
CopyClass::CopyClass()
{
	a = 0;
	str = NULL;
}

CopyClass::CopyClass(int i,char *s)
{
    a = i;
    str = s;
}

CopyClass::CopyClass(CopyClass const& cp)
{
    a = cp.a;
    str= cp.str;
}

CopyClass::~CopyClass()
{
    
}

void CopyClass::setA(int i)
{
    a = i;
}

int CopyClass::getA()
{
    return a;
}

void CopyClass::setStr(char *s)
{
    str = s;
}

char* CopyClass::getStr()
{
    return str;
}

void display()
{
    //printf("A %d displayed via friend function\n",a);
}
