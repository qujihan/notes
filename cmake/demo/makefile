.PHONY: all  cmake build run clean

cmake:
	cmake -S . -B build
	
build:
	cmake --build build

run:
	./build/main

clean:
	rm -rf build bin
