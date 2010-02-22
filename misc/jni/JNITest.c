#include <stdio.h>
#include "/System/Library/Frameworks/JavaVM.framework/Versions/1.4.1/Headers/jni.h"
#include "JNITest.h"

JNIEXPORT void JNICALL 
Java_JNITest_print (JNIEnv *jenv, jobject jobr)
{
	printf("Hello world from native applcation written in C\n");
//	cout<<"Hello world from native applcation written in C\n";
	return; 
}

