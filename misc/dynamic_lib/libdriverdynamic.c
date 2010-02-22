#include <iostream.h>
#include "ctest.h"
#include <dlfcn.h> 

int main()
{
	void *lib_handle;
	void (*fn)(int *);
	int x;
	char *error;
	
	lib_handle = dlopen("/usr/lib/libctest.so",RTLD_LAZY);
	if(!lib_handle) {
		fprintf(stderr,"%s\n",dlerror);
		exit(1);
	}
	
	fn = dlsym(lib_handle,"ctest1");
	if((error = dlerror()) != NULL) {
		fprintf(stderr,"%s\n",error);
		exit(1);
	}	
	(*fn)(&x);
	cout<<"Valx="<<x<<endl;
	dlclose(lib_handle);
	return 0;
}
