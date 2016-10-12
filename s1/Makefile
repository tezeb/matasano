CXX=g++
CXXFLAGS=--std=c++11 -Wall -Wextra -Werror -pedantic -pedantic-errors -Wshadow

CXXFLAGS+=-I../common

SRC := $(sort $(wildcard c?.cpp))
OBJ := $(addsuffix .o,$(basename $(SRC)))
EXE := $(SRC:c%.cpp=chal%)

COMMON = ../common/utils.cpp
COMMON_OBJ = $(addsuffix .o,$(basename $(COMMON)))

chal7: LIBS = -lcrypto

.SECONDARY: $(addprefix chal,$(TARGETS))

all: $(EXE)

run: $(EXE:chal%=run%)

chal%: c%.o $(COMMON_OBJ)
	$(CXX) $(LIBS) -o $@ $^

chal7: c7.o $(COMMON_OBJ) ../common/aes128_ecb.o
	$(CXX) $(LIBS) -o $@ $^

run%: chal%
	@echo -e "----- Running $^ -----"
	@./$^

p%: c%.py
	@./$^

clean:
	rm -f $(EXE) $(OBJ) *.o $(COMMON_OBJ)
