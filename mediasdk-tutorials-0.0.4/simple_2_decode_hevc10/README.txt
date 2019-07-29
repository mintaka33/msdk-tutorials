1. Introduction
    This example shows to decode the HEVC 10bit video stream, it works on 7th Generation Intel(R) Core(TM) processors(KabyLake) with HW HEVC decode. It should work with both HW and SW mode on KabyLake while the SW mode only for SkyLake processors. The code also shows the plugin load process by the MFXVideoUSER_Load() function.
    To prepare the video content, please do following steps:
        1) Go to http://jell.yfish.us/ and select a HEVC/Main10 video file
        2) Use the following command to extract the elementary stream from the video file:
           >ffmpeg -i input.mkv -vcodec copy -an -bsf hevc_mp4toannexb -vframes 900 output.265
        3) Before run the simple_decode_hevc10 binary, use "ffprobe" to check if the input stream is HEVC Main 10.

2. To run the program, use the following command:
   > \_build\x64\Debug>simple_decode_hevc10.exe -hw output.265 output.yuv
     Loading HEVC plugin: 33a61c0b4c27454ca8d85dde757c6f8e
     Frame number: 900 
     Execution time: 209.73 s (4.29 fps)

    NOTE:
        1) Use "-sw" for the software mode.
        2) The default mode is "-auto"
        3) The output file can be empty so the example will run the decode only.

3. Design
This example shows the minimum API usage to decode the H.264 stream. The same as the simple_decode sample, it uses the system memory to allocate the frame memory for the decoding, this is the working memory for the decoder called "surface". The decoding API is an asynchronized call and much faster, so the surface allocation creates a pool for better utilization.
    It has the following steps:(Aligned to the source code for details)
    1) Read and parse options from the command line.
    2) Initialize the Media SDK session.
    3) Set required video parameters for decoding.
    4) Load the HEVC plugin since it is not included in the MFX library
    5) Prepare the bit stream buffer for input, read the input stream header and fill up the rest of the video parameters for the decoder.
    6) Allocate the surfaces(working frame memory) for the decoder.
    7) Initialize the decoder and start the decoding loops
        Stage 1: Main decoding loop, noted the write function is specilized to write 10bit data to the YUV file.
        Stage 2: After the input bit stream is empty, retrieve the buffered decoded frames in the hardware.
    8) Clean up the resources.
