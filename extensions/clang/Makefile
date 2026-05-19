.PHONY: all build test clean

all: build

build:
	cmake -B build -S . -DCMAKE_C_COMPILER=clang -DCMAKE_CXX_COMPILER=clang++ -GNinja
	cmake --build build

test: build
	ctest --test-dir build --output-on-failure --progress

clean:
	rm -rf build
