1. Introduction
    This example shows to encode the YUV frames from file into H.264 stream using surfaces in video memory, showcasing asynchronous encode pipeline flow by running multiple tasks at the same time. To get the input YUV file, the tutorial simple_encode can be used; or the FFMpeg can also be used. To improve the encoding performance, it uses video memory surfaces to avoid the memory copying. Since the video memory management is different among the different OS and frameworks, there are 3 kind of binaries being build: DirectX9, DirectX11 and libVA surface. The asynchronous operation results in further improved performance.

2. To run the example, use the following command:
    ~\_build\x64\Debug>simple_encoded_d3d_async.exe  -g 1920x1080 -b 2000 -f 24/1 -hw input.yuv output.264
    Frame number: 200
    Execution time: 1.19 s (168.41 fps)

    ~\_build\x64\Debug>simple_encoded_d3d11_async.exe  -g 1920x1080 -b 2000 -f 24/1 -hw input.yuv output.264
    Frame number: 200
    Execution time: 1.24 s (160.85 fps)

    [~/_build]$ ./simple_encode_vmem_async -g 1920x1080 -b 20000 -f 24/1 input.yuv output.264
    libva info: VA-API version 0.99.0
    libva info: va_getDriverName() returns 0
    libva info: User requested driver 'iHD'
    libva info: Trying to open /opt/intel/mediasdk/lib64/iHD_drv_video.so
    libva info: Found init function __vaDriverInit_0_32
    libva info: va_openDriver() returns 0
    Frame number: 200
    Execution time: 1.00 s (200.52 fps)

    NOTE:
        1) Use "-sw" for the software mode.
        2) The default mode is "-auto"
        3) The output file can be empty so the example will run the encode only.

3. Design
    This example shows the minimum API usage to encode the YUV stream(YV12) into the H.264 stream. It uses the video memory to allocate the frame memory for the encoding process, this is the working memory for the encoder called "surface". Since the encoder accesses the video memory directly, this avoids the extra copying for each frame, so the performance is much better. The encoding API is an asynchronized call and much faster, so the surface allocation creates a pool for better utilization.
    It has the following steps:
    1) Parse the argument of the command line, then open the file I/O objects.
    2) Create the Media SDK session, and initialize the mfxFrameAllocator, the video memory type is decided here.
    3) Configure the video parameters based on the user input, then validate the video parameters from the encoder object, noticed the IOPattern is set to MFX_IOPATTERN_OUT_VIDEO_MEMORY for the video memory.
    4) Assign the async depth to 4, create the Media SDK encoder object, and then validate the video parameters.
    5) Allocate the surfaces(video frame working memory) with the video memory required by the encoder, noticed the allocating method is different from the basic encode tutorial code.
    6) Initialize the encoder
    7) Create a task pool for the async tasks, for each task, allocate the bit stream buffer required by the output file.
    8) Start the encoding loop, in this tutorial, the sync point is different in each task.
        Stage 1: Main encoding loop, to encode from the input bit stream, the output bit stream is only executed when all the task is completed.
        Stage 2: Retrieve the buffered encoded frames; after the end of the input stream, flush the cached stream data for encoding, the bit stream output logic is the same as Stage 1..
        Stage 3: Sync all remaining tasks in task pool, only flush the bit stream output.
    8) Clean up the resources and close the session.
