CXX=g++
CXXFLAGS=--std=c++11 -Wall -Wextra -Werror -pedantic -pedantic-errors -Wshadow

CXXFLAGS+=-I../common

CXXFLAGS+=-ggdb

SRC := $(sort $(wildcard c*.cpp))
OBJ := $(addsuffix .o,$(basename $(SRC)))
EXE := $(SRC:c%.cpp=chal%)

COMMON := ../common/utils.cpp ../common/aes128_ecb.cpp
COMMON_OBJ := $(addsuffix .o,$(basename $(COMMON)))

chal9: LIBS = -lcrypto

.SECONDARY: $(addprefix chal,$(TARGETS))

all: $(EXE)

run: $(EXE:chal%=run%)

chal%: c%.o $(COMMON_OBJ)
	$(CXX) $(LIBS) -o $@ $^

run%: chal%
	@echo -e "----- Running $^ -----"
	@./$^

p%: c%.py
	@./$^

clean:
	rm -f $(EXE) $(OBJ) $(COMMON_OBJ)
