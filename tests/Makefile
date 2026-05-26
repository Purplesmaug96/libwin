CXX_NATIVE = g++
CXX_WINE = wineg++
CXX_MINGW = x86_64-w64-mingw32-g++

DOCTEST_INCLUDES = -I../external/doctest
LIBWIN_INCLUDES = -I../include

.PHONY: build clean

build: ../build/libwin_tests ../build/libwin_tests_wine ../build/libwin_tests_mingw

../build/libwin_tests: libwin_tests.cpp
	mkdir -p ../build
	$(CXX_NATIVE) $(LIBWIN_INCLUDES) $(DOCTEST_INCLUDES) -o $@ $<

../build/libwin_tests_wine: libwin_tests.cpp
	mkdir -p ../build
	$(CXX_WINE) $(DOCTEST_INCLUDES) -I. -o $@ $<

../build/libwin_tests_mingw: libwin_tests.cpp
	mkdir -p ../build
	$(CXX_MINGW) $(DOCTEST_INCLUDES) -static -o $@ $< -lwinmm

clean:
	rm -f ../build/libwin_tests ../build/libwin_tests_wine ../build/libwin_tests_mingw