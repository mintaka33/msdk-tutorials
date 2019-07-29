1. Introduction
    This example shows to encode the YUV frames from file into H.264 stream using surfaces in system memory, showcasing simple synchronous encode pipeline flow. to get the input YUV file, the tutorial simple_encode can be used; or the FFMpeg can also be used.
2. To run the example, use the following command:
    ~\_build\x64\Debug>simple_encode.exe  -g 1920x1080 -b 2000 -f 24/1 -hw input.yuv output.264
    Frame number: 200
    Execution time: 1.14 s (175.92 fps)

    NOTE:
        1) Use "-sw" for the software mode.
        2) The default mode is "-auto"
        3) The output file can be empty so the example will run the encode only.
3. Design
    This example shows the minimum API usage to encode the YUV stream(YV12) into the H.264 stream. It uses the system memory to allocate the frame memory for the encoding, this is the working memory for the encoder called "surface". The encoding API is an asynchronized call and much faster, so the surface allocation creates a pool for better utilization.
    It has the following steps:
    1) Parse the argument of the command line, then open the file I/O objects.
    2) Create the Media SDK session, and then create the Media SDK encoder object.
    3) Configure the video parameters based on the user input, then validate the video parameters from the encoder object.
    4) Allocate the surfaces(video frame working memory) with the system memory required by the encoder.
    5) Initialize the encoder
    6) Allocate the bit stream buffer required by the encoder input.
    7) Start the encoding loop
        Stage1: Main encoding loop, to encode from the input bit stream.
        Stage2: Retrieve the buffered encoded frames; after the end of the input stream, flush the cached stream data for encoding.
    8) Clean up the resources and close the session.
