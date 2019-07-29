EXEC=../_build/simple_vpp_vmem
INCS= \
  ../common/common_utils.h \
  ../common/common_vaapi.h \
  ../common/bits/linux_defs.h \
  ../common/cmd_options.h
SRCS= \
  src/simple_vpp_vmem.cpp \
  ../common/common_utils.cpp \
  ../common/common_utils_linux.cpp \
  ../common/common_vaapi.cpp \
  ../common/cmd_options.cpp

CFLAGS=-std=c++11 -I/usr/local/include -I../common -I$(MFX_HOME)/include/mfx
LFLAGS=-L$(MFX_HOME)/lib -lmfx -lva -lva-drm -lpthread -lrt -ldl

$(EXEC): $(INCS) $(SRCS) Makefile
	mkdir -p ../_build
	g++ -O0 -g -o $(EXEC) $(SRCS) $(CFLAGS) $(LFLAGS)

.PHONY: clean
clean:
	rm $(EXEC)
