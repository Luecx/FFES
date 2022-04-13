# --------------------------- COMPILERS ---------------------------------
CXX      = g++
EMCC	 = emcc
# ----------------------------- SOURCE  ---------------------------------
SRC_CXX  = $(wildcard *.cpp */*.cpp */*/*.cpp */*/*/*.cpp)
SRC_C	 = $(wildcard *.c */*.c */*/*.c */*/*/*.c)
# ----------------------------- PATHS ---------------------------------
_THIS    := $(realpath $(dir $(abspath $(lastword $(MAKEFILE_LIST)))))
_ROOT	 := $(_THIS)/.
_OUT     := $(_ROOT)/bin/
# ------------------------------ EXE ------------------------------------
NAME     = FFES
EXE      = $(_OUT)$(NAME)_$(MAJOR).$(MINOR)
MINOR    = 1
MAJOR    = 0
ifeq ($(OS),Windows_NT)
    PREFIX := windows
    SUFFIX := .exe
else
    PREFIX := linux
    SUFFIX :=
endif

LIBS     = -pthread -Wl,--whole-archive -lpthread -Wl,--no-whole-archive

########################################################################################################################
#--------------------------------------------- FLAGS    ---------------------------------------------------------------#
########################################################################################################################

WFLAGS = -std=c++20
CFLAGS = -O3 $(WFLAGS) -DNDEBUG -flto
RFLAGS = -O3 $(WFLAGS) -DNDEBUG -flto -static
PFLAGS = -O0 $(WFLAGS) -DNDEBUG -p -pg
DFLAGS = -O0 $(WFLAGS) -g


# ------------------------------- vector extensions ----------------------------------------------
POPCNTFLAGS = -DUSE_POPCNT -msse3 -mpopcnt
PEXTFLAGS   = $(POPCNTFLAGS) -DUSE_PEXT -mbmi2
SSEFLAGS    = $(POPCNTFLAGS) -msse -msse4.1 -mssse3 -msse2
AVX2FLAGS   = $(SSEFLAGS) -mavx2
AVX512FLAGS = $(AVX2FLAGS) -mavx512f -mavx512bw -mavx512dq
NATIVEFLAGS = -march=native

# ------------------------------- Emscripten ---------------------------------------------
EMS_MEMORY		 = -s TOTAL_MEMORY=4294967296 -s ALLOW_MEMORY_GROWTH=1 -s INITIAL_MEMORY=33554432
EMS_THREADS 	 = -s PROXY_TO_PTHREAD
EMS_KEEPALIVE	 = -s NO_EXIT_RUNTIME=1 -s "EXPORTED_RUNTIME_METHODS=['ccall']"
EMS_SIMD		 = -msimd128
EMS_ENVIRONMENT  = -s MODULARIZE=1 -s EXPORT_NAME=FFES -s ENVIRONMENT=web,worker
EMS_PREAMBLE     =  --pre-js $(_ROOT)/src/wasm/web_preamble.js
EMS_ASSERT		 = -s ASSERTIONS=1
EMS_MAKROS		 = $(EMS_MEMORY) $(EMS_THREADS) $(EMS_KEEPALIVE) $(EMS_SIMD) $(EMS_ENVIRONMENT) $(EMS_PREAMBLE) $(EMS_ASSERT)

# ------------------------------- Makros ----------------------------------------------
VERSION_MAKRO    = -DMINOR_VERSION=$(MINOR) -DMAJOR_VERSION=$(MAJOR)
ENABLE_FILESYS   = -DUSE_FILESYSTEM

FULL_MAKRO 		 = $(VERSION_MAKRO) $(ENABLE_FILESYS)

########################################################################################################################
#--------------------------------------------- TARGETS  ---------------------------------------------------------------#
########################################################################################################################

emscripten:
	$(EMCC) $(CFLAGS) $(SRC_CXX) $(EMS_MAKROS) $(VERSION_MAKRO) $(LIBS) -o F:/OneDrive/ProgrammSpeicher/PHPStormProjects/FEMWebsite/solver/$(NAME)_$(MAJOR).$(MINOR).js

native:
	mkdir -p $(ROOT)$(FOLDER)
	$(CXX) $(CFLAGS) $(SRC_CXX) $(SRC_C) $(FULL_MAKRO) $(LIBS) $(NATIVEFLAGS) -o $(EXE)-x64-$(PREFIX)-native$(SUFFIX)

release:
	mkdir -p $(ROOT)$(FOLDER)
	$(CC) $(RFLAGS) $(SRC_CXX) $(SRC_C) $(FULL_MAKRO) $(LIBS) $(AVX512FLAGS) -o $(EXE)-x64-$(PREFIX)-avx512$(SUFFIX)
	$(CC) $(RFLAGS) $(SRC_CXX) $(SRC_C) $(FULL_MAKRO) $(LIBS) $(AVX2FLAGS) -o $(EXE)-x64-$(PREFIX)-avx2$(SUFFIX)
	$(CC) $(RFLAGS) $(SRC_CXX) $(SRC_C) $(FULL_MAKRO) $(LIBS) $(SSEFLAGS) -o $(EXE)-x64-$(PREFIX)-sse2$(SUFFIX)

