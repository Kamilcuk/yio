.PHONY: all build test clean

all: build

build:
	cmake -B build -S . -DCMAKE_C_COMPILER=gcc -DCMAKE_CXX_COMPILER=g++ -GNinja
	cmake --build build

test: build
	ctest --test-dir build --output-on-failure

clean:
	rm -rf build
