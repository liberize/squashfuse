#include "squashfuse_dlopen.h"

void *libhandle = NULL;
int have_libloaded = 0;

const char *load_library_errmsg = "";

