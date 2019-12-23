CC = /usr/bin/gcc

.PHONY: build all clean doc

all: build

run: build
	@ ./build/Tsp ./data/att10.tsp

test: build
	@ cd build/ && ./Test

build:
	@	mkdir -p build
	@ cd build/ && cmake ../src -DCMAKE_C_COMPILER=${CC} && make

doc:
	@ doxygen Doxyfile

clean:
	@ rm -r build


