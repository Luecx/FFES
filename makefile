CC       = g++
SRC      = $(wildcard *.cpp */*.cpp */*/*.cpp */*/*/*.cpp)
LIBS     = -pthread -Wl,--whole-archive -lpthread -Wl,--no-whole-archive
FOLDER   = bin/
ROOT     = ./
NAME     = FFEM
EXE      = $(ROOT)$(FOLDER)$(NAME)_$(MAJOR).$(MINOR)
MINOR    = 1
MAJOR    = 0
ifeq ($(OS),Windows_NT)
    PREFIX := windows
    SUFFIX := .exe
else
    PREFIX := linux
    SUFFIX :=
endif

WFLAGS = -std=c++17 -Wall -Wextra -Wshadow
CFLAGS = -O3 $(WFLAGS) -DNDEBUG
PFLAGS = -O0 $(WFLAGS) -DNDEBUG -p -pg
DFLAGS = -O0 $(WFLAGS) -g

SSEFLAGS    = -msse
SSE2FLAGS   = $(SSEFLAGS) -msse2
SSE3FLAGS   = $(SSE2FLAGS) -msse3
SSE41FLAGS  = $(SSE3FLAGS) -msse4.1
SSE42FLAGS  = $(SSE41FLAGS) -msse4.2
POPFLAGS    = $(SSE42FLAGS) -DUSE_POPCNT -mpopcnt
AVXFLAGS    = $(POPFLAGS) -mavx
AVX2FLAGS   = $(AVXFLAGS) -mavx2
AVX512FLAGS = $(AVX2FLAGS) -mavx512f -mavx512bw -mavx512dq

OPENMPFLAGS = -fopenmp
NATIVEFLAGS = -march=native

MAKROS      = -DMINOR_VERSION=$(MINOR) -DMAJOR_VERSION=$(MAJOR)

NATIVEFLAGS = -march=native -flto
STATICFLAGS = -static -static-libgcc -static-libstdc++


static:
	mkdir -p $(ROOT)$(FOLDER)
	$(CC) $(CFLAGS) $(SRC) $(STATICFLAGS) $(MAKROS) $(LIBS) -o $(EXE)-x64-$(PREFIX)-static$(SUFFIX)

native:
	mkdir -p $(ROOT)$(FOLDER)
	$(CC) $(CFLAGS) $(SRC) $(STATICFLAGS) $(MAKROS) $(LIBS) -o $(EXE)-x64-$(PREFIX)-native$(SUFFIX)

#release:
#	mkdir -p $(ROOT)$(FOLDER)
#	$(CC) $(RFLAGS) $(SRC) $(MAKROS) $(LIBS) $(AVX512FLAGS) -o $(EXE)-x64-$(PREFIX)-avx512$(SUFFIX)
#	$(CC) $(RFLAGS) $(SRC) $(MAKROS) $(LIBS) $(AVX2FLAGS) -o $(EXE)-x64-$(PREFIX)-avx2$(SUFFIX)
#	$(CC) $(RFLAGS) $(SRC) $(MAKROS) $(LIBS) $(POPFLAGS) -o $(EXE)-x64-$(PREFIX)-popcnt$(SUFFIX)
#	$(CC) $(RFLAGS) $(SRC) $(MAKROS) $(LIBS) $(SSE42FLAGS) -o $(EXE)-x64-$(PREFIX)-sse4.2$(SUFFIX)
#	$(CC) $(RFLAGS) $(SRC) $(MAKROS) $(LIBS) $(SSE41FLAGS) -o $(EXE)-x64-$(PREFIX)-sse4.1$(SUFFIX)
#	$(CC) $(RFLAGS) $(SRC) $(MAKROS) $(LIBS) $(SSE3FLAGS) -o $(EXE)-x64-$(PREFIX)-sse3$(SUFFIX)
#	$(CC) $(RFLAGS) $(SRC) $(MAKROS) $(LIBS) $(SSE2FLAGS) -o $(EXE)-x64-$(PREFIX)-sse2$(SUFFIX)
#	$(CC) $(RFLAGS) $(SRC) $(MAKROS) $(LIBS) $(SSEFLAGS) -o $(EXE)-x64-$(PREFIX)-sse$(SUFFIX)
