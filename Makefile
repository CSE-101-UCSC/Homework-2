CXX := g++
override CXXFLAGS := -std=c++17 -Wall -Wextra -pedantic-errors

.PHONY: all check clean

all: check FlightManager

check:
	@python3 scripts/check_rules.py

FlightManager: main.cpp flights.cpp avl.cpp flights.h avl.h types.h
	$(CXX) $(CXXFLAGS) main.cpp flights.cpp avl.cpp -o $@

clean:
	rm -f FlightManager FlightManager.exe
	if [ -d tests ]; then find tests -type f -name output.txt -delete; fi
