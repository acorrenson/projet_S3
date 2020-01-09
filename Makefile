CC = /usr/bin/gcc

.PHONY: build all clean doc graph demo

all: build

demo: build
	@ ./build/Tsp -f ./data/att10.tsp -bf -bfm -rw -ppv -2opt -ga -v -o ./data/results_att10.csv
	@ python3 scripts/graph.py ./data/results_att10.csv
	@ ./build/Tsp -f ./data/att20.tsp -rw -ppv -2opt -ga -o ./data/results_att20.csv
	@ python3 scripts/graph.py ./data/results_att20.csv
	@ ./build/Tsp -f ./data/att48.tsp -rw -ppv -2opt -ga -o ./data/results_att48.csv
	@ python3 scripts/graph.py ./data/results_att48.csv

run: build
	@ ./build/Tsp -f ./data/att10.tsp -bf -rw -bfm -ppv -v -2opt -o ./data/results.csv

graph:
	@ python3 ./scripts/graph.py ./data/results.csv

test: build
	@ cd build/ && ./Test

build:
	@	mkdir -p build
	@ cd build/ && cmake ../src -DCMAKE_C_COMPILER=${CC} && make

doc:
	@ doxygen Doxyfile

clean:
	@ rm -r build


