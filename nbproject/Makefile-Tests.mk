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
CND_CONF=Tests
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
	${OBJECTDIR}/src/tests/test_io.o \
	${OBJECTDIR}/src/tests/test_ioascii.o \
	${OBJECTDIR}/src/tests/test_iocantp.o \
	${OBJECTDIR}/src/tests/test_ioslip.o \
	${OBJECTDIR}/src/tests/tests.o \
	${OBJECTDIR}/src/tools/ihex.o \
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
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/iotools

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/iotools: ${OBJECTFILES}
	${MKDIR} -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${LINK.c} -o ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/iotools ${OBJECTFILES} ${LDLIBSOPTIONS}

${OBJECTDIR}/src/net/io.o: src/net/io.c 
	${MKDIR} -p ${OBJECTDIR}/src/net
	${RM} "$@.d"
	$(COMPILE.c) -g -Werror -Isrc -std=c11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/net/io.o src/net/io.c

${OBJECTDIR}/src/net/ioascii.o: src/net/ioascii.c 
	${MKDIR} -p ${OBJECTDIR}/src/net
	${RM} "$@.d"
	$(COMPILE.c) -g -Werror -Isrc -std=c11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/net/ioascii.o src/net/ioascii.c

${OBJECTDIR}/src/net/iocantp.o: src/net/iocantp.c 
	${MKDIR} -p ${OBJECTDIR}/src/net
	${RM} "$@.d"
	$(COMPILE.c) -g -Werror -Isrc -std=c11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/net/iocantp.o src/net/iocantp.c

${OBJECTDIR}/src/net/ioslip.o: src/net/ioslip.c 
	${MKDIR} -p ${OBJECTDIR}/src/net
	${RM} "$@.d"
	$(COMPILE.c) -g -Werror -Isrc -std=c11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/net/ioslip.o src/net/ioslip.c

${OBJECTDIR}/src/tests/test_io.o: src/tests/test_io.c 
	${MKDIR} -p ${OBJECTDIR}/src/tests
	${RM} "$@.d"
	$(COMPILE.c) -g -Werror -Isrc -std=c11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/tests/test_io.o src/tests/test_io.c

${OBJECTDIR}/src/tests/test_ioascii.o: src/tests/test_ioascii.c 
	${MKDIR} -p ${OBJECTDIR}/src/tests
	${RM} "$@.d"
	$(COMPILE.c) -g -Werror -Isrc -std=c11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/tests/test_ioascii.o src/tests/test_ioascii.c

${OBJECTDIR}/src/tests/test_iocantp.o: src/tests/test_iocantp.c 
	${MKDIR} -p ${OBJECTDIR}/src/tests
	${RM} "$@.d"
	$(COMPILE.c) -g -Werror -Isrc -std=c11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/tests/test_iocantp.o src/tests/test_iocantp.c

${OBJECTDIR}/src/tests/test_ioslip.o: src/tests/test_ioslip.c 
	${MKDIR} -p ${OBJECTDIR}/src/tests
	${RM} "$@.d"
	$(COMPILE.c) -g -Werror -Isrc -std=c11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/tests/test_ioslip.o src/tests/test_ioslip.c

${OBJECTDIR}/src/tests/tests.o: src/tests/tests.c 
	${MKDIR} -p ${OBJECTDIR}/src/tests
	${RM} "$@.d"
	$(COMPILE.c) -g -Werror -Isrc -std=c11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/tests/tests.o src/tests/tests.c

${OBJECTDIR}/src/tools/ihex.o: src/tools/ihex.c 
	${MKDIR} -p ${OBJECTDIR}/src/tools
	${RM} "$@.d"
	$(COMPILE.c) -g -Werror -Isrc -std=c11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/tools/ihex.o src/tools/ihex.c

${OBJECTDIR}/src/tools/queue.o: src/tools/queue.c 
	${MKDIR} -p ${OBJECTDIR}/src/tools
	${RM} "$@.d"
	$(COMPILE.c) -g -Werror -Isrc -std=c11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/tools/queue.o src/tools/queue.c

${OBJECTDIR}/src/tools/queuefifo.o: src/tools/queuefifo.c 
	${MKDIR} -p ${OBJECTDIR}/src/tools
	${RM} "$@.d"
	$(COMPILE.c) -g -Werror -Isrc -std=c11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/tools/queuefifo.o src/tools/queuefifo.c

# Subprojects
.build-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${CND_BUILDDIR}/${CND_CONF}
	${RM} ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/iotools

# Subprojects
.clean-subprojects:

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
