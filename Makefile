.PHONY: all
all: clean format build

.PHONY: format
format:
	clang-format src/lib/*.?pp  -i
	clang-format src/app/*.?pp  -i

.PHONY: build
build:
	mkdir -p build
	mkdir -p install
	cd build && \
	cmake -DCMAKE_BUILD_TYPE=release -DCMAKE_INSTALL_PREFIX=../install .. && \
	make install

.PHONY: debug
debug:
	mkdir -p build
	mkdir -p install
	cd build && \
	cmake -DCMAKE_BUILD_TYPE=debug -DCMAKE_INSTALL_PREFIX=../install .. && \
	make install

.PHONY: clean
clean:
	rm -rf build install
	

