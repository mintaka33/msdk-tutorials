1. Introducation
   Showcases MSDK features via call Query functions (MFXVideoDECODE_Query/MFXVideoENCODE_Query/MFXVideoVPP_Query) 

2. The tutorial can be run with the "-hw", "-sw", "-auto" or no options, it will report the capability for each codec.

3. Design.
    Since this tutorial check all supported coded, it will configure a set of the video parameters for each codec by the predefined filling function. Each filling function will try the maximum resolutions, user might change the resolution based on his platform. 

    It will go through the following steps:
    1) Check and print the platform information, this step only works on Windows.
    2) Create a Media SDK session.
    3) Run a loop on all the registered decoders
        3.1) Fill the video parameter structure for the decoder.
        3.2) For the decoder with the plugin, load it.
        3.3) Validate each decoder with the filled video parameters, and then try to initialize it.
        3.4) Close the decoder, unload the plugin and report the results.
    4) Run a loop on all the registered encoders
        4.1) Fill the video parameter structure for the encoder.
        4.2) For the encoder with the plugin, load it.
        4.3) Validate each encoder with the filled video parameters, and then try to initialize it.
        4.4) Close the encoder, unload the plugin and report the results.
