CXX=g++
CXXFLAGS=--std=c++11 -Wall -Wextra -Werror -pedantic -pedantic-errors -Wshadow

SRC := $(sort $(wildcard c?.cpp))
OBJ := $(addsuffix .o,$(basename $(SRC)))
EXE := $(SRC:c%.cpp=chal%)

COMMON := utils.cpp
COMMON_OBJ := $(addsuffix .o,$(basename $(COMMON)))

.SECONDARY: $(addprefix chal,$(TARGETS))

all: $(EXE)

run: $(EXE:chal%=run%)

chal%: c%.o $(COMMON_OBJ)
	$(CXX) -o $@ $^

run%: chal%
	@echo -e "----- Running $^ -----"
	@./$^

p%: c%.py
	@./$^

clean:
	rm -f $(EXE) $(OBJ) $(COMMON_OBJ)
