EXEC=../_build/simple_decode_ffmpeg
INCS= \
  ../common/common_utils.h \
  ../common/common_vaapi.h \
  ../common/bits/linux_defs.h \
  ../common/cmd_options.h
SRCS= \
  src/simple_decode_ffmpeg.cpp \
  ../common/common_utils.cpp \
  ../common/common_utils_linux.cpp \
  ../common/common_vaapi.cpp \
  ../common/ffmpeg_utils.cpp \
  ../common/cmd_options.cpp

FFMPEG_LFLAGS=-lavformat -lavcodec -lavutil -lswresample
CFLAGS=-DDECODE_AUDIO -DDEBUG -g -std=c++11 -I/usr/local/include -I../common -I$(MFX_HOME)/include/mfx $(FFMPEG_CFLAGS)
#CFLAGS=-DDECODE_AUDIO -std=c++11 -I/usr/local/include -I../common -I$(MFX_HOME)/include/mfx $(FFMPEG_CFLAGS)
LFLAGS=-L$(MFX_HOME)/lib -lmfx -lva -lva-drm -lpthread -lrt -ldl $(FFMPEG_LFLAGS)

$(EXEC): $(INCS) $(SRCS) Makefile
	mkdir -p ../_build
	g++ -O0 -g -o $(EXEC) $(SRCS) $(CFLAGS) $(LFLAGS)

.PHONY: clean
clean:
	rm $(EXEC)
