1. Introduction
    This example shows to encode the YUV frames from file into H.264 stream using surfaces in video memory while configures the low latency pipeline by forcing the I/P frame only. To get the input YUV file, the tutorial simple_encode can be used; or the FFMpeg can also be used. To improve the encoding performance, it uses video memory surfaces to avoid the memory copying. Since the video memory management is different among the different OS and frameworks, there are 3 kind of binaries being build: DirectX9, DirectX11 and libVA surface. Add additional code to illustrate how to configure an encode pipeline for low latency and how to measure latency.

2. To run the example, use the following command:
    ~\_build\x64\Debug>simple_encoded_d3d_lowlat.exe  -g 1920x1080 -b 2000 -f 24/1 -hw input.yuv output.264
    Frame number: 200
    Execution time: 2.96 s (67.64 fps)
    Latency: AVG=  9.7 ms, MAX= 14.6 ms, MIN=  6.3 ms

    ~\_build\x64\Debug>simple_encoded_d3d11_lowlat.exe  -g 1920x1080 -b 2000 -f 24/1 -hw input.yuv output.264
    Frame number: 200
    Execution time: 3.25 s (61.47 fps)
    Latency: AVG= 11.0 ms, MAX= 16.0 ms, MIN=  7.2 ms

    [~/_build]$ ./simple_encode_vmem_lowlat -g 1920x1080 -b 20000 -f 24/1 input.yuv output.264
    libva info: VA-API version 0.99.0
    libva info: va_getDriverName() returns 0
    libva info: User requested driver 'iHD'
    libva info: Trying to open /opt/intel/mediasdk/lib64/iHD_drv_video.so
    libva info: Found init function __vaDriverInit_0_32
    libva info: va_openDriver() returns 0
    Frame number: 200
    Execution time: 1.84 s (108.41 fps)
    Latency: AVG=  5.0 ms, MAX=  7.2 ms, MIN=  3.3 ms

    NOTE:
        1) Use "-sw" for the software mode.
        2) The default mode is "-auto"
        3) The output file can be empty so the example will run the encode only.
        4) Use "--no-measure-latency" for not measurement; the default is "--measure-latency"

3. Design
    This example shows the minimum API usage to encode the YUV stream(YV12) into the H.264 stream. The low latency configuration happens at the encoder initialization stage. The same as the other encode tutorials, it uses the video memory to allocate the frame memory for the encoding process, this is the working memory for the encoder called "surface". Since the encoder accesses the video memory directly, this avoids the extra copying for each frame, so the performance is much better. The encoding API is an asynchronized call and much faster, so the surface allocation creates a pool for better utilization.
    It has the following steps:
    1) Parse the argument of the command line, then open the file I/O objects.
    2) Create the Media SDK session and initialize the mfxFrameAllocator, the video memory type is decided during the initialization.
    3) Configure the video parameters based on the user input, then validate the video parameters from the encoder object, noticed the IOPattern is set to MFX_IOPATTERN_OUT_VIDEO_MEMORY for the video memory.
    4) Configure the low latency: set both the async depth and GOP ref distance to 1, this will force the I and P frame only.
    5) Create the encoder and validate the video parameters in the previous steps.
    6) Allocate the surfaces(video frame working memory) with the video memory required by the encoder, noticed the allocating method is different from the basic encode tutorial code.
    7) Initialize the encoder
    8) Allocate the bit stream buffer required by the encoder input.
    9) Start the encoding loop
        Stage1: Main encoding loop, to encode from the input bit stream.
        Stage2: Retrieve the buffered encoded frames; after the end of the input stream, flush the cached stream data for encoding.
    10) Print out the latency measurement result.
    11) Clean up the resources and close the session.
