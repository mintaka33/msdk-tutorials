1. Introducation
    This simple showcases VPP using system memory surfaces. Downscaled frame resize to 1/4 and denoise filter processing.
    But adds use of video memory surfaces for improved VPP performance. 

2. To run the example, use the following command:
    On Windows:
    ~\_build\x64\Debug>simple_vpp_d3d.exe -g 1920x1080 input.yuv output.yuv
    Frame number: 200
    Execution time: 8.50 s (23.54 fps)

    ~\_build\x64\Debug>simple_vpp_d3d11.exe -g 1920x1080 input.yuv output.yuv
    Frame number: 200
    Execution time: 11.15 s (17.93 fps)

    On Linux:
    $ ./simple_vpp_vmem -g 1920x1080 input.yuv output.yuv
    libva info: VA-API version 0.99.0
    libva info: va_getDriverName() returns 0
    libva info: User requested driver 'iHD'
    libva info: Trying to open /opt/intel/mediasdk/lib64/iHD_drv_video.so
    libva info: Found init function __vaDriverInit_0_32
    libva info: va_openDriver() returns 0
    Frame number: 200
    Execution time: 10.84 s (18.45 fps)
    
    NOTE:
        1) Use "-sw" for the software mode.
        2) The default mode is "-auto"
        3) The output file can be empty if no output file parameter is given
3. Design
    This example shows the minimum API usage to show frame resize and denoise filter processing. It uses the video memory to allocate the frame memory,
    this is the working memory called "surface". The MSDK API is an asynchronized call and much faster, so the surface allocation creates a pool for better utilization.
    It has the following steps:
    1) Parse the argument of the command line, then open the file I/O objects.
    2) Create the Media SDK session with the video memory allocator.
    3) Configure the VPP video parameters with downscaling and 30/1 frame rate, and create the VPP component, configured video memory as the working memory.
    4) Allocate the VPP input and output surfaces(video frame working memory) with the video memory, also initialize the extended buffer for denoise.
    5) Initialize the MSDK vpp component
    6) Start the VPP processing loop
        Stage1: Main processing loop, to process from the input yuv stream.
        Stage2: Retrieve the buffered VPP frames; after the end of the input stream, flush the cached stream data for vpp processing.
    7) Clean up the resources and close the session.

