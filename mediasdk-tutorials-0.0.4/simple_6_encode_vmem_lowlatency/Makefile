EXEC=../_build/simple_encode_vmem_lowlat
INCS= \
  ../common/common_utils.h \
  ../common/common_vaapi.h \
  ../common/bits/linux_defs.h \
  ../common/cmd_options.h
SRCS= \
  src/simple_encode_vmem_lowlat.cpp \
  ../common/common_utils.cpp \
  ../common/common_utils_linux.cpp \
  ../common/common_vaapi.cpp \
  ../common/cmd_options.cpp

CFLAGS=-std=c++11 -I/usr/local/include -I../common -I$(MFX_HOME)/include/mfx -std=c++0x
LFLAGS=-L$(MFX_HOME)/lib -lmfx -lva -lva-drm -lpthread -lrt -ldl

$(EXEC): $(SRCS) Makefile
	mkdir -p ../_build
	g++ -O0 -g -o $(EXEC) $(SRCS) $(CFLAGS) $(LFLAGS)

.PHONY: clean
clean:
	rm $(EXEC)
