# Microsoft Developer Studio Generated NMAKE File, Format Version 4.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Console Application" 0x0103

!IF "$(CFG)" == ""
CFG=emath - Win32 Debug
!MESSAGE No configuration specified.  Defaulting to emath - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "emath - Win32 Release" && "$(CFG)" != "emath - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE on this makefile
!MESSAGE by defining the macro CFG on the command line.  For example:
!MESSAGE 
!MESSAGE NMAKE /f "emath.mak" CFG="emath - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "emath - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "emath - Win32 Debug" (based on "Win32 (x86) Console Application")
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
# PROP Target_Last_Scanned "emath - Win32 Debug"
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "emath - Win32 Release"

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
OUTDIR=.\Release
INTDIR=.\Release

ALL : "$(OUTDIR)\emath.exe"

CLEAN : 
	-@erase ".\Release\emath.exe"
	-@erase ".\Release\imatrices.obj"
	-@erase ".\Release\parse.obj"
	-@erase ".\Release\points.obj"
	-@erase ".\Release\cylgeom.obj"
	-@erase ".\Release\post.obj"
	-@erase ".\Release\gauss.obj"
	-@erase ".\Release\linearc.obj"
	-@erase ".\Release\mickey.obj"
	-@erase ".\Release\do_contact.obj"
	-@erase ".\Release\mcheck.obj"
	-@erase ".\Release\memstat.obj"
	-@erase ".\Release\contactc.obj"
	-@erase ".\Release\solve.obj"
	-@erase ".\Release\scale.obj"
	-@erase ".\Release\tril.obj"
	-@erase ".\Release\singular.obj"
	-@erase ".\Release\restr.obj"
	-@erase ".\Release\matrices.obj"
	-@erase ".\Release\matrix.obj"
	-@erase ".\Release\elements.obj"
	-@erase ".\Release\m_io.obj"
	-@erase ".\Release\do_linear.obj"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /YX /c
# ADD CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /YX /c
CPP_PROJ=/nologo /ML /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE"\
 /Fp"$(INTDIR)/emath.pch" /YX /Fo"$(INTDIR)/" /c 
CPP_OBJS=.\Release/
CPP_SBRS=
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/emath.bsc" 
BSC32_SBRS=
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib\
 advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib\
 odbccp32.lib /nologo /subsystem:console /incremental:no\
 /pdb:"$(OUTDIR)/emath.pdb" /machine:I386 /out:"$(OUTDIR)/emath.exe" 
LINK32_OBJS= \
	"$(INTDIR)/imatrices.obj" \
	"$(INTDIR)/parse.obj" \
	"$(INTDIR)/points.obj" \
	"$(INTDIR)/cylgeom.obj" \
	"$(INTDIR)/post.obj" \
	"$(INTDIR)/gauss.obj" \
	"$(INTDIR)/linearc.obj" \
	"$(INTDIR)/mickey.obj" \
	"$(INTDIR)/do_contact.obj" \
	"$(INTDIR)/mcheck.obj" \
	"$(INTDIR)/memstat.obj" \
	"$(INTDIR)/contactc.obj" \
	"$(INTDIR)/solve.obj" \
	"$(INTDIR)/scale.obj" \
	"$(INTDIR)/tril.obj" \
	"$(INTDIR)/singular.obj" \
	"$(INTDIR)/restr.obj" \
	"$(INTDIR)/matrices.obj" \
	"$(INTDIR)/matrix.obj" \
	"$(INTDIR)/elements.obj" \
	"$(INTDIR)/m_io.obj" \
	"$(INTDIR)/do_linear.obj"

"$(OUTDIR)\emath.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "emath - Win32 Debug"

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
OUTDIR=.\Debug
INTDIR=.\Debug

ALL : "$(OUTDIR)\emath.exe"

