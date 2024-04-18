BASES := $(shell seq 1 10 | sed 's/^/base/')
ITEMS := $(shell seq 1 42 | sed 's/^/item/')


.PHONY: all cmake build clean

all: cmake build

cmake:
	@cmake -S . -B build -DCMAKE_EXPORT_COMPILE_COMMANDS=ON -G Ninja;
	
build:
	@cmake --build build -j

clean:
	@/bin/rm -rf build bin

.PHONY: $(ITEMS) $(BASES)

$(ITEMS):
	@echo "Building and running $@"
	@cmake -S . -B build -DCMAKE_EXPORT_COMPILE_COMMANDS=ON -G Ninja -DONLY_BUILD=$@
	@cmake --build build -j
	@./build/src/$@

$(BASES):
	@echo "Building and running $@"
	@cmake -S . -B build -DCMAKE_EXPORT_COMPILE_COMMANDS=ON -G Ninja -DONLY_BUILD=$@
	@cmake --build build -j
	@./build/src/$@