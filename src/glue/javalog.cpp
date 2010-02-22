#include <jni.h>
#include "javalog.h"
#include "log-class.h"
#include <iostream>

using namespace std;

JNIEXPORT void JNICALL Java_com_waqqas_log_JavaLog_log
  (JNIEnv *env, jobject jObj, jstring s)
{
	const char *str = env->GetStringUTFChars(s, 0);
	if(str != NULL && strlen(str) > 0)
	{
		lprintf("%s",str);
	}
	env->ReleaseStringUTFChars(s, str);
	return; 
}