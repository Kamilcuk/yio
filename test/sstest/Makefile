MAKEFLAGS = -rR
B ?=
CMAKEARGS ?=
all: test
build:
	cmake -S . -B ./_build/$(B) \
		-DSSTEST_BUILD_TESTING=1 \
		-DBUILD_TESTING=1 \
		-DCMAKE_BUILD_TYPE=Debug \
		-DCMAKE_EXPORT_COMPILE_COMMANDS=1 \
		$(CMAKEARGS)
	cmake --build _build/$(B)
test: build
	ctest --test-dir _build/$(B) -V

cicd: build
	ctest --test-dir _build/$(B) -V -E 'segfault'

arm: B = arm
arm: CMAKEARGS = \
	-DCMAKE_C_COMPILER=arm-none-eabi-gcc \
	-DCMAKE_C_FLAGS='--specs=rdimon.specs --specs=nano.specs' \
	-DCMAKE_CROSSCOMPILING=1 \
	-DCMAKE_CROSSCOMPILING_EMULATOR=qemu-arm \
	-DCMAKE_TRY_COMPILE_TARGET_TYPE=STATIC_LIBRARY \
	-DCMAKE_SYSTEM=default
arm: test

clean:
	rm -rf _build
