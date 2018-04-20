// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the UTIL_REHEARSAL_EXPORTS
// symbol defined on the command line. This symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// UTIL_REHEARSAL_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
#ifdef UTIL_REHEARSAL_EXPORTS
#define UTIL_REHEARSAL_API __declspec(dllexport)
#else
#define UTIL_REHEARSAL_API __declspec(dllimport)
#endif

// This class is exported from the util_rehearsal.dll
class UTIL_REHEARSAL_API Cutil_rehearsal {
public:
	Cutil_rehearsal(void);
	// TODO: add your methods here.
};

extern UTIL_REHEARSAL_API int nutil_rehearsal;

UTIL_REHEARSAL_API int fnutil_rehearsal(void);
