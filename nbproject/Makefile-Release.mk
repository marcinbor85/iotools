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
CND_PLATFORM=GNU-Linux
CND_DLIB_EXT=so
CND_CONF=Release
CND_DISTDIR=dist
CND_BUILDDIR=build

# Include project Makefile
include Makefile

# Object Directory
OBJECTDIR=${CND_BUILDDIR}/${CND_CONF}/${CND_PLATFORM}

# Object Files
OBJECTFILES= \
	${OBJECTDIR}/src/net/io.o \
	${OBJECTDIR}/src/net/ioascii.o \
	${OBJECTDIR}/src/net/iocantp.o \
	${OBJECTDIR}/src/net/ioslip.o \
	${OBJECTDIR}/src/tools/queue.o \
	${OBJECTDIR}/src/tools/queuefifo.o


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
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libiotools.a

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libiotools.a: ${OBJECTFILES}
	${MKDIR} -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${RM} ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libiotools.a
	${AR} -rv ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libiotools.a ${OBJECTFILES} 
	$(RANLIB) ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libiotools.a

${OBJECTDIR}/src/net/io.o: src/net/io.c 
	${MKDIR} -p ${OBJECTDIR}/src/net
	${RM} "$@.d"
	$(COMPILE.c) -O2 -Werror -Isrc -std=c11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/net/io.o src/net/io.c

${OBJECTDIR}/src/net/ioascii.o: src/net/ioascii.c 
	${MKDIR} -p ${OBJECTDIR}/src/net
	${RM} "$@.d"
	$(COMPILE.c) -O2 -Werror -Isrc -std=c11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/net/ioascii.o src/net/ioascii.c

${OBJECTDIR}/src/net/iocantp.o: src/net/iocantp.c 
	${MKDIR} -p ${OBJECTDIR}/src/net
	${RM} "$@.d"
	$(COMPILE.c) -O2 -Werror -Isrc -std=c11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/net/iocantp.o src/net/iocantp.c

${OBJECTDIR}/src/net/ioslip.o: src/net/ioslip.c 
	${MKDIR} -p ${OBJECTDIR}/src/net
	${RM} "$@.d"
	$(COMPILE.c) -O2 -Werror -Isrc -std=c11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/net/ioslip.o src/net/ioslip.c

${OBJECTDIR}/src/tools/queue.o: src/tools/queue.c 
	${MKDIR} -p ${OBJECTDIR}/src/tools
	${RM} "$@.d"
	$(COMPILE.c) -O2 -Werror -Isrc -std=c11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/tools/queue.o src/tools/queue.c

${OBJECTDIR}/src/tools/queuefifo.o: src/tools/queuefifo.c 
	${MKDIR} -p ${OBJECTDIR}/src/tools
	${RM} "$@.d"
	$(COMPILE.c) -O2 -Werror -Isrc -std=c11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/tools/queuefifo.o src/tools/queuefifo.c

# Subprojects
.build-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${CND_BUILDDIR}/${CND_CONF}
	${RM} ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libiotools.a

# Subprojects
.clean-subprojects:

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
