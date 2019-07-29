Tutorials are geared towards beginners by providing simple code examples for basic media features and excellent code examples to start working with the Intel Media SDK. All code examples come with Microsoft Visual Studio* solution files (Windows) and Makefile (Linux) to get you started.

1. Build and run the tutorial programs
    The source code has to be built, here are build instructions for Windows and Linux:
    1.1 To build on Windows, open the solution file "tutorial.sln" in Visual Studio 2015 or above to open all the projects, set "Platform" as "x64" and build the solution for all the tutorial binary at one time; you can also open the single project by opening the solution file in the project directory.
    1.2 The root of the build binary on Windows is "<tutorial root>\_build" and different binaries based on 32bit or 64bit are built to different sub-directory.
    1.3 To build on Linux, chang the directory to the root of the tutorial source code, run "make" to build the solution for all the tutorial binary at one time; you can also change the directory of the specific project and run "make" to build the single binary for that project.
    1.4 The root of the build binary on Linux is "<tutorial root>/_build".
    1.5 The simple_encode_ffmpeg and simple_decode_ffmpeg requires to install the FFmpeg Dev package and set up the environment variable to point to the FFMpeg libraries, refer to the README.txt in these projects or this online article: https://software.intel.com/en-us/articles/quick-start-on-integrating-ffmpeg-libraries
    1.6 How to run tutorial binary: refer to README.txt of each project.
    1.7 How to prepare the media content to run the tutorial:
        1) Download the content with the standard container, for example, .mp4, .mkv.
            Example for H.264: http://download.blender.org/peach/bigbuckbunny_movies/
            Example for HEVC: http://www.libde265.org/downloads-videos/
            Example for HEVC 10bit: http://kodi.wiki/view/Samples
        2) Use FFmpeg to extract the raw video stream(elementary stream), here are the command:
            > ffmpeg -i example.mp4 -vcodec copy -an -bsf h264_mp4toannexb example.264
            > ffmpeg -i example.mp4 -vcodec copy -an -bsf h264_mp4toannexb -vframes 100 example.264 (Extract only 100 frames)
            > ffmpeg -i example.mp4 -vcodec copy -an -bsf hevc_mp4toannexb example.265
            > ffmpeg -i example.mp4 -vcodec copy -an -bsf hevc_mp4toannexb -vframes 100 example.265 (Extract only 100 frames)
        3) To extract the YUV raw video stream with NV12 format, use the following FFmpeg command:
            >ffmpeg -i example.mkv -vcodec rawvideo -pix_fmt yuv420p example.yuv
            >ffmpeg -i example.mkv -vcodec rawvideo -pix_fmt yuv420p -vframes 100 example.yuv (Extract only 100 frames)
    1.8 To run the help message, use "--help" argument:
            ~\_build\x64\Debug>simple_decode.exe --help
            Decodes INPUT and optionally writes OUTPUT.

            Usage: simple_decode.exe [options] INPUT [OUTPUT]

            Options:
              --help        Print this help
              --version     Print version info
              -auto         Automatically chose Media SDK library implementation. This is the default.
              -sw           Load SW Media SDK Library implementation
              -hw           Load HW Media SDK Library implementation

