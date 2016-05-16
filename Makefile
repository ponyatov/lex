# OS = linux | windows
OS ?= linux
MODULE ?= $(notdir $(CURDIR))
AUTHOR ?= "Dmitry Ponyatov <dponyatov@gmail.com>"
GITHUB ?= "https://github.com/ponyatov/$(MODULE)"
LICENSE ?= "GNU Lesser GPL"

# TAIL = -n7 | -n17
TAIL += -n17

log.log: ./exe.exe src.src
	./exe.exe < src.src > log.log && tail $(TAIL) log.log
C = cpp.cpp $(OS).cpp ypp.tab.cpp lex.yy.c
H = hpp.hpp $(OS).hpp ypp.tab.hpp
CXXFLAGS += -std=gnu++11 \
	-DOS=\"$(OS)\" -DMODULE=\"$(MODULE)\" -DAUTHOR=\"$(AUTHOR)\" \
	-DGITHUB=\"$(GITHUB)\" -DLICENSE=\"$(LICENSE)\"
./exe.exe: $(C) $(H) Makefile
	$(CXX) $(CXXFLAGS) -o $@ $(C)
ypp.tab.cpp: ypp.ypp
	bison $<
lex.yy.c: lpp.lpp
	flex $<