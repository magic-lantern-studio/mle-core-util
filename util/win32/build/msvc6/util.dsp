# Microsoft Developer Studio Project File - Name="util" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=util - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "util.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "util.mak" CFG="util - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "util - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "util - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "util - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD CPP /nologo /Zp8 /MT /W3 /GX /O2 /I "..\..\include" /I "..\..\..\common\include" /D "MLE_NOT_DLL" /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo
# Begin Special Build Tool
SOURCE="$(InputPath)"
PostBuild_Desc=Install Library
PostBuild_Cmds=mkdir $(MLE_HOME)\lib\tools	copy Release\util.lib $(MLE_HOME)\lib\tools\util.lib	mkdir $(MLE_HOME)\include\mle	xcopy /Y ..\..\..\common\include\mle\* $(MLE_HOME)\include\mle	xcopy /Y ..\..\include\mle\* $(MLE_HOME)\include\mle
# End Special Build Tool

!ELSEIF  "$(CFG)" == "util - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /YX /FD /GZ /c
# ADD CPP /nologo /Zp8 /MTd /W3 /Gm /GX /ZI /Od /I "..\..\include" /I "..\..\..\common\include" /D "MLE_NOT_DLL" /D "MLE_DEBUG" /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /YX /FD /GZ /c
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo /o"Debug/utild.bsc"
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"Debug\utild.lib"
# Begin Special Build Tool
SOURCE="$(InputPath)"
PostBuild_Desc=Install Library
PostBuild_Cmds=mkdir $(MLE_HOME)\lib\tools	copy Debug\utild.lib $(MLE_HOME)\lib\tools\utild.lib	mkdir $(MLE_HOME)\include\mle	xcopy /Y ..\..\..\common\include\mle\* $(MLE_HOME)\include\mle	xcopy /Y ..\..\include\mle\* $(MLE_HOME)\include\mle
# End Special Build Tool

!ENDIF 

# Begin Target

# Name "util - Win32 Release"
# Name "util - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=..\..\..\common\src\mlDebug.c
# End Source File
# Begin Source File

SOURCE=..\..\..\common\src\MleDsoLoader.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\common\src\MleMemoryManager.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\common\src\mlErrno.c
# End Source File
# Begin Source File

SOURCE=..\..\..\common\src\MleTemplate.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\MleWin32MemoryManager.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\MleWin32Path.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\common\src\mlExpandFilename.c
# End Source File
# Begin Source File

SOURCE=..\..\src\mlGetopt.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\common\src\mlLogFile.c
# End Source File
# Begin Source File

SOURCE=..\..\..\common\src\mlUnique.c
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=..\..\..\common\include\mle\mlArray.h
# End Source File
# Begin Source File

SOURCE=..\..\..\common\include\mle\mlAssert.h
# End Source File
# Begin Source File

SOURCE=..\..\..\common\include\mle\mlConfig.h
# End Source File
# Begin Source File

SOURCE=..\..\..\common\include\mle\mlDebug.h
# End Source File
# Begin Source File

SOURCE=..\..\..\common\include\mle\mle.h
# End Source File
# Begin Source File

SOURCE=..\..\..\common\include\mle\MleDsoLoader.h
# End Source File
# Begin Source File

SOURCE=..\..\..\common\include\mle\MleMemoryManager.h
# End Source File
# Begin Source File

SOURCE=..\..\..\common\include\mle\MlePath.h
# End Source File
# Begin Source File

SOURCE=..\..\..\common\include\mle\mlErrno.h
# End Source File
# Begin Source File

SOURCE=..\..\..\common\include\mle\MleTemplate.h
# End Source File
# Begin Source File

SOURCE=..\..\..\common\include\mle\MleUtil.h
# End Source File
# Begin Source File

SOURCE=..\..\include\mle\MleWin32Path.h
# End Source File
# Begin Source File

SOURCE=..\..\..\common\include\mle\mlExpandFilename.h
# End Source File
# Begin Source File

SOURCE=..\..\..\common\include\mle\mlFileio.h
# End Source File
# Begin Source File

SOURCE=..\..\include\mle\mlGetopt.h
# End Source File
# Begin Source File

SOURCE=..\..\..\common\include\mle\mlLogFile.h
# End Source File
# Begin Source File

SOURCE=..\..\..\common\include\mle\mlMacros.h
# End Source File
# Begin Source File

SOURCE=..\..\..\common\include\mle\mlMalloc.h
# End Source File
# Begin Source File

SOURCE=..\..\include\mle\mlPlatformDefs.h
# End Source File
# Begin Source File

SOURCE=..\..\..\common\include\mle\mlToken.h
# End Source File
# Begin Source File

SOURCE=..\..\..\common\include\mle\mlTypes.h
# End Source File
# Begin Source File

SOURCE=..\..\..\common\include\mle\mlUnique.h
# End Source File
# End Group
# End Target
# End Project
