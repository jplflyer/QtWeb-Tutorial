#======================================================================
# GNU MAKE REQUIRED!
#
# This Makefile covers Vitac-authored programs used to talk to the
# dialogic modem bank. It requires GNU Make.
#
# Possible targets:
#
#		make clean
#		make
#
# With luck, to add programs, you only need to do two things:
#
# 1. Add them to the PROGRAMS variable here at the very very top.
# 2. Define a rule near the bottom.
#
# Notice VPATH. This is a GNU Make thing that lets us put our source
# files where we want. I've broken them based on where they're running.
# I may later break them up even more for one directory for each program.
# Anyway, we search for .cpp files in the list of directories, and it
# just magically works.
#======================================================================

# This needs to be above the include, as he has targets.
.PHONY: all
all: directories DemoServer

# We include a standard base with lots of boilerplate.
include Makefile-Base

TEST_SRC=test
TEST_BIN=test-bin${MACAPPEND}

VPATH := ${SRCDIR}
INCLUDES += -I./includes
LDFLAGS += -L. -lpqxx -lpq -lz -lboost_system -l${BOOST_THREAD} -luuid -lpthread -lstdc++ -lm -ldl -lssl -lcrypto

SRCS_NOSORT = $(shell find . -name "*.cpp" -print)
SRCS = $(sort ${SRCS_NOSORT})
OBJ_LIST := $(patsubst ${SRCDIR}/%.cpp,${OBJDIR}/%.o,${SRCS})

#======================================================================
# Top-level targets.
#======================================================================

Makefile: ;

# Clean the contents of the subdirs.
.PHONY: clean
clean:
	rm -f ${DEPDIR}/* ${OBJDIR}/* ${LIB} ${TEST_BIN}/*

# Clean out the subdirs entirely.
.PHONY: reallyclean
reallyclean:
	rm -rf ${DEPDIR} ${OBJDIR}

.PHONY: echo
echo:
	@echo CXXFLAGS is ${CXXFLAGS}
	@echo SRCDIR is ${SRCDIR}
	@echo OBJDIR is ${OBJDIR}
	@echo SRCS is ${SRCS}
	@echo OBJ_LIST is ${OBJ_LIST}
	@echo VPATH = ${VPATH}

DemoServer: ${OBJ_LIST}
	g++ ${OBJ_LIST} ${LDFLAGS} -o DemoServer
