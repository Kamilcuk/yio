.PHONY: all build test clean pack install

B = build
all: build

build:
	cmake -B $(B) -S . -DCMAKE_C_COMPILER=gcc -DCMAKE_CXX_COMPILER=g++ -GNinja
	cmake --build $(B)

install: build
	cmake --install $(B)

test: build
	ctest --test-dir $(B) --output-on-failure --progress

clean:
	rm -rf build
