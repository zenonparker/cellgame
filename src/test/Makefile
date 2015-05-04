###############################################################################
#                              COMMON SETTINGS                                #
###############################################################################
EXEC_NAME = cell.test.x
CUSTOM_LD_FLAGS = -lgtest_main -lgtest
CUSTOM_CC_FLAGS = 

OBJS = \
../grid.o \
../scan.o \
../reward.o \
../rewardmanager.o \
../rewardclass.o \
cell.test.o \
grid_map.test.o \
grid_multi_map.test.o \
scan.test.o \
reward.test.o \

###############################################################################
#                            INCLUDE AND LIB DIRS                             #
###############################################################################
BOOST_DIR = /usr/local/include/boost
CELL_DIR = ${CELL_ROOT}
GTEST_DIR = ${GTEST_ROOT}

CUSTOM_INC_DIRS = \
-I${CELL_DIR}/cell \
-I${CELL_DIR}/util \
-I${CELL_DIR}/network \

###############################################################################
#                                    FLAGS                                    #
###############################################################################

SHELL = bash
CXX = clang++
CC = clang 
OPT_LEVEL = -O3
ifdef DEBUG
  OPT_LEVEL = -g
endif
CCFLAGS = -c ${OPT_LEVEL} ${CUSTOM_INC_DIRS} -I${BOOST_DIR} -I. \
					-I${GTEST_DIR}/include ${CUSTOM_CC_FLAGS} -I${CELL_DIR}/test
CXXFLAGS = $(CCFLAGS) -std=c++1y
LDFLAGS = -L/usr/local/lib

###############################################################################
#                                    RULES                                    #
###############################################################################

default: $(EXEC_NAME)

$(EXEC_NAME): $(OBJS:.o=.d) $(OBJS)
	$(CXX) $(LDFLAGS) $(OBJS) $(CUSTOM_LD_FLAGS) -o $@

-include $(OBJS:.o=.d)

%.d: %.cpp
	$(SHELL) -ec '$(CXX) -M $(CXXFLAGS) $< | sed "s|$*.o|& $@|g" > $@'

%.d: %.c
	$(SHELL) -ec '$(CC) -M $(CCFLAGS) $< | sed "s|$*.o|& $@|g" > $@'

%.o: %.cpp
	$(CXX) $(CXXFLAGS) $< -o $@

%.o: %.c
	$(CC) $(CCFLAGS) $< -o $@

clean:
	$(RM) -rf $(OBJS) $(OBJS:.o=.d) $(EXEC_NAME)

