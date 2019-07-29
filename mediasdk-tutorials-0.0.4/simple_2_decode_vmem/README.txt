1. Introduction
    This example shows to decode the H.264 video stream. To improve the decoding performance, it uses video memory surfaces to avoid the memory copying. Since the video memory management is different among the different OS and frameworks, there are 3 kind of binaries being build: DirectX9, DirectX11 and libVA surface.
2. To run the example, use the following command:
    ~\_build\x64\Debug>simple_decode_d3d.exe -hw input.264 output.yuv
    Frame number: 14315
    Execution time: 60.07 s (238.32 fps)

    ~\_build\x64\Debug>simple_decode_d3d11.exe -hw input.264 output.yuv
    Frame number: 14315
    Execution time: 64.46 s (222.09 fps)

    [~/_build]$ ./simple_decode_vmem ~/Videos/bb320.264 output.yuv
    libva info: VA-API version 0.99.0
    libva info: va_getDriverName() returns 0
    libva info: User requested driver 'iHD'
    libva info: Trying to open /opt/intel/mediasdk/lib64/iHD_drv_video.so
    libva info: Found init function __vaDriverInit_0_32
    libva info: va_openDriver() returns 0
    Frame number: 14315
    Execution time: 82.27 s (173.99 fps)

    NOTE:
        1) Use "-sw" for the software mode.
        2) The default mode is "-auto"
        3) The output file can be empty so the example will run the decode only.
3. Design
    This example shows the minimum API usage to decode the H.264 stream. It uses the video memory to allocate the frame memory for the decoding process, this is the working memory for the decoder called "surface". Since the decoder accesses the video memory directly, this avoids the extra copying for each frame, so the performance is much better. The decoding API is an asynchronized call and much faster, so the surface allocation creates a pool for better utilization.
    It has the following steps:
    1) Parse the argument of the command line, then open the file I/O objects.
    2) Create the Media SDK session, noticed the mfxFrameAllocator is initialized here while there is a NULL when using the system memory.
    3) Configure the basic video parameters, noticed the IOPattern is set to MFX_IOPATTERN_OUT_VIDEO_MEMORY for the video memory.
    4) Allocate the bit stream buffer required, read the ES header from the input file and fill the rest of the video parameters.
    5) Allocate the surfaces(video frame working memory) with the video memory required by the decoder, noticed the allocating method is different from the basic decode tutorial code.
    6) Initialize the decoder
    7) Start the decoding loop
        Stage1: Main decoding loop, to decode from the input bit stream.
        Stage2: Retrieve the buffered decoded frames; after the end of the input stream, flush the cached stream data from the surface pool.
    8) Clean up the resources and close the session.