2. The tutorial list

    2.1 There are 22 projects in the Linux OS, and please go to each project directory to find the detailed document: 
        1) simple_codec: A program to check if the hardware codecs are support in the current platform.
        2) simple_decode: This is the basic decoding example by using the system memory
        3) simple_decode_ffmpeg: This example shows how to integrate with the FFmpeg demux, so it can read the video container file.
        4) simple_decode_hevc10: This example shows how to decode the HEVC 10bit stream.
        5) simple_decode_vmem: This is the decoding example by using the video memory
        6) simple_decode_vpp_pp: This example downsizes the input video to half both ways after decoding it.
        7) simple_encode: This example shows the basic encoding programming.
        8) simple_encode_ffmpeg: This example shows the encoder integrated with the FFmpeg mux model to output a video with the container.
        9) simple_encode_vmem: This is the encoding example by using the video memory.
        10) simple_encode_vmem_async: This is the encoding example with the video memory running with 4 async tasks explicitely.
        11) simple_encode_vmem_lowlat: This example encodes the input video with the configuration of video memory and low latency configuration. It also measures and shows the latency of the first 1000 frames.
        12) simple_encode_vmem_preproc: This example inputs the video with RGB format, it pre-processes the input to NV12--the native color format of the Media SDK and then encodes it. The example is configured with the video memory.
        13) simple_session: This basic application just creates a Media SDK session.
        14) simple_transcode: This is the basic transcoding example to transcode an input H.264 stream to a H.264 stream by using the system memory.
        15) simple_transcode_opaque: This is the basic transcoding example to transcode an input H.264 stream to a H.264 stream by using the Media SDK opaque memory management.
        16) simple_transcode_opaque_async: This example transcodes an input H.264 stream to a H.264 stream in 4 async tasks with the Media SDK opaque memory management.
        17) simple_transcode_opaque_async_vppresize: This example transcodes an input H.264 stream to a H.264 stream and resized to 1/4 in both height and width, the VPP works in 4 async tasks with the Media SDK opaque memory management.
        18) simple_transcode_opaque_lowlat: This is the basic transcoding example to transcode an input H.264 stream to a H.264 stream by using the Media SDK opaque memory management. The example is configured to low latency with measuring the latency for the first 1000 frames.
        19) simple_transcode_vmem: This is the basic transcoding example to transcode an input H.264 stream to a H.264 stream by using the video memory.
        20) simple_vpp: This example shows how to downsize and denoise of the input video by using the system memory.
        21) simple_vpp_vmem: This example shows how to downsize and denoise of the input video by using the video memory.

    2.2 There are 30 projects in the Windows solution, and please go to each project directory to find the detailed document: 
        1) simple_codec: A program to check if the hardware codecs are support in the current platform.
        2) simple_decode: This is the basic decoding example by using the system memory
        3) simple_decode_d3d: This is the decoding example by using the DirectX 9 video memory
        4) simple_decode_d3d11: This is the decoding example by using the DirectX 11 video memory
        5) simple_decode_ffmpeg: This example shows how to integrate with the FFmpeg demux, so it can read the video container file.
        6) simple_decode_hevc10: This example shows how to decode the HEVC 10bit stream.
        7) simple_decode_vpp_pp: This example downsizes the input video to half both ways after decoding it.
        8) simple_encode: This example shows the basic encoding programming.
        9) simple_encode_d3d: This is the encoding example by using the DirectX 9 video memory.
        10) simple_encode_d3d_async: This is the encoding example with the DirectX 9 video memory running with 4 async tasks explicitely.
        11) simple_encode_d3d_lowlat: This example encodes the input video with the configuration of DirectX 9 video memory and low latency configuration. It also measures and shows the latency of the first 1000 frames.
        12) simple_encode_d3d_preproc: This example inputs the video with RGB format, it pre-processes the input to NV12--the native color format of the Media SDK and then encodes it. The example is configured with DirectX 9 video memory.
        13) simple_encode_d3d11: This is the encoding example with the DirectX 11 video memory.
        14) simple_encode_d3d11_async: This is the encoding example with the DirectX 11 video memory running with 4 async tasks explicitely.
        15) simple_encode_d3d11_lowlat: This example encodes the input video with the configuration of DirectX 11 video memory and low latency configuration. It also measures and shows the latency of the first 1000 frames.
        16) simple_encode_d3d11_preproc: This example inputs the video with RGB format, it pre-processes the input to NV12--the native color format of the Media SDK and then encodes it. The example is configured with DirectX 11 video memory.
        17) simple_encode_ffmpeg: This example shows the encoder integrated with the FFmpeg mux model to output a video with the container.
        18) simple_encode_hevc10: This example encodes the 10bit YUV raw video to HEVC 10bit stream.
        19) simple_session: This basic application just creates a Media SDK session.
        20) simple_transcode: This is the basic transcoding example to transcode an input H.264 stream to a H.264 stream by using the system memory.
        21) simple_transcode_d3d: This is the basic transcoding example to transcode an input H.264 stream to a H.264 stream by using the DirectX 9 memory.
        22) simple_transcode_d3d11: This is the basic transcoding example to transcode an input H.264 stream to a H.264 stream by using the DirectX 11 memory.
        23) simple_transcode_opaque: This is the basic transcoding example to transcode an input H.264 stream to a H.264 stream by using the Media SDK opaque memory management.
        24) simple_transcode_opaque_async: This example transcodes an input H.264 stream to a H.264 stream in 4 async tasks with the Media SDK opaque memory management.
        25) simple_transcode_opaque_async_vppresize: This example transcodes an input H.264 stream to a H.264 stream and resized to 1/4 in both height and width, the VPP works in 4 async tasks with the Media SDK opaque memory management.
        26) simple_transcode_opaque_lowlat: This is the basic transcoding example to transcode an input H.264 stream to a H.264 stream by using the Media SDK opaque memory management. The example is configured to low latency with measuring the latency for the first 1000 frames.
        27) simple_vpp: This example shows how to downsize and denoise of the input video by using the system memory.
        28) simple_vpp_d3d: This example shows how to downsize and denoise of the input video by using the DirectX 9 video memory.
        29) simple_vpp_d3d11: This example shows how to downsize and denoise of the input video by using the DirectX 11 video memory.

3. The Media SDK programming model.
    1. The Media SDK input data is the elementary stream, it can not input the standard video container, the user need to use the other tools to convert the video to the ES format, for example, ffmpeg..
    2. The general programming process starts by creating a Media SDK session and creates the pipeline components in it, then to configure the media pipeline, allocate the memory for the intermediate frame memory and then control the execution of the pipeline at the runtime.
    3. Refer to the README.txt file in each project for the partcular feature usage.
