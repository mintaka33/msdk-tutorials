1. Introduction
    This example shows to preprocess the YV12 frames before encoding it into H.264 stream using surfaces in video memory. To generate the input YUV file, the tutorial simple_encode can be used; or the FFMpeg can also be used. To improve the encoding performance, it uses video memory surfaces to avoid the memory copying. Since the video memory management is different among the different OS and frameworks, there are 3 kind of binaries being build: DirectX9, DirectX11 and libVA surface. And adds VPP pre-processing capabilities to show frame color conversion from RGB32(4) to NV12.

Since the tutorial required the RGB32 format, please use the following ffmpeg command to convert:
    > ffmpeg -i input.mp4 -pix_fmt rgba -c:v rawvideo -map 0:v -f rawvideo -vframes 200 output.rgb4

2. To run the example, use the following command:
    ~\_build\x64\Debug>simple_encoded_d3d_preproc.exe  -g 1920x1080 -b 2000 -f 24/1 -hw input.rgb4 output.264
    Frame number: 200
    Execution time: 1.78 s (112.40 fps)

    ~\_build\x64\Debug>simple_encoded_d3d11_preproc.exe  -g 1920x1080 -b 2000 -f 24/1 -hw input.rgb4 output.264
    Frame number: 200
    Execution time: 1.57 s (127.21 fps)

    [~/_build]$ ./simple_encode_vmem_preproc -g 1920x1080 -b 20000 -f 24/1 input.rgb4 output.264
    libva info: VA-API version 0.99.0
    libva info: va_getDriverName() returns 0
    libva info: User requested driver 'iHD'
    libva info: Trying to open /opt/intel/mediasdk/lib64/iHD_drv_video.so
    libva info: Found init function __vaDriverInit_0_32
    libva info: va_openDriver() returns 0
    Frame number: 200
    Execution time: 0.92 s (216.35 fps)

    NOTE:
        1) Use "-sw" for the software mode.
        2) The default mode is "-auto"
        3) The output file can be empty so the example will run the encode only.

3. Design
    This example shows the minimum API usage to encode the YUV stream(YV12) into the H.264 stream. It uses the video memory to allocate the frame memory for the encoding process, this is the working memory for the encoder called "surface". Since the encoder accesses the video memory directly, this avoids the extra copying for each frame, so the performance is much better. The encoding API is an asynchronized call and much faster, so the surface allocation creates a pool for better utilization.
    It has the following steps:
    1) Parse the argument of the command line, then open the file I/O objects.
    2) Create the Media SDK session, and then create the Media SDK encoder object, noticed the mfxFrameAllocator is initialized here while there is a NULL when using the system memory.
    3) Configure the video parameters for encoder based on the user input, then validate the video parameters from the encoder object, noticed the IOPattern is set to MFX_IOPATTERN_OUT_VIDEO_MEMORY for the video memory.
    4) Initialize the video parameters for VPP based on the encoder video parameters and user input.
    5) Create the encoder and VPP objects, then validate the initialized video parameters.
    6) Allocate the surfaces for VPP, noticed we don't allocate the surface for the encoder since the VPP output surface is sharing with the encoder input.
    7) Configure the extended VPP buffer for denoise.
    8) Initialize the encoder and VPP objects.
    9) Allocate the bit stream buffer required by the encoder input.
    10) Start the encoding loop
        Stage1: Main VPP/encoding loop, to VPP/encode from the input bit stream.
        Stage2: Retrieve the buffered VPP frames.
        Stage3: Retrieve the buffered encoded frames; after the end of the input stream, flush the cached stream data for encoding.
        
    11) Clean up the resources and close the session.
