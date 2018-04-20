// util_rehearsal.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "util_rehearsal.h"


// This is an example of an exported variable
UTIL_REHEARSAL_API int nutil_rehearsal=0;

// This is an example of an exported function.
UTIL_REHEARSAL_API int fnutil_rehearsal(void)
{
	return 42;
}

// This is the constructor of a class that has been exported.
// see util_rehearsal.h for the class definition
Cutil_rehearsal::Cutil_rehearsal()
{
	return;
}