CLEAN : 
	-@erase ".\Debug\vc40.pdb"
	-@erase ".\Debug\vc40.idb"
	-@erase ".\Debug\emath.exe"
	-@erase ".\Debug\linearc.obj"
	-@erase ".\Debug\contactc.obj"
	-@erase ".\Debug\mickey.obj"
	-@erase ".\Debug\cylgeom.obj"
	-@erase ".\Debug\scale.obj"
	-@erase ".\Debug\singular.obj"
	-@erase ".\Debug\matrices.obj"
	-@erase ".\Debug\mcheck.obj"
	-@erase ".\Debug\m_io.obj"
	-@erase ".\Debug\restr.obj"
	-@erase ".\Debug\matrix.obj"
	-@erase ".\Debug\memstat.obj"
	-@erase ".\Debug\do_contact.obj"
	-@erase ".\Debug\parse.obj"
	-@erase ".\Debug\tril.obj"
	-@erase ".\Debug\post.obj"
	-@erase ".\Debug\elements.obj"
	-@erase ".\Debug\solve.obj"
	-@erase ".\Debug\points.obj"
	-@erase ".\Debug\do_linear.obj"
	-@erase ".\Debug\gauss.obj"
	-@erase ".\Debug\imatrices.obj"
	-@erase ".\Debug\emath.ilk"
	-@erase ".\Debug\emath.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

# ADD BASE CPP /nologo /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /YX /c
# ADD CPP /nologo /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /YX /c
CPP_PROJ=/nologo /MLd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE"\
 /Fp"$(INTDIR)/emath.pch" /YX /Fo"$(INTDIR)/" /Fd"$(INTDIR)/" /c 
CPP_OBJS=.\Debug/
CPP_SBRS=
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/emath.bsc" 
BSC32_SBRS=
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib\
 advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib\
 odbccp32.lib /nologo /subsystem:console /incremental:yes\
 /pdb:"$(OUTDIR)/emath.pdb" /debug /machine:I386 /out:"$(OUTDIR)/emath.exe" 
LINK32_OBJS= \
	"$(INTDIR)/linearc.obj" \
	"$(INTDIR)/contactc.obj" \
	"$(INTDIR)/mickey.obj" \
	"$(INTDIR)/cylgeom.obj" \
	"$(INTDIR)/scale.obj" \
	"$(INTDIR)/singular.obj" \
	"$(INTDIR)/matrices.obj" \
	"$(INTDIR)/mcheck.obj" \
	"$(INTDIR)/m_io.obj" \
	"$(INTDIR)/restr.obj" \
	"$(INTDIR)/matrix.obj" \
	"$(INTDIR)/memstat.obj" \
	"$(INTDIR)/do_contact.obj" \
	"$(INTDIR)/parse.obj" \
	"$(INTDIR)/tril.obj" \
	"$(INTDIR)/post.obj" \
	"$(INTDIR)/elements.obj" \
	"$(INTDIR)/solve.obj" \
	"$(INTDIR)/points.obj" \
	"$(INTDIR)/do_linear.obj" \
	"$(INTDIR)/gauss.obj" \
	"$(INTDIR)/imatrices.obj"

"$(OUTDIR)\emath.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ENDIF 

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

################################################################################
# Begin Target

# Name "emath - Win32 Release"
# Name "emath - Win32 Debug"

!IF  "$(CFG)" == "emath - Win32 Release"

!ELSEIF  "$(CFG)" == "emath - Win32 Debug"

!ENDIF 

################################################################################
# Begin Source File

SOURCE=.\solve.c

!IF  "$(CFG)" == "emath - Win32 Release"

DEP_CPP_SOLVE=\
	{$(INCLUDE)}"\globals.h"\
	{$(INCLUDE)}"\solve.h"\
	{$(INCLUDE)}"\m_io.h"\
	{$(INCLUDE)}"\imatrices.h"\
	{$(INCLUDE)}"\matrices.h"\
	{$(INCLUDE)}"\points.h"\
	{$(INCLUDE)}"\elements.h"\
	{$(INCLUDE)}"\restr.h"\
	

