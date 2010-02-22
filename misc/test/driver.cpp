/*
 *  driver.cpp
 *  
 *
 *  Created by waqqas sharif on Sun Feb 18 2007.
 *  Copyright (c) 2007 __MyCompanyName__. All rights reserved.
 *
 */
#include <iostream.h>
#include "copy.h"

int main()
{
    CopyClass *cc = new CopyClass();
    cc->setA(10);
    cc->setStr("abc");
    cout<<cc->getStr()<<endl;
    cout<<cc->getA()<<endl;
    CopyClass *cp = new CopyClass(*cc);
    //cp = cc;
    delete cc;
    cout<<cp->getA()<<endl;
    cout<<cp->getStr()<<endl;
    display();
    return 0;
}
