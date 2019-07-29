1. Introduction
    This example shows to decode the H.264 video stream; to improve the decoding performance, it uses system memory surfaces as the working memory; it also adds the video post-processing(VPP) component to showcase the resizing.
2. To run the example, use the following command:
    ~\_build\x64\Debug>simple_decode_d3d.exe -hw input.264 output.yuv
    Frame number: 14315
    Execution time: 49.76 s (287.67 fps)

    NOTE:
        1) Use "-sw" for the software mode.
        2) The default mode is "-auto"
        3) The output file can be empty so the example will run the decode only.
3. Design
    This example shows the minimum API usage to decode the H.264 stream. It uses the system memory to allocate the frame memory for the decoding process, this is the working memory for the decoder called "surface". It also adds the VPP usage to the code. The decoding and video processing API is an asynchronized call and much faster, so the surface allocation creates a pool for better utilization.
    It has the following steps:
    1) Parse the argument of the command line, then open the file I/O objects.
    2) Create the Media SDK session.
    3) Create the decoder and VPP objects.
    4) Configure the basic video parameters, noticed the IOPattern is set to MFX_IOPATTERN_OUT_SYSTEM_MEMORY for the system memory.
    5) Allocate the bit stream buffer required, read the ES header from the input file and fill the rest of the video parameters.
    6) Configure the VPP video parameters based on the decoder video parameters.
    7)  Allocate the surfaces(video frame working memory) with the video memory required by the VPP, notice we need to allocate both input and output surfaces.
    5) Allocate the surfaces(video frame working memory) with the video memory required by the decoder.
    6) Initialize the decoder and VPP modules;
    7) Start the decoding/VPP loop
        Stage1: Main decoding loop, to decode from the input bit stream and then process the output with the VPP module.
        Stage2: Retrieve the buffered decoded frames; after the end of the input stream, flush the cached stream data for both decoding and VPP.
        Stage3: Retrieve the buffered VPP frames; flush the cached frame data frame for VPP.
    8) Clean up the resources and close the session.