"$(INTDIR)\solve.obj" : $(SOURCE) $(DEP_CPP_SOLVE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "emath - Win32 Debug"

DEP_CPP_SOLVE=\
	{$(INCLUDE)}"\globals.h"\
	{$(INCLUDE)}"\solve.h"\
	{$(INCLUDE)}"\m_io.h"\
	{$(INCLUDE)}"\imatrices.h"\
	{$(INCLUDE)}"\matrices.h"\
	{$(INCLUDE)}"\points.h"\
	{$(INCLUDE)}"\restr.h"\
	

"$(INTDIR)\solve.obj" : $(SOURCE) $(DEP_CPP_SOLVE) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\do_contact.c

!IF  "$(CFG)" == "emath - Win32 Release"

DEP_CPP_DO_CO=\
	{$(INCLUDE)}"\globals.h"\
	{$(INCLUDE)}"\parse.h"\
	{$(INCLUDE)}"\m_io.h"\
	{$(INCLUDE)}"\contact.h"\
	{$(INCLUDE)}"\solve.h"\
	{$(INCLUDE)}"\matrices.h"\
	{$(INCLUDE)}"\imatrices.h"\
	{$(INCLUDE)}"\post.h"\
	{$(INCLUDE)}"\memstat.h"\
	{$(INCLUDE)}"\points.h"\
	{$(INCLUDE)}"\elements.h"\
	{$(INCLUDE)}"\restr.h"\
	

"$(INTDIR)\do_contact.obj" : $(SOURCE) $(DEP_CPP_DO_CO) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "emath - Win32 Debug"

DEP_CPP_DO_CO=\
	{$(INCLUDE)}"\globals.h"\
	{$(INCLUDE)}"\parse.h"\
	{$(INCLUDE)}"\m_io.h"\
	{$(INCLUDE)}"\contact.h"\
	{$(INCLUDE)}"\solve.h"\
	{$(INCLUDE)}"\matrices.h"\
	{$(INCLUDE)}"\imatrices.h"\
	{$(INCLUDE)}"\post.h"\
	{$(INCLUDE)}"\memstat.h"\
	{$(INCLUDE)}"\points.h"\
	{$(INCLUDE)}"\restr.h"\
	

"$(INTDIR)\do_contact.obj" : $(SOURCE) $(DEP_CPP_DO_CO) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\do_linear.c

!IF  "$(CFG)" == "emath - Win32 Release"

DEP_CPP_DO_LI=\
	{$(INCLUDE)}"\globals.h"\
	{$(INCLUDE)}"\parse.h"\
	{$(INCLUDE)}"\m_io.h"\
	{$(INCLUDE)}"\linear.h"\
	{$(INCLUDE)}"\solve.h"\
	{$(INCLUDE)}"\matrices.h"\
	{$(INCLUDE)}"\post.h"\
	{$(INCLUDE)}"\memstat.h"\
	{$(INCLUDE)}"\scale.h"\
	{$(INCLUDE)}"\points.h"\
	{$(INCLUDE)}"\elements.h"\
	{$(INCLUDE)}"\restr.h"\
	{$(INCLUDE)}"\imatrices.h"\
	

"$(INTDIR)\do_linear.obj" : $(SOURCE) $(DEP_CPP_DO_LI) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "emath - Win32 Debug"

DEP_CPP_DO_LI=\
	{$(INCLUDE)}"\globals.h"\
	{$(INCLUDE)}"\parse.h"\
	{$(INCLUDE)}"\m_io.h"\
	{$(INCLUDE)}"\linear.h"\
	{$(INCLUDE)}"\solve.h"\
	{$(INCLUDE)}"\matrices.h"\
	{$(INCLUDE)}"\post.h"\
	{$(INCLUDE)}"\memstat.h"\
	{$(INCLUDE)}"\scale.h"\
	{$(INCLUDE)}"\imatrices.h"\
	{$(INCLUDE)}"\points.h"\
	{$(INCLUDE)}"\restr.h"\
	

"$(INTDIR)\do_linear.obj" : $(SOURCE) $(DEP_CPP_DO_LI) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\gauss.c

!IF  "$(CFG)" == "emath - Win32 Release"

DEP_CPP_GAUSS=\
	{$(INCLUDE)}"\globals.h"\
	{$(INCLUDE)}"\gauss.h"\
	{$(INCLUDE)}"\elements.h"\
	{$(INCLUDE)}"\matrices.h"\
	{$(INCLUDE)}"\points.h"\
	{$(INCLUDE)}"\restr.h"\
	

"$(INTDIR)\gauss.obj" : $(SOURCE) $(DEP_CPP_GAUSS) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "emath - Win32 Debug"

DEP_CPP_GAUSS=\
	{$(INCLUDE)}"\globals.h"\
	{$(INCLUDE)}"\gauss.h"\
	{$(INCLUDE)}"\elements.h"\
	

"$(INTDIR)\gauss.obj" : $(SOURCE) $(DEP_CPP_GAUSS) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\linearc.c

!IF  "$(CFG)" == "emath - Win32 Release"

DEP_CPP_LINEA=\
	{$(INCLUDE)}"\globals.h"\
	{$(INCLUDE)}"\matrices.h"\
	{$(INCLUDE)}"\tril.h"\
	{$(INCLUDE)}"\singular.h"\
	{$(INCLUDE)}"\m_io.h"\
	{$(INCLUDE)}"\fsol3d.h"\
	{$(INCLUDE)}"\memstat.h"\
	{$(INCLUDE)}"\linear.h"\
	{$(INCLUDE)}"\points.h"\
	{$(INCLUDE)}"\elements.h"\
	{$(INCLUDE)}"\restr.h"\
	

"$(INTDIR)\linearc.obj" : $(SOURCE) $(DEP_CPP_LINEA) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "emath - Win32 Debug"

DEP_CPP_LINEA=\
	{$(INCLUDE)}"\globals.h"\
	{$(INCLUDE)}"\matrices.h"\
	{$(INCLUDE)}"\tril.h"\
	{$(INCLUDE)}"\singular.h"\
	{$(INCLUDE)}"\m_io.h"\
	{$(INCLUDE)}"\fsol3d.h"\
	{$(INCLUDE)}"\memstat.h"\
	{$(INCLUDE)}"\linear.h"\
	{$(INCLUDE)}"\points.h"\
	{$(INCLUDE)}"\restr.h"\
	{$(INCLUDE)}"\elements.h"\
	

"$(INTDIR)\linearc.obj" : $(SOURCE) $(DEP_CPP_LINEA) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\mcheck.c
DEP_CPP_MCHEC=\
	{$(INCLUDE)}"\globals.h"\
	{$(INCLUDE)}"\matrices.h"\
	{$(INCLUDE)}"\points.h"\
	{$(INCLUDE)}"\restr.h"\
	{$(INCLUDE)}"\scale.h"\
	{$(INCLUDE)}"\linear.h"\
	{$(INCLUDE)}"\m_io.h"\
	{$(INCLUDE)}"\gauss.h"\
	{$(INCLUDE)}"\elements.h"\
	

"$(INTDIR)\mcheck.obj" : $(SOURCE) $(DEP_CPP_MCHEC) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\mickey.c

!IF  "$(CFG)" == "emath - Win32 Release"

DEP_CPP_MICKE=\
	{$(INCLUDE)}"\globals.h"\
	{$(INCLUDE)}"\parse.h"\
	{$(INCLUDE)}"\m_io.h"\
	{$(INCLUDE)}"\linear.h"\
	{$(INCLUDE)}"\bcapp.h"\
	{$(INCLUDE)}"\solve.h"\
	{$(INCLUDE)}"\matrices.h"\
	{$(INCLUDE)}"\post.h"\
	{$(INCLUDE)}"\memstat.h"\
	{$(INCLUDE)}"\do_linear.h"\
	{$(INCLUDE)}"\do_contact.h"\
	{$(INCLUDE)}"\mcheck.h"\
	{$(INCLUDE)}"\text.h"\
	{$(INCLUDE)}"\points.h"\
	{$(INCLUDE)}"\elements.h"\
	{$(INCLUDE)}"\restr.h"\
	{$(INCLUDE)}"\imatrices.h"\
	{$(INCLUDE)}"\date_time.h"\
	

"$(INTDIR)\mickey.obj" : $(SOURCE) $(DEP_CPP_MICKE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "emath - Win32 Debug"

DEP_CPP_MICKE=\
	{$(INCLUDE)}"\globals.h"\
	{$(INCLUDE)}"\parse.h"\
	{$(INCLUDE)}"\m_io.h"\
	{$(INCLUDE)}"\linear.h"\
	{$(INCLUDE)}"\bcapp.h"\
	{$(INCLUDE)}"\solve.h"\
	{$(INCLUDE)}"\matrices.h"\
	{$(INCLUDE)}"\post.h"\
	{$(INCLUDE)}"\memstat.h"\
	{$(INCLUDE)}"\do_linear.h"\
	{$(INCLUDE)}"\do_contact.h"\
	{$(INCLUDE)}"\mcheck.h"\
	{$(INCLUDE)}"\text.h"\
	{$(INCLUDE)}"\imatrices.h"\
	{$(INCLUDE)}"\points.h"\
	{$(INCLUDE)}"\restr.h"\
	{$(INCLUDE)}"\date_time.h"\
	

"$(INTDIR)\mickey.obj" : $(SOURCE) $(DEP_CPP_MICKE) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\parse.c

!IF  "$(CFG)" == "emath - Win32 Release"

DEP_CPP_PARSE=\
	{$(INCLUDE)}"\globals.h"\
	{$(INCLUDE)}"\scale.h"\
	{$(INCLUDE)}"\cylgeom.h"\
	{$(INCLUDE)}"\m_io.h"\
	{$(INCLUDE)}"\tril.h"\
	{$(INCLUDE)}"\parse.h"\
	{$(INCLUDE)}"\matrices.h"\
	{$(INCLUDE)}"\points.h"\
	{$(INCLUDE)}"\elements.h"\
	{$(INCLUDE)}"\restr.h"\
	

"$(INTDIR)\parse.obj" : $(SOURCE) $(DEP_CPP_PARSE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "emath - Win32 Debug"

DEP_CPP_PARSE=\
	{$(INCLUDE)}"\globals.h"\
	{$(INCLUDE)}"\scale.h"\
	{$(INCLUDE)}"\cylgeom.h"\
	{$(INCLUDE)}"\m_io.h"\
	{$(INCLUDE)}"\tril.h"\
	{$(INCLUDE)}"\parse.h"\
	{$(INCLUDE)}"\elements.h"\
	{$(INCLUDE)}"\points.h"\
	{$(INCLUDE)}"\matrices.h"\
	{$(INCLUDE)}"\restr.h"\
	

"$(INTDIR)\parse.obj" : $(SOURCE) $(DEP_CPP_PARSE) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\post.c
DEP_CPP_POST_=\
	{$(INCLUDE)}"\globals.h"\
	{$(INCLUDE)}"\m_io.h"\
	{$(INCLUDE)}"\matrices.h"\
	{$(INCLUDE)}"\restr.h"\
	{$(INCLUDE)}"\points.h"\
	{$(INCLUDE)}"\elements.h"\
	{$(INCLUDE)}"\fsol3d.h"\
	{$(INCLUDE)}"\scale.h"\
	{$(INCLUDE)}"\post.h"\
	{$(INCLUDE)}"\gauss.h"\
	{$(INCLUDE)}"\contact.h"\
	

"$(INTDIR)\post.obj" : $(SOURCE) $(DEP_CPP_POST_) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\scale.c
DEP_CPP_SCALE=\
	{$(INCLUDE)}"\globals.h"\
	{$(INCLUDE)}"\points.h"\
	{$(INCLUDE)}"\restr.h"\
	{$(INCLUDE)}"\elements.h"\
	{$(INCLUDE)}"\scale.h"\
	{$(INCLUDE)}"\tril.h"\
	{$(INCLUDE)}"\matrices.h"\
	

"$(INTDIR)\scale.obj" : $(SOURCE) $(DEP_CPP_SCALE) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\singular.c
DEP_CPP_SINGU=\
	{$(INCLUDE)}"\globals.h"\
	{$(INCLUDE)}"\points.h"\
	{$(INCLUDE)}"\elements.h"\
	{$(INCLUDE)}"\tril.h"\
	{$(INCLUDE)}"\singular.h"\
	{$(INCLUDE)}"\matrices.h"\
	{$(INCLUDE)}"\restr.h"\
	

"$(INTDIR)\singular.obj" : $(SOURCE) $(DEP_CPP_SINGU) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\contactc.c

!IF  "$(CFG)" == "emath - Win32 Release"

DEP_CPP_CONTA=\
	{$(INCLUDE)}"\globals.h"\
	{$(INCLUDE)}"\matrices.h"\
	{$(INCLUDE)}"\tril.h"\
	{$(INCLUDE)}"\singular.h"\
	{$(INCLUDE)}"\m_io.h"\
	{$(INCLUDE)}"\fsol3d.h"\
	{$(INCLUDE)}"\memstat.h"\
	{$(INCLUDE)}"\contact.h"\
	{$(INCLUDE)}"\points.h"\
	{$(INCLUDE)}"\elements.h"\
	{$(INCLUDE)}"\restr.h"\
	

"$(INTDIR)\contactc.obj" : $(SOURCE) $(DEP_CPP_CONTA) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "emath - Win32 Debug"

DEP_CPP_CONTA=\
	{$(INCLUDE)}"\globals.h"\
	{$(INCLUDE)}"\matrices.h"\
	{$(INCLUDE)}"\tril.h"\
	{$(INCLUDE)}"\singular.h"\
	{$(INCLUDE)}"\m_io.h"\
	{$(INCLUDE)}"\fsol3d.h"\
	{$(INCLUDE)}"\memstat.h"\
	{$(INCLUDE)}"\contact.h"\
	{$(INCLUDE)}"\points.h"\
	{$(INCLUDE)}"\restr.h"\
	{$(INCLUDE)}"\elements.h"\
	

"$(INTDIR)\contactc.obj" : $(SOURCE) $(DEP_CPP_CONTA) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=\irix\elastomath\general\tril.c
DEP_CPP_TRIL_=\
	{$(INCLUDE)}"\globals.h"\
	{$(INCLUDE)}"\m_io.h"\
	{$(INCLUDE)}"\parse.h"\
	{$(INCLUDE)}"\matrices.h"\
	{$(INCLUDE)}"\elements.h"\
	{$(INCLUDE)}"\tril.h"\
	{$(INCLUDE)}"\points.h"\
	{$(INCLUDE)}"\restr.h"\
	

"$(INTDIR)\tril.obj" : $(SOURCE) $(DEP_CPP_TRIL_) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


# End Source File
################################################################################
# Begin Source File

SOURCE=\irix\elastomath\general\elements.c

!IF  "$(CFG)" == "emath - Win32 Release"

DEP_CPP_ELEME=\
	{$(INCLUDE)}"\globals.h"\
	{$(INCLUDE)}"\memstat.h"\
	{$(INCLUDE)}"\elements.h"\
	{$(INCLUDE)}"\m_io.h"\
	{$(INCLUDE)}"\matrices.h"\
	{$(INCLUDE)}"\points.h"\
	{$(INCLUDE)}"\restr.h"\
	

"$(INTDIR)\elements.obj" : $(SOURCE) $(DEP_CPP_ELEME) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "emath - Win32 Debug"

DEP_CPP_ELEME=\
	{$(INCLUDE)}"\globals.h"\
	{$(INCLUDE)}"\memstat.h"\
	{$(INCLUDE)}"\elements.h"\
	{$(INCLUDE)}"\m_io.h"\
	

"$(INTDIR)\elements.obj" : $(SOURCE) $(DEP_CPP_ELEME) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=\irix\elastomath\general\imatrices.c

!IF  "$(CFG)" == "emath - Win32 Release"

DEP_CPP_IMATR=\
	{$(INCLUDE)}"\globals.h"\
	{$(INCLUDE)}"\m_io.h"\
	{$(INCLUDE)}"\imatrices.h"\
	{$(INCLUDE)}"\restr.h"\
	{$(INCLUDE)}"\memstat.h"\
	{$(INCLUDE)}"\matrices.h"\
	{$(INCLUDE)}"\points.h"\
	{$(INCLUDE)}"\elements.h"\
	

"$(INTDIR)\imatrices.obj" : $(SOURCE) $(DEP_CPP_IMATR) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "emath - Win32 Debug"

DEP_CPP_IMATR=\
	{$(INCLUDE)}"\globals.h"\
	{$(INCLUDE)}"\m_io.h"\
	{$(INCLUDE)}"\imatrices.h"\
	{$(INCLUDE)}"\restr.h"\
	{$(INCLUDE)}"\memstat.h"\
	

"$(INTDIR)\imatrices.obj" : $(SOURCE) $(DEP_CPP_IMATR) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=\irix\elastomath\general\m_io.c

!IF  "$(CFG)" == "emath - Win32 Release"

DEP_CPP_M_IO_=\
	{$(INCLUDE)}"\globals.h"\
	{$(INCLUDE)}"\message.h"\
	{$(INCLUDE)}"\m_io.h"\
	{$(INCLUDE)}"\matrices.h"\
	{$(INCLUDE)}"\points.h"\
	{$(INCLUDE)}"\elements.h"\
	{$(INCLUDE)}"\restr.h"\
	

"$(INTDIR)\m_io.obj" : $(SOURCE) $(DEP_CPP_M_IO_) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "emath - Win32 Debug"

DEP_CPP_M_IO_=\
	{$(INCLUDE)}"\globals.h"\
	{$(INCLUDE)}"\message.h"\
	{$(INCLUDE)}"\m_io.h"\
	

"$(INTDIR)\m_io.obj" : $(SOURCE) $(DEP_CPP_M_IO_) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=\irix\elastomath\general\matrices.c

!IF  "$(CFG)" == "emath - Win32 Release"

DEP_CPP_MATRI=\
	{$(INCLUDE)}"\globals.h"\
	{$(INCLUDE)}"\m_io.h"\
	{$(INCLUDE)}"\matrices.h"\
	{$(INCLUDE)}"\restr.h"\
	{$(INCLUDE)}"\memstat.h"\
	{$(INCLUDE)}"\points.h"\
	{$(INCLUDE)}"\elements.h"\
	

"$(INTDIR)\matrices.obj" : $(SOURCE) $(DEP_CPP_MATRI) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "emath - Win32 Debug"

DEP_CPP_MATRI=\
	{$(INCLUDE)}"\globals.h"\
	{$(INCLUDE)}"\m_io.h"\
	{$(INCLUDE)}"\matrices.h"\
	{$(INCLUDE)}"\restr.h"\
	{$(INCLUDE)}"\memstat.h"\
	{$(INCLUDE)}"\points.h"\
	

"$(INTDIR)\matrices.obj" : $(SOURCE) $(DEP_CPP_MATRI) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=\irix\elastomath\general\matrix.c

!IF  "$(CFG)" == "emath - Win32 Release"

DEP_CPP_MATRIX=\
	{$(INCLUDE)}"\globals.h"\
	{$(INCLUDE)}"\imatrices.h"\
	{$(INCLUDE)}"\matrices.h"\
	{$(INCLUDE)}"\points.h"\
	{$(INCLUDE)}"\elements.h"\
	{$(INCLUDE)}"\restr.h"\
	

"$(INTDIR)\matrix.obj" : $(SOURCE) $(DEP_CPP_MATRIX) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "emath - Win32 Debug"

DEP_CPP_MATRIX=\
	{$(INCLUDE)}"\globals.h"\
	{$(INCLUDE)}"\imatrices.h"\
	{$(INCLUDE)}"\matrices.h"\
	{$(INCLUDE)}"\points.h"\
	{$(INCLUDE)}"\restr.h"\
	

"$(INTDIR)\matrix.obj" : $(SOURCE) $(DEP_CPP_MATRIX) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=\irix\elastomath\general\memstat.c

!IF  "$(CFG)" == "emath - Win32 Release"

DEP_CPP_MEMST=\
	{$(INCLUDE)}"\globals.h"\
	{$(INCLUDE)}"\memstat.h"\
	{$(INCLUDE)}"\sys\Types.h"\
	{$(INCLUDE)}"\matrices.h"\
	{$(INCLUDE)}"\points.h"\
	{$(INCLUDE)}"\elements.h"\
	{$(INCLUDE)}"\restr.h"\
	

"$(INTDIR)\memstat.obj" : $(SOURCE) $(DEP_CPP_MEMST) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "emath - Win32 Debug"

DEP_CPP_MEMST=\
	{$(INCLUDE)}"\globals.h"\
	{$(INCLUDE)}"\memstat.h"\
	{$(INCLUDE)}"\sys\Types.h"\
	

"$(INTDIR)\memstat.obj" : $(SOURCE) $(DEP_CPP_MEMST) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=\irix\elastomath\general\points.c

!IF  "$(CFG)" == "emath - Win32 Release"

DEP_CPP_POINT=\
	{$(INCLUDE)}"\globals.h"\
	{$(INCLUDE)}"\m_io.h"\
	{$(INCLUDE)}"\memstat.h"\
	{$(INCLUDE)}"\points.h"\
	{$(INCLUDE)}"\matrices.h"\
	{$(INCLUDE)}"\elements.h"\
	{$(INCLUDE)}"\restr.h"\
	

"$(INTDIR)\points.obj" : $(SOURCE) $(DEP_CPP_POINT) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "emath - Win32 Debug"

DEP_CPP_POINT=\
	{$(INCLUDE)}"\globals.h"\
	{$(INCLUDE)}"\m_io.h"\
	{$(INCLUDE)}"\memstat.h"\
	{$(INCLUDE)}"\points.h"\
	

"$(INTDIR)\points.obj" : $(SOURCE) $(DEP_CPP_POINT) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=\irix\elastomath\general\restr.c

!IF  "$(CFG)" == "emath - Win32 Release"

DEP_CPP_RESTR=\
	{$(INCLUDE)}"\globals.h"\
	{$(INCLUDE)}"\m_io.h"\
	{$(INCLUDE)}"\parse.h"\
	{$(INCLUDE)}"\memstat.h"\
	{$(INCLUDE)}"\restr.h"\
	{$(INCLUDE)}"\matrices.h"\
	{$(INCLUDE)}"\points.h"\
	{$(INCLUDE)}"\elements.h"\
	

"$(INTDIR)\restr.obj" : $(SOURCE) $(DEP_CPP_RESTR) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "emath - Win32 Debug"

DEP_CPP_RESTR=\
	{$(INCLUDE)}"\globals.h"\
	{$(INCLUDE)}"\m_io.h"\
	{$(INCLUDE)}"\parse.h"\
	{$(INCLUDE)}"\memstat.h"\
	{$(INCLUDE)}"\restr.h"\
	

"$(INTDIR)\restr.obj" : $(SOURCE) $(DEP_CPP_RESTR) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=\irix\elastomath\general\cylgeom.c

!IF  "$(CFG)" == "emath - Win32 Release"

DEP_CPP_CYLGE=\
	{$(INCLUDE)}"\globals.h"\
	{$(INCLUDE)}"\points.h"\
	{$(INCLUDE)}"\matrices.h"\
	{$(INCLUDE)}"\elements.h"\
	{$(INCLUDE)}"\restr.h"\
	

"$(INTDIR)\cylgeom.obj" : $(SOURCE) $(DEP_CPP_CYLGE) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "emath - Win32 Debug"

DEP_CPP_CYLGE=\
	{$(INCLUDE)}"\globals.h"\
	{$(INCLUDE)}"\points.h"\
	

"$(INTDIR)\cylgeom.obj" : $(SOURCE) $(DEP_CPP_CYLGE) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

# End Source File
# End Target
# End Project
################################################################################
