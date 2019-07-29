1. Introduction
    This example shows how to encode the YUV YV12 10bit file, it works on 7th Generation Intel(R) Core(TM) processors(KabyLake) with HW HEVC encode. It should work with both HW and SW mode on KabyLake while the SW mode only for SkyLake processors with the professional edition. The code also shows the plugin load process by the MFXVideoUSER_Load() function.
    To prepare the YUV video content, the simple_decode_hevc10 can be run or the FFMpeg can be used to uncompress the YUV frames. Before run the simple_encode_hevc10 binary, use "ffprobe" to check if the input stream is YV12 10bit.

2. To run the program, use the following command:
   > ~\_build\x64\Debug>simple_encode_hevc10.exe -g 1920x1080x10 -b 20000 -f 24/1 -hw output.yuv output.265
    Loading HEVC plugin: 6fadc791a0c2eb479ab6dcd5ea9da347
    Frame number: 200
    Execution time: 3.39 s (58.98 fps)

    NOTE:
        1) If running under the professional edition, use "-sw" for the software mode.
        2) The default mode is "-auto"
        3) The output file can be empty so the example will run the encode only.

3. Design
This example shows the minimum API usage to encode the YV12 10bit frames into the HEVC 10bit stream. The same as the simple_encode sample, it uses the system memory to allocate the frame memory for the encoding, this is the working memory for the encoder called "surface". The encoding API is an asynchronized call and much faster, so the surface allocation creates a pool for better utilization.
    It has the following steps:(Aligned to the source code for details)
    1) Read and parse arguments from the command line.
    2) Initialize the Media SDK session and Media SDK encoder.
    3) Set required video parameters for encoding and fill up the parameters with the user input arguments.
    4) Load the HEVC plugin since it is not included in the MFX library
    5) Allocate the surfaces(working frame memory) for the encoder.
    6) Initialize the encoder.
    7) Prepare the bit stream buffer for input.
    8) Start the encoding loops
        Stage 1: Main encoding loop, noted the read function is specilized to read 10bit data from the YUV file.
        Stage 2: After the input bit stream is empty, retrieve the buffered encoded frames in the hardware.
    9) Clean up the resources.
