CXX=g++
CXXFLAGS=--std=c++11 -Wall -Wextra -Werror -pedantic -pedantic-errors -Wshadow

TARGETS=1 2 3 4 5 6 7

.SECONDARY: $(addprefix chal,$(TARGETS))

all:

chal%: c%.o
	$(CXX) -o $@ $<

c%: chal%
	@./$^

p%: c%.py
	@./$^

clean:
	rm -f chal? c?.o
