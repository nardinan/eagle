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
CND_CONF=Release
CND_DISTDIR=dist
CND_BUILDDIR=build

# Include project Makefile
include Makefile

# Object Directory
OBJECTDIR=${CND_BUILDDIR}/${CND_CONF}/${CND_PLATFORM}

# Object Files
OBJECTFILES= \
	${OBJECTDIR}/source/ground/structures/entity/effects/fx/cfx.o \
	${OBJECTDIR}/source/ground/structures/entity/centity.o \
	${OBJECTDIR}/source/ground/structures/entity/effects/ceffects.o \
	${OBJECTDIR}/source/environment/countdown/ccountdown.o \
	${OBJECTDIR}/source/ground/structures/animation/canimation.o


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
LDLIBSOPTIONS=

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libfrontend.${CND_DLIB_EXT}

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libfrontend.${CND_DLIB_EXT}: ${OBJECTFILES}
	${MKDIR} -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${LINK.cc} -o ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libfrontend.${CND_DLIB_EXT} ${OBJECTFILES} ${LDLIBSOPTIONS} -shared

${OBJECTDIR}/source/ground/structures/entity/effects/fx/cfx.o: source/ground/structures/entity/effects/fx/cfx.cpp 
	${MKDIR} -p ${OBJECTDIR}/source/ground/structures/entity/effects/fx
	${RM} $@.d
	$(COMPILE.cc) -O2  -MMD -MP -MF $@.d -o ${OBJECTDIR}/source/ground/structures/entity/effects/fx/cfx.o source/ground/structures/entity/effects/fx/cfx.cpp

${OBJECTDIR}/source/ground/structures/entity/centity.o: source/ground/structures/entity/centity.cpp 
	${MKDIR} -p ${OBJECTDIR}/source/ground/structures/entity
	${RM} $@.d
	$(COMPILE.cc) -O2  -MMD -MP -MF $@.d -o ${OBJECTDIR}/source/ground/structures/entity/centity.o source/ground/structures/entity/centity.cpp

${OBJECTDIR}/source/ground/structures/entity/effects/ceffects.o: source/ground/structures/entity/effects/ceffects.cpp 
	${MKDIR} -p ${OBJECTDIR}/source/ground/structures/entity/effects
	${RM} $@.d
	$(COMPILE.cc) -O2  -MMD -MP -MF $@.d -o ${OBJECTDIR}/source/ground/structures/entity/effects/ceffects.o source/ground/structures/entity/effects/ceffects.cpp

${OBJECTDIR}/source/environment/countdown/ccountdown.o: source/environment/countdown/ccountdown.cpp 
	${MKDIR} -p ${OBJECTDIR}/source/environment/countdown
	${RM} $@.d
	$(COMPILE.cc) -O2  -MMD -MP -MF $@.d -o ${OBJECTDIR}/source/environment/countdown/ccountdown.o source/environment/countdown/ccountdown.cpp

${OBJECTDIR}/source/ground/structures/animation/canimation.o: source/ground/structures/animation/canimation.cpp 
	${MKDIR} -p ${OBJECTDIR}/source/ground/structures/animation
	${RM} $@.d
	$(COMPILE.cc) -O2  -MMD -MP -MF $@.d -o ${OBJECTDIR}/source/ground/structures/animation/canimation.o source/ground/structures/animation/canimation.cpp

# Subprojects
.build-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${CND_BUILDDIR}/${CND_CONF}
	${RM} ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libfrontend.${CND_DLIB_EXT}

# Subprojects
.clean-subprojects:

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
