/*
 *  templateclass.h
 *  
 *
 *  Created by waqqas sharif on Tue Feb 20 2007.
 *  Copyright (c) 2007 __MyCompanyName__. All rights reserved.
 *
 */
#include<string.h>
#include <iostream.h>

template <class T>
T Max(T a, T b)
{
    return a > b ? a : b;
} 
    
template <> 
char* Max(char* a,char* b)
{
    return  strcmp(a,b) >  0  ? a : b;
}
