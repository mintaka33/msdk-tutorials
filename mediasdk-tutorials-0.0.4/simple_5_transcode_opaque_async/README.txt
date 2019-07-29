1. Introducation
    Transcodes (decode+encode) H.264 stream to another H.264 stream using system memory surfaces. The program decodes the input frame into the YV12 frame which is the input of the encoder; the encoder encodes the YV12 frame into the H.264 frame with the user input bitrate and frame rate while set the bitrate control to Variable Bitrate(VBR).
    But uses the Intel Media SDK opaque memory feature. The opaque memory type hides surface allocation specifics and allows the Intel Media SDK to select the best type for the execution in hardware or software. And adds asynchronous operation to the transcode pipeline implementation, resulting in further improved performance.

2. To run the example, use the following command:
    On Windows:
    ~\_build\x64\Debug>simple_transcode_opaque_async.exe -b 20000 -f 30/1 input.h264 output.h264
    Frame number: 100
    Execution time: 0.6l s (154.35 fps)

    On Linux:
    $ ./simple_transcode_opaque_async -b 20000 -f 30/1 input.h264 output.h264
    libva info: VA-API version 0.99.0
    libva info: va_getDriverName() returns 0
    libva info: User requested driver 'iHD'
    libva info: Trying to open /opt/intel/mediasdk/lib64/iHD_drv_video.so
    libva info: Found init function __vaDriverInit_0_32
    libva info: va_openDriver() returns 0
    Frame number: 100
    Execution time: 0.44 s (226.44 fps)
    
    NOTE:
        1) Use "-sw" for the software mode.
        2) The default mode is "-auto"
        3) The output file can be empty so the example will run the transcode only.

3. Design
    This example shows the minimum API usage to transcodes (decode+encode) H.264 stream to another H.264 stream. It uses the system memory with opaque memory feature to allocate the frame memory, adds asynchronous operation to the transcode pipeline implementation.
    This is the working memory called "surface". The MSDK API is an asynchronized call and much faster, so the surface allocation creates a pool for better utilization.
    It has the following steps:
    1) Parse the argument of the command line, then open the file I/O objects.
    2) Create the Media SDK session, and then create the encoder and decoder.
    3) Configure the decoder video parameters to H.264 and opeque memory. Assign the decoder async depth.
    4) Allocate the bit stream buffer for decoder input, read the ES header from the input file and fill the rest of the decoder video parameters.
    5) Configure the encoder video parameter with the input argument and configures the VBR, configure the opaque memory. Assign the encoder async depth to the same as the decoder async depth.
    6) After creating the mfxExtOpaqueSurfaceAlloc, allocate the surfaces(video frame working memory) with the opaque memory for decoder and encoder.
    7) Initialize the MSDK decode and encode component.
    8) Create a task pool for the async tasks, for each task, allocate the bit stream buffer required by the output file.
    9) Start the transcode processing loop
        Stage1: Main transcode (decode+encode) loop, to decode from the input bit stream and encode to the output bit stream until the end of the input bit stream. The output bit stream is only executed when all the task is completed.
        Stage2: Retrieve the buffered decoded frames and flush the cached stream data for decoding, and then encode to the output bit stream. The output bit stream is only executed when all the task is completed.
        Stage3: Retrieve the buffered encoded frame; flush the cached stream data for encoding to the output bit stream. The output bit stream is only executed when all the task is completed.
	Stage4: Sync all remaining tasks in task pool, only flash the output bit stream.
    10) Clean up the resources and close the session, noted that the surface was handled by the Media SDK automatically.

