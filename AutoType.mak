# Microsoft Developer Studio Generated NMAKE File, Format Version 4.20
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101
# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

!IF "$(CFG)" == ""
CFG=KeyGen - Win32 Debug
!MESSAGE No configuration specified.  Defaulting to KeyGen - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "AutoType - Win32 Debug" && "$(CFG)" !=\
 "AutoType - Win32 Release" && "$(CFG)" != "Hook - Win32 Release" && "$(CFG)" !=\
 "Hook - Win32 Debug" && "$(CFG)" != "KeyGen - Win32 Release" && "$(CFG)" !=\
 "KeyGen - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE on this makefile
!MESSAGE by defining the macro CFG on the command line.  For example:
!MESSAGE 
!MESSAGE NMAKE /f "AutoType.mak" CFG="KeyGen - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "AutoType - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE "AutoType - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "Hook - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "Hook - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "KeyGen - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "KeyGen - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 
################################################################################
# Begin Project
# PROP Target_Last_Scanned "AutoType - Win32 Debug"

!IF  "$(CFG)" == "AutoType - Win32 Debug"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "WinDebug"
# PROP BASE Intermediate_Dir "WinDebug"
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "WinDebug"
# PROP Intermediate_Dir "WinDebug"
OUTDIR=.\WinDebug
INTDIR=.\WinDebug

ALL : "Hook - Win32 Debug" "$(OUTDIR)\LetMeTyp.exe"

CLEAN : 
	-@erase "$(INTDIR)\AutoTdoc.obj"
	-@erase "$(INTDIR)\AutoTvw.obj"
	-@erase "$(INTDIR)\AutoType.obj"
	-@erase "$(INTDIR)\AutoType.pch"
	-@erase "$(INTDIR)\AutoType.res"
	-@erase "$(INTDIR)\charsdlg.obj"
	-@erase "$(INTDIR)\crc16.obj"
	-@erase "$(INTDIR)\crc32.obj"
	-@erase "$(INTDIR)\fpwdlg.obj"
	-@erase "$(INTDIR)\HookApp.obj"
	-@erase "$(INTDIR)\mainfrm.obj"
	-@erase "$(INTDIR)\newpwdlg.obj"
	-@erase "$(INTDIR)\pwdlg.obj"
	-@erase "$(INTDIR)\RegisterDlg.obj"
	-@erase "$(INTDIR)\settings.obj"
	-@erase "$(INTDIR)\soundx.obj"
	-@erase "$(INTDIR)\Splash.obj"
	-@erase "$(INTDIR)\stdafx.obj"
	-@erase "$(INTDIR)\suggestw.obj"
	-@erase "$(INTDIR)\vc40.idb"
	-@erase "$(INTDIR)\vc40.pdb"
	-@erase "$(OUTDIR)\LetMeTyp.exe"
	-@erase "$(OUTDIR)\LetMeTyp.ilk"
	-@erase "$(OUTDIR)\LetMeTyp.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
# ADD BASE CPP /nologo /MD /W3 /GX /Zi /Od /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "_AFXDLL" /FR /Yu"stdafx.h" /c
# ADD CPP /nologo /G5 /MDd /W3 /Gm /Gi /GX /Zi /Od /Gy /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Yu"stdafx.h" /c
# SUBTRACT CPP /Fr
CPP_PROJ=/nologo /G5 /MDd /W3 /Gm /Gi /GX /Zi /Od /Gy /D "_DEBUG" /D "WIN32" /D\
 "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Fp"$(INTDIR)/AutoType.pch" /Yu"stdafx.h"\
 /Fo"$(INTDIR)/" /Fd"$(INTDIR)/" /c 
CPP_OBJS=.\WinDebug/
CPP_SBRS=.\.

