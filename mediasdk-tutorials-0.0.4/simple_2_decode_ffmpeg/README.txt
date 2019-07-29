1. Introduction
    This example uses the basic decode code and combines with the ffmpeg libraries to extract and decode the video from the standard container: mp4, mkv and mpeg2. To build the binary of this example, the FFmpeg Dev libraries must be installed first.

2. Install the FFmpeg Dev library.
    2.1 Install on Windows
        2.1.1 Open a browser and go to https://ffmpeg.zeranoe.com/builds/
        2.1.2 Select "Version" as version you want; "Architecture" as "Windows 64-bit"; "Linking" as "Dev"; click the "Download Build" button.
        2.1.2 Unzip the Dev package to a local directory.
        2.1.3 Select "Version" the same as in 2.1.2; "Architecture" as "Windows 64-bit"; "Linking" as "Shared"; click the "Download Build" button.
        2.1.4 Unzip the Shared package to a local directory.
        2.1.5 Add the environment variables, open "Advanced system settings" and click "Environment Variables..." button
            i. Add "FFMPEG_ROOT" with the directory to the unzip the ffmpeg Dev package.
            ii. Add the directory "bin" under the unzipped the ffmpeg Shared package directory to the "PATH"
        2.1.6 Run the solution build.
    2.2 Install on Linux:
        2.2.1 Starts from the Generic Compilation Guide of FFmpeg(https://trac.ffmpeg.org/wiki/CompilationGuide), in the guide, select the Linux and the distribution you are working on, for example, the Ubuntu build instructions(https://trac.ffmpeg.org/wiki/CompilationGuide/Ubuntu); then following the specific instruction page to build the required FFmpeg modules..
        2.2.2 After building the requested FFMpeg modules. Starts building the shared FFmpeg libraries. To build the shared library, a minor change must be done to the original build command: When configuring the final build, please append the following arguments to the original "./configure..." command: "--enable-shared --enable-pic --extra-cflags=-fPIC", for example, 
>PATH="$HOME/bin:$PATH" PKG_CONFIG_PATH="$HOME/ffmpeg_build/lib/pkgconfig" ./configure \
  --prefix="$HOME/ffmpeg_build" \
  --pkg-config-flags="--static" \
  --extra-cflags="-I$HOME/ffmpeg_build/include" \
  --extra-ldflags="-L$HOME/ffmpeg_build/lib" \
  --extra-libs=-lpthread \
  --bindir="$HOME/bin" \
  --enable-gpl \
  --enable-libass \
  --enable-libfdk-aac \
  --enable-libfreetype \
  --enable-libmp3lame \
  --enable-libopus \
  --enable-libtheora \
  --enable-libvorbis \
  --enable-libvpx \
  --enable-libx264 \
  --enable-libx265 \
  --enable-nonfree \
  --enable-shared \
  --enable-pic \
  --extra-cflags=-fPIC
        2.2.3 In the "CFLAG" of the Makefile, enable/disable "-DDEBUG" flag for debug purpose; enable/disable "-DDECODE_AUDIO" to include the audio decoder which will generate the raw audio file "Audio.dat"
        2.2.4 Run normal "make" command to build

3. To run the example, use the following command:
~\_build\x64\Debug>simple_decode_ffmpeg.exe ~\Downloads\BigBuckBunny_320x180.mp4 out.yuv
Input #0, mov,mp4,m4a,3gp,3g2,mj2, from '~\Downloads\BigBuckBunny_320x180.mp4':
  Metadata:
    major_brand     : isom
    minor_version   : 512
    compatible_brands: mp41
    creation_time   : 1970-01-01T00:00:00.000000Z
    title           : Big Buck Bunny
    artist          : Blender Foundation
    composer        : Blender Foundation
    date            : 2008
    encoder         : Lavf52.14.0
  Duration: 00:09:56.46, start: 0.000000, bitrate: 867 kb/s
    Stream #0:0(und): Video: h264 (Constrained Baseline) (avc1 / 0x31637661), yuv420p, 320x180 [SAR 1:1 DAR 16:9], 702 kb/s, 24 fps, 24 tbr, 24 tbn, 48 tbc (default)
    Metadata:
      creation_time   : 1970-01-01T00:00:00.000000Z
      handler_name    : VideoHandler
    Stream #0:1(und): Audio: aac (LC) (mp4a / 0x6134706D), 48000 Hz, stereo, fltp, 159 kb/s (default)
    Metadata:
      creation_time   : 1970-01-01T00:00:00.000000Z
      handler_name    : SoundHandler
Frame number: 14315
Execution time: 113.57 s (126.04 fps)

    NOTE:
        1) Use "-sw" for the software mode.
        2) The default mode is "-auto"
        3) The output file can be empty so the example will run the decode only.
        4) This example only tested with mp4 file with H.264 codec.

4. Design
    The sample code is modified base on the original tutorial simple_decode. The call to the FFMpeg integration is added to the original source code, the modified area is wrapped by the following comment line:
    // =========== ffmpeg splitter integration ============
    ......
 
    // =========== ffmpeg splitter integration end ============
    The API to the FFmpeg integration is defined and implemented in the files ffmpeg_utils.h, ffmpeg_utils.cpp. The modified area shows the difference from the original simple_decode source code. It can be see the ffmpeg dmux works as the bit stream reader in the original code.
    The structure demuxControl keeps the control parameters of the demux process; the function openDemuxControl() initializes and configures the demuxControl structure; the structure is then used for the demux and decoding process; during the decoding, the function ffmpegReadFrame() reads the video frame after demuxing; finally the function closeDemuxControl() releases the system resources.
    It has the following stage:
    1) Parse the argument of the command line, then open the file I/O objects. The input file is the standard container supported by FFMpeg; note if the output file is designated, an audio data file will be created automatically. The demux open function is called to open the input file, extract the meta data and initialize the FFMpeg process configuration.
    2) Create the Media SDK session.
    3) Configure the basic video parameters, noted the video codec is set with the value from the input file. It will also load the HEVC plugin if detected the codec type from the input.
    4) Allocate the bit stream buffer required, read the ES header from the input file with the FFMpeg function and fill the rest of the video parameters.
    5) Allocate the surfaces(video frame working memory) with the system memory required by the decoder.
    6) Initialize the decoder
    7) Start the decoding loop
        Stage1: Main decoding loop, to decode from the input bit stream; the bit stream buffer is filled from the FFMpeg reader; then write the output surface into the output file(YUV file).
        Stage2: Retrieve the buffered decoded frames; after the end of the input stream, flush the cached stream data for decoding.
    8) Clean up the resources and close the session, include a FFMpeg clean up function.


5. Reference:
    The latest article: https://software.intel.com/en-us/articles/quick-start-on-integrating-ffmpeg-libraries
    The previous article: https://software.intel.com/en-us/articles/integrating-intel-media-sdk-with-ffmpeg-for-muxdemuxing-and-audio-encodedecode-usages

