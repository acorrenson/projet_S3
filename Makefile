CC = /usr/bin/gcc

.PHONY: build all clean doc

all: build

run: build
	@ ./build/Tsp ./src/test.txt

test: build
	@ ./build/Test

build:
	@	mkdir -p build
	@ cd build/ && cmake ../src -DCMAKE_C_COMPILER=${CC} && make

doc:
	@ doxygen Doxyfile

clean:
	@ rm -r build


