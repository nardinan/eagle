#
# Generated Makefile - do not edit!
#
# Edit the Makefile in the project folder instead (../Makefile). Each target
# has a -pre and a -post target defined where you can add customized code.
#
# This makefile implements configuration specific macros and targets.


# Environment
MKDIR=mkdir
CP=cp
GREP=grep
NM=nm
CCADMIN=CCadmin
RANLIB=ranlib
CC=gcc
CCC=g++
CXX=g++
FC=gfortran
AS=as

# Macros
CND_PLATFORM=MinGW-Windows
CND_DLIB_EXT=dll
CND_CONF=Debug
CND_DISTDIR=dist
CND_BUILDDIR=build

# Include project Makefile
include Makefile

# Object Directory
OBJECTDIR=${CND_BUILDDIR}/${CND_CONF}/${CND_PLATFORM}

# Object Files
OBJECTFILES= \
	${OBJECTDIR}/main.o


# C Compiler Flags
CFLAGS=

# CC Compiler Flags
CCFLAGS=
CXXFLAGS=

# Fortran Compiler Flags
FFLAGS=

# Assembler Flags
ASFLAGS=

# Link Libraries and Options
LDLIBSOPTIONS=-L../../engine/dist/Debug/MinGW-Windows -lengine -L../../frontend/dist/Debug/MinGW-Windows -lfrontend -lopengl32 -lfreeglut -lpthread -lwsock32 -lmingw32 -lws2_32 -lopenal32

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/skeleton.exe

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/skeleton.exe: ../../engine/dist/Debug/MinGW-Windows/libengine.dll

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/skeleton.exe: ../../frontend/dist/Debug/MinGW-Windows/libfrontend.dll

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/skeleton.exe: ${OBJECTFILES}
	${MKDIR} -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${LINK.cc} -o ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/skeleton ${OBJECTFILES} ${LDLIBSOPTIONS}

${OBJECTDIR}/main.o: main.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -g -I../../engine/source -I../../engine/source/ground -I../../engine/source/ground/datafiles -I../../engine/source/ground/datafiles/filesystem -I../../engine/source/ground/datafiles/xml -I../../engine/source/ground/environment/paths -I../../engine/source/ground/environment/physics/collision -I../../engine/source/ground/environment/physics/collision/impacts -I../../engine/source/ground/networking -I../../engine/source/ground/networking/client -I../../engine/source/ground/networking/endian -I../../engine/source/ground/networking/lobby -I../../engine/source/ground/networking/server -I../../engine/source/ground/strings -I../../engine/source/ground/structures/array -I../../engine/source/ground/structures/timer -I../../engine/source/ground/support/glee -I../../engine/source/ground/system/input -I../../engine/source/ground/system/log -I../../engine/source/ground/system/math -I../../engine/source/ground/system/memory -I../../engine/source/ground/system/random -I../../engine/source/graphics -I../../engine/source/graphics/environment/lights -I../../engine/source/graphics/environment/shadow -I../../engine/source/graphics/ground/structures/model -I../../engine/source/graphics/ground/structures/particle -I../../engine/source/graphics/ground/structures/targa -I../../engine/source/graphics/ground/system/context -I../../engine/source/graphics/ground/system/environment -I../../engine/source/graphics/uid/structures/button -I../../engine/source/graphics/uid/structures/scroll -I../../engine/source/graphics/uid/structures/window -I../../engine/source/graphics/uid/structures/label -I../../engine/source/graphics/uid/structures/label/field -I../../engine/source/graphics/uid/structures/label/list -I../../engine/source/graphics/uid/structures/label/list/browser -I../../engine/source/graphics/uid/system/component -I../../engine/source/graphics/uid/system/fonts -I../../engine/source/graphics/uid/system/theme -I../../engine/source/audio -I../../engine/source/audio/structures/channels -I../../engine/source/audio/structures/wave -I../../engine/source/audio/system/sample -I../../frontend/source -I../../frontend/source/environment/countdown -I../../frontend/source/ground/structures/animation -I../../frontend/source/ground/structures/entity -I../../frontend/source/ground/structures/entity/effects -I../../frontend/source/ground/structures/entity/effects/fx -MMD -MP -MF $@.d -o ${OBJECTDIR}/main.o main.cpp

# Subprojects
.build-subprojects:
	cd ../../engine && ${MAKE}  -f Makefile CONF=Debug
	cd ../../frontend && ${MAKE}  -f Makefile CONF=Debug

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${CND_BUILDDIR}/${CND_CONF}
	${RM} ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/skeleton.exe

# Subprojects
.clean-subprojects:
	cd ../../engine && ${MAKE}  -f Makefile CONF=Debug clean
	cd ../../frontend && ${MAKE}  -f Makefile CONF=Debug clean

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
