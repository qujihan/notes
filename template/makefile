.PHONY: all cmake build clean 

all: cmake build run

cmake:
	@cmake -S . -B build -G Ninja

build:
	@cmake --build build

run:
	@./build/main

clean:
	@rm -rf build