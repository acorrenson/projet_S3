CC = /usr/bin/gcc

.PHONY: build

run: build
	@ ./build/Tsp ./src/test.txt

build:
	@	mkdir -p build
	@ cd build/ && cmake ../src -DCMAKE_C_COMPILER=${CC} && make

doc:
	@ doxygen Doxyfile

clean:
	@ rm -r build


