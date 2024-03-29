# msdk-tutorials

## Intel Media SDK development tutorials:

https://software.intel.com/en-us/intel-media-server-studio-support/training

## Build

```bash
export MFX_HOME=/opt/intel/mediasdk
cd mediasdk-tutorials-0.0.4
make
```
## Description

| Title  | Description  |
|---|---|
|  Set Up |  **simple_1_session** <br> Establishes an Intel Media SDK session. Perform queries to determine selected implementation and the API version.<br> **simple_7_codec** <br> Checks the runtime codec support on the current platform and iterates all supported codes (on each codec) for configuring, querying, and reporting. |
| Decode  | **simple_2_decode** <br> Transforms an AVC stream into a .yuv file using system memory surfaces, showcasing a simple synchronous decode pipeline flow. <br> **simple_2_decode_vmem** <br> Adds the ability to use video memory surfaces for improved decode performance. <br> **simple_2_decode_ffmpeg** <br> By integrating the demux function of FFmpeg, this sample decodes an AVC stream in the mp4 container into a .yuv file using system memory surfaces, showcasing a simple synchronous decode pipeline flow. <br> **simple_2_decode_hevc10** <br> Decodes a HEVC stream with 10-bit depth into a .yuv file using system memory surfaces, showcasing a simple synchronous decode pipeline flow. |
|  Encode |  **simple_3_encode** <br> Converts .yuv frames from a file into an AVC stream using surfaces in system memory, showcasing simple synchronous encode pipeline flow. <br> **simple_3_encode_vmem** <br> Adds the ability to use video memory surfaces for improved encode performance. <br> **simple_3_encode_vmem_async** <br> Includes an asynchronous operation to the previous example, resulting in further improved performance. <br> **simple_3_encode_ffmpeg** <br> By integrating the mux function of FFmpeg, this sample encodes .yuv frames from a file into an mp4 container with the AVC stream using surfaces in system memory, showcasing a simple synchronous encode pipeline flow. <br> **simple_3_encode_hevc10** <br> Encodes .yuv frames with 10-bit color depth from a file into an HEVC stream using surfaces in system memory, showcasing a simple synchronous encode pipeline flow. |
|  Transcode | **simple_5_transcode** <br> Transcodes (decodes and encodes) an AVC stream to another AVC stream using system memory surfaces. <br> **simple_5_transcode_opaque** <br> Like the simple_5_transcode example, it transcodes an AVC stream but uses the opaque memory feature in the SDK. It hides surface allocation specifics while selecting the best type for hardware or software execution. <br> **simple_5_transcode_vmem** <br> Same as simple_5_transcode sample but instead uses video memory surfaces. While opaque surfaces use video memory internally, application-level video memory allocation is required to integrate components that are not in the SDK. <br> **simple_5_transcode_opaque_async** <br> Adds asynchronous operation to an implementation of the transcode pipeline, resulting in additional improved performance. <br> **simple_5_transcode_opaque_async_vppresize** <br> Similar to the simple_5_transcode_opaque_async sample, but instead, this pipeline includes a resize of video frame processing (VPP).  |
|  Video Processing and More | **simple_4_vpp_resize_denoise** <br> Showcases VPP using system memory surfaces. Highlights frame resize and denoise filter processing. <br> **simple_4_vpp_resize_denoise_vmem** <br> Adds the ability to use video memory surfaces for improved VPP performance. <br> **simple_6_decode_vpp_postproc** <br> Similar to the simple_2_decode sample, while adding VPP post-processing capabilities to showcase resizing videos and the processing amplifier (ProcAmp). <br> **simple_6_encode_vmem_lowlatency** <br> Like the simple_3_encode_vmem sample, but with additional code to illustrate how to configure an encode pipeline for low latency and how to measure latency. <br> **simple_6_transcode_opaque_lowlatency** <br> Same as the simple_5_transcode_opaque sample, but with additional code that illustrates how to configure a transcode pipeline for low latency and how to measure latency. <br> **simple_6_encode_vmem_vpp_preproc** <br> Similar to the simple_3_encode_vmem sample, but adds VPP preprocessing capabilities that show frame color conversion from RGB32(4) to NV12.  |
