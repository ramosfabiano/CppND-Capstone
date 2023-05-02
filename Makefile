.PHONY: all
all: build

.PHONY: format
format:
	find src/ -name  *.?pp | xargs astyle -n --style=allman --add-braces --convert-tabs --indent-cases --pad-oper

.PHONY: build
build:
	mkdir -p build
	mkdir -p install
	cd build && \
	cmake -DCMAKE_BUILD_TYPE=release -DCMAKE_INSTALL_PREFIX=../install .. && \
	make -j3 install

.PHONY: debug
debug:
	mkdir -p build
	mkdir -p install
	cd build && \
	cmake -DCMAKE_BUILD_TYPE=debug -DCMAKE_INSTALL_PREFIX=../install .. && \
	make -j3 install

.PHONY: clean
clean:
	rm -rf build install
	

