B=_build
all:
	cmake -H. -B$(B)
	cmake --build $(B)
	$(B)/yio_test
	cd $(B) && ctest

clean:
	rm -fr $(B)