.c{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

.cpp{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

.cxx{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

.c{$(CPP_SBRS)}.sbr:
   $(CPP) $(CPP_PROJ) $<  

.cpp{$(CPP_SBRS)}.sbr:
   $(CPP) $(CPP_PROJ) $<  

.cxx{$(CPP_SBRS)}.sbr:
   $(CPP) $(CPP_PROJ) $<  

MTL=mktyplib.exe
MTL_PROJ=
RSC=rc.exe
# ADD BASE RSC /l 0x407 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x407 /d "_DEBUG" /d "_AFXDLL"
RSC_PROJ=/l 0x407 /fo"$(INTDIR)/AutoType.res" /d "_DEBUG" /d "_AFXDLL" 
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/AutoType.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386
# SUBTRACT BASE LINK32 /pdb:none
# ADD LINK32 winmm.lib /nologo /subsystem:windows /debug /machine:I386 /out:"WinDebug/LetMeTyp.exe"
# SUBTRACT LINK32 /pdb:none
LINK32_FLAGS=winmm.lib /nologo /subsystem:windows /incremental:yes\
 /pdb:"$(OUTDIR)/LetMeTyp.pdb" /debug /machine:I386\
 /out:"$(OUTDIR)/LetMeTyp.exe" 
LINK32_OBJS= \
	"$(INTDIR)\AutoTdoc.obj" \
	"$(INTDIR)\AutoTvw.obj" \
	"$(INTDIR)\AutoType.obj" \
	"$(INTDIR)\AutoType.res" \
	"$(INTDIR)\charsdlg.obj" \
	"$(INTDIR)\crc16.obj" \
	"$(INTDIR)\crc32.obj" \
	"$(INTDIR)\fpwdlg.obj" \
	"$(INTDIR)\HookApp.obj" \
	"$(INTDIR)\mainfrm.obj" \
	"$(INTDIR)\newpwdlg.obj" \
	"$(INTDIR)\pwdlg.obj" \
	"$(INTDIR)\RegisterDlg.obj" \
	"$(INTDIR)\settings.obj" \
	"$(INTDIR)\soundx.obj" \
	"$(INTDIR)\Splash.obj" \
	"$(INTDIR)\stdafx.obj" \
	"$(INTDIR)\suggestw.obj" \
	".\Hook\Debug\LMTHook.lib"

"$(OUTDIR)\LetMeTyp.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "AutoType - Win32 Release"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "WinRel"
# PROP BASE Intermediate_Dir "WinRel"
# PROP Use_MFC 5
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "WinRel"
# PROP Intermediate_Dir "WinRel"
OUTDIR=.\WinRel
INTDIR=.\WinRel

ALL : "Hook - Win32 Release" "$(OUTDIR)\LetMeTyp.exe"

CLEAN : 
	-@erase "$(INTDIR)\AutoTdoc.obj"
	-@erase "$(INTDIR)\AutoTvw.obj"
	-@erase "$(INTDIR)\AutoType.obj"
	-@erase "$(INTDIR)\AutoType.pch"
	-@erase "$(INTDIR)\AutoType.res"
	-@erase "$(INTDIR)\charsdlg.obj"
	-@erase "$(INTDIR)\crc16.obj"
	-@erase "$(INTDIR)\crc32.obj"
	-@erase "$(INTDIR)\fpwdlg.obj"
	-@erase "$(INTDIR)\HookApp.obj"
	-@erase "$(INTDIR)\mainfrm.obj"
	-@erase "$(INTDIR)\newpwdlg.obj"
	-@erase "$(INTDIR)\pwdlg.obj"
	-@erase "$(INTDIR)\RegisterDlg.obj"
	-@erase "$(INTDIR)\settings.obj"
	-@erase "$(INTDIR)\soundx.obj"
	-@erase "$(INTDIR)\Splash.obj"
	-@erase "$(INTDIR)\stdafx.obj"
	-@erase "$(INTDIR)\suggestw.obj"
	-@erase "$(OUTDIR)\LetMeTyp.exe"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "_AFXDLL" /FR /Yu"stdafx.h" /c
# ADD CPP /nologo /G5 /MT /W3 /GX /O2 /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /Yu"stdafx.h" /c
# SUBTRACT CPP /Fr
CPP_PROJ=/nologo /G5 /MT /W3 /GX /O2 /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D\
 "_MBCS" /Fp"$(INTDIR)/AutoType.pch" /Yu"stdafx.h" /Fo"$(INTDIR)/" /c 
CPP_OBJS=.\WinRel/
CPP_SBRS=.\.

.c{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

.cpp{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

.cxx{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

.c{$(CPP_SBRS)}.sbr:
   $(CPP) $(CPP_PROJ) $<  

.cpp{$(CPP_SBRS)}.sbr:
   $(CPP) $(CPP_PROJ) $<  

.cxx{$(CPP_SBRS)}.sbr:
   $(CPP) $(CPP_PROJ) $<  

MTL=mktyplib.exe
MTL_PROJ=
RSC=rc.exe
# ADD BASE RSC /l 0x407 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x407 /d "NDEBUG"
RSC_PROJ=/l 0x407 /fo"$(INTDIR)/AutoType.res" /d "NDEBUG" 
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/AutoType.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# SUBTRACT BASE LINK32 /pdb:none
# ADD LINK32 winmm.lib /nologo /subsystem:windows /machine:I386 /out:"WinRel/LetMeTyp.exe"
# SUBTRACT LINK32 /pdb:none
LINK32_FLAGS=winmm.lib /nologo /subsystem:windows /incremental:no\
 /pdb:"$(OUTDIR)/LetMeTyp.pdb" /machine:I386 /out:"$(OUTDIR)/LetMeTyp.exe" 
LINK32_OBJS= \
	"$(INTDIR)\AutoTdoc.obj" \
	"$(INTDIR)\AutoTvw.obj" \
	"$(INTDIR)\AutoType.obj" \
	"$(INTDIR)\AutoType.res" \
	"$(INTDIR)\charsdlg.obj" \
	"$(INTDIR)\crc16.obj" \
	"$(INTDIR)\crc32.obj" \
	"$(INTDIR)\fpwdlg.obj" \
	"$(INTDIR)\HookApp.obj" \
	"$(INTDIR)\mainfrm.obj" \
	"$(INTDIR)\newpwdlg.obj" \
	"$(INTDIR)\pwdlg.obj" \
	"$(INTDIR)\RegisterDlg.obj" \
	"$(INTDIR)\settings.obj" \
	"$(INTDIR)\soundx.obj" \
	"$(INTDIR)\Splash.obj" \
	"$(INTDIR)\stdafx.obj" \
	"$(INTDIR)\suggestw.obj" \
	".\Hook\Release\LMTHook.lib"

"$(OUTDIR)\LetMeTyp.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "Hook - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Hook\Release"
# PROP BASE Intermediate_Dir "Hook\Release"
# PROP BASE Target_Dir "Hook"
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Hook\Release"
# PROP Intermediate_Dir "Hook\Release"
# PROP Target_Dir "Hook"
OUTDIR=.\Hook\Release
INTDIR=.\Hook\Release

ALL : ".\WinRel\LMTHook.dll"

CLEAN : 
	-@erase "$(INTDIR)\Hook.obj"
	-@erase "$(OUTDIR)\LMTHook.exp"
	-@erase "$(OUTDIR)\LMTHook.lib"
	-@erase ".\WinRel\LMTHook.dll"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /c
# ADD CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /c
CPP_PROJ=/nologo /ML /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS"\
 /Fp"$(INTDIR)/Hook.pch" /YX /Fo"$(INTDIR)/" /c 
CPP_OBJS=.\Hook\Release/
CPP_SBRS=.\.

.c{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

.cpp{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

.cxx{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

.c{$(CPP_SBRS)}.sbr:
   $(CPP) $(CPP_PROJ) $<  

.cpp{$(CPP_SBRS)}.sbr:
   $(CPP) $(CPP_PROJ) $<  

.cxx{$(CPP_SBRS)}.sbr:
   $(CPP) $(CPP_PROJ) $<  

MTL=mktyplib.exe
# ADD BASE MTL /nologo /D "NDEBUG" /win32
# ADD MTL /nologo /D "NDEBUG" /win32
MTL_PROJ=/nologo /D "NDEBUG" /win32 
RSC=rc.exe
# ADD BASE RSC /l 0x407 /d "NDEBUG"
# ADD RSC /l 0x407 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/Hook.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /machine:I386
# ADD LINK32 user32.lib winmm.lib /nologo /subsystem:windows /dll /machine:I386 /out:"WinRel\LMTHook.dll"
LINK32_FLAGS=user32.lib winmm.lib /nologo /subsystem:windows /dll\
 /incremental:no /pdb:"$(OUTDIR)/LMTHook.pdb" /machine:I386\
 /def:".\Hook\Hook.def" /out:"WinRel\LMTHook.dll"\
 /implib:"$(OUTDIR)/LMTHook.lib" 
DEF_FILE= \
	".\Hook\Hook.def"
LINK32_OBJS= \
	"$(INTDIR)\Hook.obj"

".\WinRel\LMTHook.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "Hook - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Hook\Debug"
# PROP BASE Intermediate_Dir "Hook\Debug"
# PROP BASE Target_Dir "Hook"
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Hook\Debug"
# PROP Intermediate_Dir "Hook\Debug"
# PROP Target_Dir "Hook"
OUTDIR=.\Hook\Debug
INTDIR=.\Hook\Debug

ALL : ".\WinDebug\LMTHook.dll"

CLEAN : 
	-@erase "$(INTDIR)\Hook.obj"
	-@erase "$(INTDIR)\vc40.idb"
	-@erase "$(INTDIR)\vc40.pdb"
	-@erase "$(OUTDIR)\LMTHook.exp"
	-@erase "$(OUTDIR)\LMTHook.lib"
	-@erase "$(OUTDIR)\LMTHook.pdb"
	-@erase ".\WinDebug\LMTHook.dll"
	-@erase ".\WinDebug\LMTHook.ilk"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX /c
# ADD CPP /nologo /G5 /W3 /Gm /Gi /GX /Zi /Od /Gy /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX /c
CPP_PROJ=/nologo /G5 /MLd /W3 /Gm /Gi /GX /Zi /Od /Gy /D "WIN32" /D "_DEBUG" /D\
 "_WINDOWS" /Fp"$(INTDIR)/Hook.pch" /YX /Fo"$(INTDIR)/" /Fd"$(INTDIR)/" /c 
CPP_OBJS=.\Hook\Debug/
CPP_SBRS=.\.

.c{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

.cpp{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

.cxx{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

.c{$(CPP_SBRS)}.sbr:
   $(CPP) $(CPP_PROJ) $<  

.cpp{$(CPP_SBRS)}.sbr:
   $(CPP) $(CPP_PROJ) $<  

.cxx{$(CPP_SBRS)}.sbr:
   $(CPP) $(CPP_PROJ) $<  

MTL=mktyplib.exe
# ADD BASE MTL /nologo /D "_DEBUG" /win32
# ADD MTL /nologo /D "_DEBUG" /win32
MTL_PROJ=/nologo /D "_DEBUG" /win32 
RSC=rc.exe
# ADD BASE RSC /l 0x407 /d "_DEBUG"
# ADD RSC /l 0x407 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/Hook.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /debug /machine:I386
# ADD LINK32 winmm.lib user32.lib /nologo /subsystem:windows /dll /debug /machine:I386 /out:"WinDebug\LMTHook.dll"
# SUBTRACT LINK32 /incremental:no
LINK32_FLAGS=winmm.lib user32.lib /nologo /subsystem:windows /dll\
 /incremental:yes /pdb:"$(OUTDIR)/LMTHook.pdb" /debug /machine:I386\
 /def:".\Hook\Hook.def" /out:"WinDebug\LMTHook.dll"\
 /implib:"$(OUTDIR)/LMTHook.lib" 
DEF_FILE= \
	".\Hook\Hook.def"
LINK32_OBJS= \
	"$(INTDIR)\Hook.obj"

".\WinDebug\LMTHook.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "KeyGen - Win32 Release"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "KeyGen\Release"
# PROP BASE Intermediate_Dir "KeyGen\Release"
# PROP BASE Target_Dir "KeyGen"
# PROP Use_MFC 5
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "KeyGen\Release"
# PROP Intermediate_Dir "KeyGen\Release"
# PROP Target_Dir "KeyGen"
OUTDIR=.\KeyGen\Release
INTDIR=.\KeyGen\Release

ALL : "$(OUTDIR)\KeyGen.exe"

CLEAN : 
	-@erase "$(INTDIR)\crc32.obj"
	-@erase "$(INTDIR)\KeyGen.obj"
	-@erase "$(INTDIR)\KeyGen.pch"
	-@erase "$(INTDIR)\KeyGen.res"
	-@erase "$(INTDIR)\KeyGenDlg.obj"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(OUTDIR)\KeyGen.exe"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Yu"stdafx.h" /c
# ADD CPP /nologo /MT /W3 /Gi /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /Yu"stdafx.h" /c
CPP_PROJ=/nologo /MT /W3 /Gi /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D\
 "_MBCS" /Fp"$(INTDIR)/KeyGen.pch" /Yu"stdafx.h" /Fo"$(INTDIR)/" /Fd"$(INTDIR)/"\
 /c 
CPP_OBJS=.\KeyGen\Release/
CPP_SBRS=.\.

.c{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

.cpp{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

.cxx{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

.c{$(CPP_SBRS)}.sbr:
   $(CPP) $(CPP_PROJ) $<  

.cpp{$(CPP_SBRS)}.sbr:
   $(CPP) $(CPP_PROJ) $<  

.cxx{$(CPP_SBRS)}.sbr:
   $(CPP) $(CPP_PROJ) $<  

MTL=mktyplib.exe
# ADD BASE MTL /nologo /D "NDEBUG" /win32
# ADD MTL /nologo /D "NDEBUG" /win32
MTL_PROJ=/nologo /D "NDEBUG" /win32 
RSC=rc.exe
# ADD BASE RSC /l 0x407 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x407 /d "NDEBUG"
RSC_PROJ=/l 0x407 /fo"$(INTDIR)/KeyGen.res" /d "NDEBUG" 
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/KeyGen.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 /nologo /subsystem:windows /machine:I386
LINK32_FLAGS=/nologo /subsystem:windows /incremental:no\
 /pdb:"$(OUTDIR)/KeyGen.pdb" /machine:I386 /out:"$(OUTDIR)/KeyGen.exe" 
LINK32_OBJS= \
	"$(INTDIR)\crc32.obj" \
	"$(INTDIR)\KeyGen.obj" \
	"$(INTDIR)\KeyGen.res" \
	"$(INTDIR)\KeyGenDlg.obj" \
	"$(INTDIR)\StdAfx.obj"

"$(OUTDIR)\KeyGen.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "KeyGen - Win32 Debug"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "KeyGen\Debug"
# PROP BASE Intermediate_Dir "KeyGen\Debug"
# PROP BASE Target_Dir "KeyGen"
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "KeyGen\Debug"
# PROP Intermediate_Dir "KeyGen\Debug"
# PROP Target_Dir "KeyGen"
OUTDIR=.\KeyGen\Debug
INTDIR=.\KeyGen\Debug

ALL : "$(OUTDIR)\KeyGen.exe"

CLEAN : 
	-@erase "$(INTDIR)\crc32.obj"
	-@erase "$(INTDIR)\KeyGen.obj"
	-@erase "$(INTDIR)\KeyGen.pch"
	-@erase "$(INTDIR)\KeyGen.res"
	-@erase "$(INTDIR)\KeyGenDlg.obj"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\vc40.idb"
	-@erase "$(INTDIR)\vc40.pdb"
	-@erase "$(OUTDIR)\KeyGen.exe"
	-@erase "$(OUTDIR)\KeyGen.ilk"
	-@erase "$(OUTDIR)\KeyGen.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Yu"stdafx.h" /c
# ADD CPP /nologo /MDd /W3 /Gm /Gi /GX /Zi /Od /Gy /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Yu"stdafx.h" /c
CPP_PROJ=/nologo /MDd /W3 /Gm /Gi /GX /Zi /Od /Gy /D "WIN32" /D "_DEBUG" /D\
 "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Fp"$(INTDIR)/KeyGen.pch" /Yu"stdafx.h"\
 /Fo"$(INTDIR)/" /Fd"$(INTDIR)/" /c 
CPP_OBJS=.\KeyGen\Debug/
CPP_SBRS=.\.

.c{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

.cpp{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

.cxx{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

.c{$(CPP_SBRS)}.sbr:
   $(CPP) $(CPP_PROJ) $<  

.cpp{$(CPP_SBRS)}.sbr:
   $(CPP) $(CPP_PROJ) $<  

.cxx{$(CPP_SBRS)}.sbr:
   $(CPP) $(CPP_PROJ) $<  

MTL=mktyplib.exe
# ADD BASE MTL /nologo /D "_DEBUG" /win32
# ADD MTL /nologo /D "_DEBUG" /win32
MTL_PROJ=/nologo /D "_DEBUG" /win32 
RSC=rc.exe
# ADD BASE RSC /l 0x407 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x407 /d "_DEBUG" /d "_AFXDLL"
RSC_PROJ=/l 0x407 /fo"$(INTDIR)/KeyGen.res" /d "_DEBUG" /d "_AFXDLL" 
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/KeyGen.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386
# ADD LINK32 /nologo /subsystem:windows /debug /machine:I386
LINK32_FLAGS=/nologo /subsystem:windows /incremental:yes\
 /pdb:"$(OUTDIR)/KeyGen.pdb" /debug /machine:I386 /out:"$(OUTDIR)/KeyGen.exe" 
LINK32_OBJS= \
	"$(INTDIR)\crc32.obj" \
	"$(INTDIR)\KeyGen.obj" \
	"$(INTDIR)\KeyGen.res" \
	"$(INTDIR)\KeyGenDlg.obj" \
	"$(INTDIR)\StdAfx.obj"

"$(OUTDIR)\KeyGen.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ENDIF 

################################################################################
# Begin Target

# Name "AutoType - Win32 Debug"
# Name "AutoType - Win32 Release"

!IF  "$(CFG)" == "AutoType - Win32 Debug"

!ELSEIF  "$(CFG)" == "AutoType - Win32 Release"

!ENDIF 

################################################################################
# Begin Source File

SOURCE=.\stdafx.cpp
DEP_CPP_STDAF=\
	".\stdafx.h"\
	

!IF  "$(CFG)" == "AutoType - Win32 Debug"

# ADD BASE CPP /Yc"stdafx.h"
# ADD CPP /Yc"stdafx.h"

BuildCmds= \
	$(CPP) /nologo /G5 /MDd /W3 /Gm /Gi /GX /Zi /Od /Gy /D "_DEBUG" /D "WIN32" /D\
 "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Fp"$(INTDIR)/AutoType.pch" /Yc"stdafx.h"\
 /Fo"$(INTDIR)/" /Fd"$(INTDIR)/" /c $(SOURCE) \
	

"$(INTDIR)\stdafx.obj" : $(SOURCE) $(DEP_CPP_STDAF) "$(INTDIR)"
   $(BuildCmds)

"$(INTDIR)\AutoType.pch" : $(SOURCE) $(DEP_CPP_STDAF) "$(INTDIR)"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "AutoType - Win32 Release"

# ADD BASE CPP /Yc"stdafx.h"
# ADD CPP /Yc"stdafx.h"

BuildCmds= \
	$(CPP) /nologo /G5 /MT /W3 /GX /O2 /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D\
 "_MBCS" /Fp"$(INTDIR)/AutoType.pch" /Yc"stdafx.h" /Fo"$(INTDIR)/" /c $(SOURCE) \
	

"$(INTDIR)\stdafx.obj" : $(SOURCE) $(DEP_CPP_STDAF) "$(INTDIR)"
   $(BuildCmds)

"$(INTDIR)\AutoType.pch" : $(SOURCE) $(DEP_CPP_STDAF) "$(INTDIR)"
   $(BuildCmds)

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\AutoType.cpp
DEP_CPP_AUTOT=\
	".\AutoTdoc.h"\
	".\AutoTvw.h"\
	".\AutoType.h"\
	".\charsdlg.h"\
	".\crc16.h"\
	".\fpwdlg.h"\
	".\Hook\..\hook.h"\
	".\KeyGen\..\crc32.h"\
	".\mainfrm.h"\
	".\pwdlg.h"\
	".\RegisterDlg.h"\
	".\settings.h"\
	".\SortedArray.h"\
	".\soundx.h"\
	".\Splash.h"\
	".\stdafx.h"\
	".\suggestw.h"\
	

"$(INTDIR)\AutoType.obj" : $(SOURCE) $(DEP_CPP_AUTOT) "$(INTDIR)"\
 "$(INTDIR)\AutoType.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\mainfrm.cpp
DEP_CPP_MAINF=\
	".\AutoType.h"\
	".\Hook\..\hook.h"\
	".\mainfrm.h"\
	".\SortedArray.h"\
	".\Splash.h"\
	".\stdafx.h"\
	".\suggestw.h"\
	

"$(INTDIR)\mainfrm.obj" : $(SOURCE) $(DEP_CPP_MAINF) "$(INTDIR)"\
 "$(INTDIR)\AutoType.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\AutoTdoc.cpp

!IF  "$(CFG)" == "AutoType - Win32 Debug"

DEP_CPP_AUTOTD=\
	".\AutoTdoc.h"\
	".\AutoTvw.h"\
	".\AutoType.h"\
	".\crc16.h"\
	".\fpwdlg.h"\
	".\Hook\..\hook.h"\
	".\KeyGen\..\crc32.h"\
	".\SortedArray.h"\
	".\soundx.h"\
	".\stdafx.h"\
	".\suggestw.h"\
	

"$(INTDIR)\AutoTdoc.obj" : $(SOURCE) $(DEP_CPP_AUTOTD) "$(INTDIR)"\
 "$(INTDIR)\AutoType.pch"


!ELSEIF  "$(CFG)" == "AutoType - Win32 Release"

DEP_CPP_AUTOTD=\
	".\AutoTdoc.h"\
	".\AutoTvw.h"\
	".\AutoType.h"\
	".\crc16.h"\
	".\fpwdlg.h"\
	".\Hook\..\hook.h"\
	".\KeyGen\..\crc32.h"\
	".\soundx.h"\
	".\stdafx.h"\
	".\suggestw.h"\
	

"$(INTDIR)\AutoTdoc.obj" : $(SOURCE) $(DEP_CPP_AUTOTD) "$(INTDIR)"\
 "$(INTDIR)\AutoType.pch"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\AutoTvw.cpp
DEP_CPP_AUTOTV=\
	".\AutoTdoc.h"\
	".\AutoTvw.h"\
	".\AutoType.h"\
	".\Hook\..\hook.h"\
	".\SortedArray.h"\
	".\soundx.h"\
	".\stdafx.h"\
	".\suggestw.h"\
	

"$(INTDIR)\AutoTvw.obj" : $(SOURCE) $(DEP_CPP_AUTOTV) "$(INTDIR)"\
 "$(INTDIR)\AutoType.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\AutoType.rc
DEP_RSC_AUTOTY=\
	".\res\AutoType.ico"\
	".\res\AutoType.rc2"\
	".\res\TOOLBAR.bmp"\
	".\Splsh16.bmp"\
	

!IF  "$(CFG)" == "AutoType - Win32 Debug"

# ADD BASE RSC /l 0x407
# ADD RSC /l 0x407
# SUBTRACT RSC /d "_AFXDLL"

"$(INTDIR)\AutoType.res" : $(SOURCE) $(DEP_RSC_AUTOTY) "$(INTDIR)"
   $(RSC) /l 0x407 /fo"$(INTDIR)/AutoType.res" /d "_DEBUG" $(SOURCE)


!ELSEIF  "$(CFG)" == "AutoType - Win32 Release"

# ADD BASE RSC /l 0x407
# ADD RSC /l 0x407

"$(INTDIR)\AutoType.res" : $(SOURCE) $(DEP_RSC_AUTOTY) "$(INTDIR)"
   $(RSC) /l 0x407 /fo"$(INTDIR)/AutoType.res" /d "NDEBUG" $(SOURCE)


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\readme.txt

!IF  "$(CFG)" == "AutoType - Win32 Debug"

!ELSEIF  "$(CFG)" == "AutoType - Win32 Release"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\HookApp.cpp
DEP_CPP_HOOKA=\
	".\AutoType.h"\
	".\Hook\..\hook.h"\
	".\stdafx.h"\
	

"$(INTDIR)\HookApp.obj" : $(SOURCE) $(DEP_CPP_HOOKA) "$(INTDIR)"\
 "$(INTDIR)\AutoType.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\soundx.c
DEP_CPP_SOUND=\
	".\soundx.h"\
	

!IF  "$(CFG)" == "AutoType - Win32 Debug"

# PROP Exclude_From_Build 0
# SUBTRACT CPP /Yu

"$(INTDIR)\soundx.obj" : $(SOURCE) $(DEP_CPP_SOUND) "$(INTDIR)"
   $(CPP) /nologo /G5 /MDd /W3 /Gm /Gi /GX /Zi /Od /Gy /D "_DEBUG" /D "WIN32"\
 /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Fo"$(INTDIR)/" /Fd"$(INTDIR)/" /c\
 $(SOURCE)


!ELSEIF  "$(CFG)" == "AutoType - Win32 Release"

# SUBTRACT CPP /YX /Yc /Yu

"$(INTDIR)\soundx.obj" : $(SOURCE) $(DEP_CPP_SOUND) "$(INTDIR)"
   $(CPP) /nologo /G5 /MT /W3 /GX /O2 /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D\
 "_MBCS" /Fo"$(INTDIR)/" /c $(SOURCE)


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\suggestw.cpp
DEP_CPP_SUGGE=\
	".\AutoType.h"\
	".\Hook\..\hook.h"\
	".\SortedArray.h"\
	".\stdafx.h"\
	".\suggestw.h"\
	

"$(INTDIR)\suggestw.obj" : $(SOURCE) $(DEP_CPP_SUGGE) "$(INTDIR)"\
 "$(INTDIR)\AutoType.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\settings.cpp
DEP_CPP_SETTI=\
	".\AutoTdoc.h"\
	".\AutoType.h"\
	".\Hook\..\hook.h"\
	".\settings.h"\
	".\SortedArray.h"\
	".\soundx.h"\
	".\stdafx.h"\
	".\suggestw.h"\
	

"$(INTDIR)\settings.obj" : $(SOURCE) $(DEP_CPP_SETTI) "$(INTDIR)"\
 "$(INTDIR)\AutoType.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\pwdlg.cpp
DEP_CPP_PWDLG=\
	".\AutoType.h"\
	".\crc16.h"\
	".\Hook\..\hook.h"\
	".\newpwdlg.h"\
	".\pwdlg.h"\
	".\stdafx.h"\
	

"$(INTDIR)\pwdlg.obj" : $(SOURCE) $(DEP_CPP_PWDLG) "$(INTDIR)"\
 "$(INTDIR)\AutoType.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\newpwdlg.cpp
DEP_CPP_NEWPW=\
	".\AutoTdoc.h"\
	".\AutoType.h"\
	".\Hook\..\hook.h"\
	".\newpwdlg.h"\
	".\SortedArray.h"\
	".\soundx.h"\
	".\stdafx.h"\
	".\suggestw.h"\
	

"$(INTDIR)\newpwdlg.obj" : $(SOURCE) $(DEP_CPP_NEWPW) "$(INTDIR)"\
 "$(INTDIR)\AutoType.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\charsdlg.cpp
DEP_CPP_CHARS=\
	".\AutoTdoc.h"\
	".\AutoType.h"\
	".\charsdlg.h"\
	".\Hook\..\hook.h"\
	".\SortedArray.h"\
	".\soundx.h"\
	".\stdafx.h"\
	".\suggestw.h"\
	

"$(INTDIR)\charsdlg.obj" : $(SOURCE) $(DEP_CPP_CHARS) "$(INTDIR)"\
 "$(INTDIR)\AutoType.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\fpwdlg.cpp
DEP_CPP_FPWDL=\
	".\AutoType.h"\
	".\fpwdlg.h"\
	".\Hook\..\hook.h"\
	".\stdafx.h"\
	

"$(INTDIR)\fpwdlg.obj" : $(SOURCE) $(DEP_CPP_FPWDL) "$(INTDIR)"\
 "$(INTDIR)\AutoType.pch"


# End Source File
################################################################################
# Begin Project Dependency

# Project_Dep_Name "Hook"

!IF  "$(CFG)" == "AutoType - Win32 Debug"

"Hook - Win32 Debug" : 
   $(MAKE) /$(MAKEFLAGS) /F ".\AutoType.mak" CFG="Hook - Win32 Debug" 

!ELSEIF  "$(CFG)" == "AutoType - Win32 Release"

"Hook - Win32 Release" : 
   $(MAKE) /$(MAKEFLAGS) /F ".\AutoType.mak" CFG="Hook - Win32 Release" 

!ENDIF 

# End Project Dependency
################################################################################
# Begin Source File

SOURCE=.\crc16.c

!IF  "$(CFG)" == "AutoType - Win32 Debug"

# SUBTRACT CPP /YX /Yc /Yu

"$(INTDIR)\crc16.obj" : $(SOURCE) "$(INTDIR)"
   $(CPP) /nologo /G5 /MDd /W3 /Gm /Gi /GX /Zi /Od /Gy /D "_DEBUG" /D "WIN32"\
 /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Fo"$(INTDIR)/" /Fd"$(INTDIR)/" /c\
 $(SOURCE)


!ELSEIF  "$(CFG)" == "AutoType - Win32 Release"

# SUBTRACT CPP /YX /Yc /Yu

"$(INTDIR)\crc16.obj" : $(SOURCE) "$(INTDIR)"
   $(CPP) /nologo /G5 /MT /W3 /GX /O2 /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D\
 "_MBCS" /Fo"$(INTDIR)/" /c $(SOURCE)


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\crc32.c
DEP_CPP_CRC32=\
	".\KeyGen\..\crc32.h"\
	

!IF  "$(CFG)" == "AutoType - Win32 Debug"

# SUBTRACT CPP /YX /Yc /Yu

"$(INTDIR)\crc32.obj" : $(SOURCE) $(DEP_CPP_CRC32) "$(INTDIR)"
   $(CPP) /nologo /G5 /MDd /W3 /Gm /Gi /GX /Zi /Od /Gy /D "_DEBUG" /D "WIN32"\
 /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Fo"$(INTDIR)/" /Fd"$(INTDIR)/" /c\
 $(SOURCE)


!ELSEIF  "$(CFG)" == "AutoType - Win32 Release"

# SUBTRACT CPP /YX /Yc /Yu

"$(INTDIR)\crc32.obj" : $(SOURCE) $(DEP_CPP_CRC32) "$(INTDIR)"
   $(CPP) /nologo /G5 /MT /W3 /GX /O2 /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D\
 "_MBCS" /Fo"$(INTDIR)/" /c $(SOURCE)


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Splash.cpp
DEP_CPP_SPLAS=\
	".\mainfrm.h"\
	".\Splash.h"\
	".\stdafx.h"\
	

"$(INTDIR)\Splash.obj" : $(SOURCE) $(DEP_CPP_SPLAS) "$(INTDIR)"\
 "$(INTDIR)\AutoType.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\RegisterDlg.cpp
DEP_CPP_REGIS=\
	".\AutoType.h"\
	".\Hook\..\hook.h"\
	".\RegisterDlg.h"\
	".\stdafx.h"\
	

"$(INTDIR)\RegisterDlg.obj" : $(SOURCE) $(DEP_CPP_REGIS) "$(INTDIR)"\
 "$(INTDIR)\AutoType.pch"


# End Source File
# End Target
################################################################################
# Begin Target

# Name "Hook - Win32 Release"
# Name "Hook - Win32 Debug"

!IF  "$(CFG)" == "Hook - Win32 Release"

!ELSEIF  "$(CFG)" == "Hook - Win32 Debug"

!ENDIF 

################################################################################
# Begin Source File

SOURCE=.\Hook\Hook.c

!IF  "$(CFG)" == "Hook - Win32 Release"

DEP_CPP_HOOK_=\
	".\Hook\..\hook.h"\
	

"$(INTDIR)\Hook.obj" : $(SOURCE) $(DEP_CPP_HOOK_) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Hook - Win32 Debug"

DEP_CPP_HOOK_=\
	".\Hook\..\hook.h"\
	

"$(INTDIR)\Hook.obj" : $(SOURCE) $(DEP_CPP_HOOK_) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Hook\Hook.def

!IF  "$(CFG)" == "Hook - Win32 Release"

!ELSEIF  "$(CFG)" == "Hook - Win32 Debug"

!ENDIF 

# End Source File
# End Target
################################################################################
# Begin Target

# Name "KeyGen - Win32 Release"
# Name "KeyGen - Win32 Debug"

!IF  "$(CFG)" == "KeyGen - Win32 Release"

!ELSEIF  "$(CFG)" == "KeyGen - Win32 Debug"

!ENDIF 

################################################################################
# Begin Source File

SOURCE=.\KeyGen\ReadMe.txt

!IF  "$(CFG)" == "KeyGen - Win32 Release"

!ELSEIF  "$(CFG)" == "KeyGen - Win32 Debug"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\KeyGen\KeyGen.cpp
DEP_CPP_KEYGE=\
	".\KeyGen\..\crc32.h"\
	".\KeyGen\KeyGen.h"\
	".\KeyGen\KeyGenDlg.h"\
	".\KeyGen\StdAfx.h"\
	

"$(INTDIR)\KeyGen.obj" : $(SOURCE) $(DEP_CPP_KEYGE) "$(INTDIR)"\
 "$(INTDIR)\KeyGen.pch"
   $(CPP) $(CPP_PROJ) $(SOURCE)


# End Source File
################################################################################
# Begin Source File

SOURCE=.\KeyGen\KeyGenDlg.cpp
DEP_CPP_KEYGEN=\
	".\KeyGen\..\crc32.h"\
	".\KeyGen\KeyGen.h"\
	".\KeyGen\KeyGenDlg.h"\
	".\KeyGen\StdAfx.h"\
	

"$(INTDIR)\KeyGenDlg.obj" : $(SOURCE) $(DEP_CPP_KEYGEN) "$(INTDIR)"\
 "$(INTDIR)\KeyGen.pch"
   $(CPP) $(CPP_PROJ) $(SOURCE)


# End Source File
################################################################################
# Begin Source File

SOURCE=.\KeyGen\StdAfx.cpp
DEP_CPP_STDAFX=\
	".\KeyGen\StdAfx.h"\
	

!IF  "$(CFG)" == "KeyGen - Win32 Release"

# ADD CPP /Yc"stdafx.h"

BuildCmds= \
	$(CPP) /nologo /MT /W3 /Gi /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D\
 "_MBCS" /Fp"$(INTDIR)/KeyGen.pch" /Yc"stdafx.h" /Fo"$(INTDIR)/" /Fd"$(INTDIR)/"\
 /c $(SOURCE) \
	

"$(INTDIR)\StdAfx.obj" : $(SOURCE) $(DEP_CPP_STDAFX) "$(INTDIR)"
   $(BuildCmds)

"$(INTDIR)\KeyGen.pch" : $(SOURCE) $(DEP_CPP_STDAFX) "$(INTDIR)"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "KeyGen - Win32 Debug"

# ADD CPP /Yc"stdafx.h"

BuildCmds= \
	$(CPP) /nologo /MDd /W3 /Gm /Gi /GX /Zi /Od /Gy /D "WIN32" /D "_DEBUG" /D\
 "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Fp"$(INTDIR)/KeyGen.pch" /Yc"stdafx.h"\
 /Fo"$(INTDIR)/" /Fd"$(INTDIR)/" /c $(SOURCE) \
	

"$(INTDIR)\StdAfx.obj" : $(SOURCE) $(DEP_CPP_STDAFX) "$(INTDIR)"
   $(BuildCmds)

"$(INTDIR)\KeyGen.pch" : $(SOURCE) $(DEP_CPP_STDAFX) "$(INTDIR)"
   $(BuildCmds)

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\KeyGen\KeyGen.rc
DEP_RSC_KEYGEN_=\
	".\KeyGen\res\KeyGen.ico"\
	".\KeyGen\res\KeyGen.rc2"\
	

!IF  "$(CFG)" == "KeyGen - Win32 Release"


"$(INTDIR)\KeyGen.res" : $(SOURCE) $(DEP_RSC_KEYGEN_) "$(INTDIR)"
   $(RSC) /l 0x407 /fo"$(INTDIR)/KeyGen.res" /i "KeyGen" /d "NDEBUG" $(SOURCE)


!ELSEIF  "$(CFG)" == "KeyGen - Win32 Debug"


"$(INTDIR)\KeyGen.res" : $(SOURCE) $(DEP_RSC_KEYGEN_) "$(INTDIR)"
   $(RSC) /l 0x407 /fo"$(INTDIR)/KeyGen.res" /i "KeyGen" /d "_DEBUG" /d\
 "_AFXDLL" $(SOURCE)


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\crc32.c
DEP_CPP_CRC32=\
	".\KeyGen\..\crc32.h"\
	

!IF  "$(CFG)" == "KeyGen - Win32 Release"

# SUBTRACT CPP /YX /Yc /Yu

"$(INTDIR)\crc32.obj" : $(SOURCE) $(DEP_CPP_CRC32) "$(INTDIR)"
   $(CPP) /nologo /MT /W3 /Gi /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D\
 "_MBCS" /Fo"$(INTDIR)/" /Fd"$(INTDIR)/" /c $(SOURCE)


!ELSEIF  "$(CFG)" == "KeyGen - Win32 Debug"

# SUBTRACT CPP /YX /Yc /Yu

"$(INTDIR)\crc32.obj" : $(SOURCE) $(DEP_CPP_CRC32) "$(INTDIR)"
   $(CPP) /nologo /MDd /W3 /Gm /Gi /GX /Zi /Od /Gy /D "WIN32" /D "_DEBUG" /D\
 "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Fo"$(INTDIR)/" /Fd"$(INTDIR)/" /c $(SOURCE)


!ENDIF 

# End Source File
# End Target
# End Project
################################################################################
################################################################################
# Section AutoType : {C9A05F30-4892-11D0-B6AF-444553540000}
# 	0:8:Splash.h:C:\MSDev\AutoType\Splash.h
# 	0:10:Splash.cpp:C:\MSDev\AutoType\Splash.cpp
# 	1:10:IDB_SPLASH:102
# 	2:10:ResHdrName:resource.h
# 	2:11:ProjHdrName:stdafx.h
# 	2:10:WrapperDef:_SPLASH_SCRN_
# 	2:12:SplClassName:CSplashWnd
# 	2:21:SplashScreenInsertKey:4.0
# 	2:10:HeaderName:Splash.h
# 	2:10:ImplemName:Splash.cpp
# 	2:7:BmpID16:IDB_SPLASH
# End Section
################################################################################
