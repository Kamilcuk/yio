.PHONY: all build test clean pack install

all: build

build:
	cmake -B build -S . -DCMAKE_C_COMPILER=gcc -DCMAKE_CXX_COMPILER=g++ -GNinja
	cmake --build build

install: build
	cmake --install build

test: build
	ctest --test-dir build --output-on-failure --progress

clean:
	rm -rf build
