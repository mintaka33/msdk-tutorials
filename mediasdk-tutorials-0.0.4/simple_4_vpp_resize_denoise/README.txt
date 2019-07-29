1. Introducation
    This simple showcases VPP using system memory surfaces. Downscaled frame resize to 1/4 and denoise filter processing.

2. To run the example, use the following command:
    On Windows:
    ~\_build\x64\Debug>simple_vpp.exe -g 1920x1080 input.yuv output.yuv
    Frame number: 200
    Execution time: 8.17 s (24.49 fps)

    ~/_build$ ./simple_vpp -g 1920x1080 input.yuv output.yuv
    libva info: VA-API version 0.99.0
    libva info: va_getDriverName() returns 0
    libva info: User requested driver 'iHD'
    libva info: Trying to open /opt/intel/mediasdk/lib64/iHD_drv_video.so
    libva info: Found init function __vaDriverInit_0_32
    libva info: va_openDriver() returns 0
    Frame number: 200
    Execution time: 2.81 s (71.20 fps)

    NOTE:
        1) Use "-sw" for the software mode.
        2) The default mode is "-auto"
        3) The output file can be empty if no output file parameter is given
3. Design
    This example shows the minimum API usage to show frame resize and denoise
    filter processing. It uses the system memory to allocate the frame memory,
    this is the working memory called "surface". The MSDK API is an asynchronized call and much faster, so the surface allocation creates a pool for better utilization.
    It has the following steps:
    1) Parse the argument of the command line, then open the file I/O objects.
    2) Create the Media SDK session.
    3) Configure the VPP video parameters with downscaling and 30/1 frame rate, and create the VPP component, configured system memory as the working memory.
    4) Allocate the VPP input and output surfaces(video frame working memory) with the system memory, also initialize the extended buffer for denoise.
    5) Initialize the MSDK vpp component
    6) Start the VPP processing loop
        Stage1: Main processing loop, to process from the input yuv stream.
        Stage2: Retrieve the buffered VPP frames; after the end of the input stream, flush the cached stream data for vpp processing.
    7) Clean up the resources and close the session.

