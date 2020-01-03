CC = /usr/bin/gcc

.PHONY: build all clean doc

all: build

run: build
	@ ./build/Tsp -f ./data/att10.tsp -bf -rw -bfm -ppv -nz -v -2opt -o ./data/results.csv

test: build
	@ cd build/ && ./Test

build:
	@	mkdir -p build
	@ cd build/ && cmake ../src -DCMAKE_C_COMPILER=${CC} && make

doc:
	@ doxygen Doxyfile

clean:
	@ rm -r build


