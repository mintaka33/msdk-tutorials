1. Introduction
    This example shows to decode the H.264 video stream
2. To run the example, use the following command:
    ~\_build\x64\Debug>simple_decode.exe -hw input.264 output.yuv
    Frame number: 100
    Execution time: 19.57 s (5.11 fps)

    NOTE:
        1) Use "-sw" for the software mode.
        2) The default mode is "-auto"
        3) The output file can be empty so the example will run the decode only.
3. Design
    This example shows the minimum API usage to decode the H.264 stream. It uses the system memory to allocate the frame memory for the decoding, this is the working memory for the decoder called "surface". The decoding API is an asynchronized call and much faster, so the surface allocation creates a pool for better utilization.
    It has the following steps:
    1) Parse the argument of the command line, then open the file I/O objects.
    2) Create the Media SDK session.
    3) Configure the basic video parameters.
    4) Allocate the bit stream buffer required, read the ES header from the input file and fill the rest of the video parameters.
    5) Allocate the surfaces(video frame working memory) with the system memory required by the decoder.
    6) Initialize the decoder
    7) Start the decoding loop
        Stage1: Main decoding loop, to decode from the input bit stream.
        Stage2: Retrieve the buffered decoded frames; after the end of the input stream, flush the cached stream data for decoding.
    8) Clean up the resources and close the session.
