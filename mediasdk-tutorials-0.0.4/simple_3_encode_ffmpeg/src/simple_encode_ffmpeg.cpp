/*****************************************************************************

INTEL CORPORATION PROPRIETARY INFORMATION
This software is supplied under the terms of a license agreement or
nondisclosure agreement with Intel Corporation and may not be copied
or disclosed except in accordance with the terms of that agreement.
Copyright(c) 2005-2014 Intel Corporation. All Rights Reserved.

*****************************************************************************/


#include "common_utils.h"
#include "cmd_options.h"
#include "ffmpeg_utils.h"
#include <vector>
#include <string>

static void usage(CmdOptionsCtx* ctx)
{
    printf(
        "Encodes INPUT and optionally writes OUTPUT. If INPUT is not specified\n"
        "simulates input with empty frames filled with the color.\n"
        "\n"
        "Usage: %s [options] [INPUT] [OUTPUT]\n", ctx->program);
}

int main(int argc, char** argv)
{
    mfxStatus sts = MFX_ERR_NONE;
    bool bEnableInput;  // if true, removes all YUV file reading (which is replaced by pre-initialized surface data). Workload runs for 1000 frames.
    bool bEnableOutput; // if true, removes all output bitsteam file writing and printing the progress
    CmdOptions options;

    // =====================================================================
    // Intel Media SDK encode pipeline setup
    // - In this example we are encoding a YV12 video stream to a standard container file
    // - For simplistic memory management, system memory surfaces are used to store the raw frames
    //   (Note that when using HW acceleration video surfaces are prefered, for better performance)
    //

    //1. Read options from the command line (if any is given)
    memset(&options, 0, sizeof(CmdOptions));
    options.ctx.options = OPTIONS_ENCODE;
    options.ctx.usage = usage;
    // Set default values:
    options.values.impl = MFX_IMPL_AUTO_ANY;

    // here we parse options
    ParseOptions(argc, argv, &options);

    //if (options.values.SourceName[0] == '\0' || options.values.SinkName[0] == '\0') {
    //    printf("error: both the input file and output file not set (mandatory)\n");
    //    return -1;
    //}
    if (!options.values.Width || !options.values.Height) {
        printf("error: input video geometry not set (mandatory)\n");
        return -1;
    }
    if (!options.values.Bitrate) {
        printf("error: bitrate not set (mandatory)\n");
        return -1;
    }
    if (!options.values.FrameRateN || !options.values.FrameRateD) {
        printf("error: framerate not set (mandatory)\n");
        return -1;
    }

    bEnableInput = (options.values.SourceName[0] != '\0');
    bEnableOutput = (options.values.SinkName[0] != '\0');

    // Open input YV12 YUV file
    FILE* fSource = NULL;
    if (bEnableInput) {
        MSDK_FOPEN(fSource, options.values.SourceName, "rb");
        MSDK_CHECK_POINTER(fSource, MFX_ERR_NULL_PTR);
    }

    muxControl muxCtrl;
    if (bEnableOutput) {
        // =========== ffmpeg muxer integration ============
        //2. Create output container file and mux control of FFMPEG
        std::string dstName = options.values.SinkName;

        if (dstName.compare(dstName.length() - 4, 1, ".") == 0 && dstName.compare(dstName.length() - 3, 3, "mkv") == 0) {
            muxCtrl.defaultFormat = true;
            muxCtrl.CodecId = MFX_CODEC_AVC;
        }
        else if (dstName.compare(dstName.length() - 4, 1, ".") == 0 && dstName.compare(dstName.length() - 3, 3, "mp4") == 0) {
            muxCtrl.defaultFormat = false;
            muxCtrl.CodecId = MFX_CODEC_AVC;
        }
        else if ((dstName.compare(dstName.length() - 5, 1, ".") == 0 && dstName.compare(dstName.length() - 4, 4, "mpeg") == 0) ||
            (dstName.compare(dstName.length() - 4, 1, ".") == 0 && dstName.compare(dstName.length() - 3, 3, "mpg") == 0) ||
            (dstName.compare(dstName.length() - 3, 1, ".") == 0 && dstName.compare(dstName.length() - 2, 2, "ts") == 0)) {
            muxCtrl.defaultFormat = false;
            muxCtrl.CodecId = MFX_CODEC_MPEG2;
        }
        else {
            printf("error: The file extension is not supported, please use '.mkv', 'mp4', '.mpeg', 'mpg' or 'ts'\n");
            exit(0);
        }
        muxCtrl.frameRateN = options.values.FrameRateN;
        muxCtrl.frameRateD = options.values.FrameRateD;
        muxCtrl.nBitRate = options.values.Bitrate;
        muxCtrl.nDstWidth = options.values.Width;
        muxCtrl.nDstHeight = options.values.Height;
        sts = openMuxControl(&muxCtrl, options.values.SinkName);
        MSDK_CHECK_RESULT(sts, MFX_ERR_NONE, sts);
        // =========== ffmpeg muxer integration end ============
    }

    //3. Initialize Intel Media SDK session
    // - MFX_IMPL_AUTO_ANY selects HW acceleration if available (on any adapter)
    // - Version 1.0 is selected for greatest backwards compatibility.
    // OS specific notes
    // - On Windows both SW and HW libraries may present
    // - On Linux only HW library only is available
    //   If more recent API features are needed, change the version accordingly
    mfxIMPL impl = options.values.impl;
    mfxVersion ver = { { 0, 1 } };
    MFXVideoSession session;

    sts = Initialize(impl, ver, &session, NULL);
    MSDK_CHECK_RESULT(sts, MFX_ERR_NONE, sts);

    // Create Media SDK encoder
    MFXVideoENCODE mfxENC(session);

    //4. Set required video parameters for encode
    // - In this example we are encoding an AVC (H.264) stream
    mfxVideoParam mfxEncParams;
    memset(&mfxEncParams, 0, sizeof(mfxEncParams));

    if (bEnableOutput)
        mfxEncParams.mfx.CodecId = muxCtrl.CodecId;
    else
        mfxEncParams.mfx.CodecId = MFX_CODEC_AVC;
    mfxEncParams.mfx.TargetUsage = MFX_TARGETUSAGE_BALANCED;
    mfxEncParams.mfx.TargetKbps = options.values.Bitrate;
    mfxEncParams.mfx.RateControlMethod = MFX_RATECONTROL_VBR;
    mfxEncParams.mfx.FrameInfo.FrameRateExtN = options.values.FrameRateN;
    mfxEncParams.mfx.FrameInfo.FrameRateExtD = options.values.FrameRateD;
    mfxEncParams.mfx.FrameInfo.FourCC = MFX_FOURCC_NV12;
    mfxEncParams.mfx.FrameInfo.ChromaFormat = MFX_CHROMAFORMAT_YUV420;
    mfxEncParams.mfx.FrameInfo.PicStruct = MFX_PICSTRUCT_PROGRESSIVE;
    mfxEncParams.mfx.FrameInfo.CropX = 0;
    mfxEncParams.mfx.FrameInfo.CropY = 0;
    mfxEncParams.mfx.FrameInfo.CropW = options.values.Width;
    mfxEncParams.mfx.FrameInfo.CropH = options.values.Height;
    // Width must be a multiple of 16
    // Height must be a multiple of 16 in case of frame picture and a multiple of 32 in case of field picture
    mfxEncParams.mfx.FrameInfo.Width = MSDK_ALIGN16(options.values.Width);
    mfxEncParams.mfx.FrameInfo.Height =
        (MFX_PICSTRUCT_PROGRESSIVE == mfxEncParams.mfx.FrameInfo.PicStruct) ?
        MSDK_ALIGN16(options.values.Height) :
        MSDK_ALIGN32(options.values.Height);

    mfxEncParams.IOPattern = MFX_IOPATTERN_IN_SYSTEM_MEMORY;

    // Validate video encode parameters (optional)
    // - In this example the validation result is written to same structure
    // - MFX_WRN_INCOMPATIBLE_VIDEO_PARAM is returned if some of the video parameters are not supported,
    //   instead the encoder will select suitable parameters closest matching the requested configuration
    sts = mfxENC.Query(&mfxEncParams, &mfxEncParams);
    MSDK_IGNORE_MFX_STS(sts, MFX_WRN_INCOMPATIBLE_VIDEO_PARAM);
    MSDK_CHECK_RESULT(sts, MFX_ERR_NONE, sts);

    // Query number of required surfaces for encoder
    mfxFrameAllocRequest EncRequest;
    memset(&EncRequest, 0, sizeof(EncRequest));
    sts = mfxENC.QueryIOSurf(&mfxEncParams, &EncRequest);
    MSDK_CHECK_RESULT(sts, MFX_ERR_NONE, sts);

    mfxU16 nEncSurfNum = EncRequest.NumFrameSuggested;

    //5. Allocate surfaces for encoder
    // - Width and height of buffer must be aligned, a multiple of 32
    // - Frame surface array keeps pointers all surface planes and general frame info
    mfxU16 width = (mfxU16) MSDK_ALIGN32(EncRequest.Info.Width);
    mfxU16 height = (mfxU16) MSDK_ALIGN32(EncRequest.Info.Height);
    mfxU8 bitsPerPixel = 12;        // NV12 format is a 12 bits per pixel format
    mfxU32 surfaceSize = width * height * bitsPerPixel / 8;
    mfxU8* surfaceBuffers = (mfxU8*) new mfxU8[surfaceSize * nEncSurfNum];

    // Allocate surface headers (mfxFrameSurface1) for encoder
    mfxFrameSurface1** pEncSurfaces = new mfxFrameSurface1 *[nEncSurfNum];
    MSDK_CHECK_POINTER(pEncSurfaces, MFX_ERR_MEMORY_ALLOC);
    for (int i = 0; i < nEncSurfNum; i++) {
        pEncSurfaces[i] = new mfxFrameSurface1;
        memset(pEncSurfaces[i], 0, sizeof(mfxFrameSurface1));
        memcpy(&(pEncSurfaces[i]->Info), &(mfxEncParams.mfx.FrameInfo), sizeof(mfxFrameInfo));
        pEncSurfaces[i]->Data.Y = &surfaceBuffers[surfaceSize * i];
        pEncSurfaces[i]->Data.U = pEncSurfaces[i]->Data.Y + width * height;
        pEncSurfaces[i]->Data.V = pEncSurfaces[i]->Data.U + 1;
        pEncSurfaces[i]->Data.Pitch = width;
        if (!bEnableInput) {
            ClearYUVSurfaceSysMem(pEncSurfaces[i], width, height);
        }
    }

    //6. Initialize the Media SDK encoder
    sts = mfxENC.Init(&mfxEncParams);
    MSDK_IGNORE_MFX_STS(sts, MFX_WRN_PARTIAL_ACCELERATION);
    MSDK_CHECK_RESULT(sts, MFX_ERR_NONE, sts);

    // Retrieve video parameters selected by encoder.
    // - BufferSizeInKB parameter is required to set bit stream buffer size
    mfxVideoParam par;
    memset(&par, 0, sizeof(par));
    sts = mfxENC.GetVideoParam(&par);
    MSDK_CHECK_RESULT(sts, MFX_ERR_NONE, sts);

    //7. Prepare Media SDK bit stream buffer
    mfxBitstream mfxBS;
    memset(&mfxBS, 0, sizeof(mfxBS));
    mfxBS.MaxLength = par.mfx.BufferSizeInKB * 1000;
    mfxBS.Data = new mfxU8[mfxBS.MaxLength];
    MSDK_CHECK_POINTER(mfxBS.Data, MFX_ERR_MEMORY_ALLOC);

    // ===================================
    //8. Start encoding the frames
    //

    mfxTime tStart, tEnd;
    mfxGetTime(&tStart);

    int nEncSurfIdx = 0;
    mfxSyncPoint syncp;
    mfxU32 nFrame = 0;

    //
    // Stage 1: Main encoding loop
    //
    while (MFX_ERR_NONE <= sts || MFX_ERR_MORE_DATA == sts) {
        nEncSurfIdx = GetFreeSurfaceIndex(pEncSurfaces, nEncSurfNum);   // Find free frame surface
        MSDK_CHECK_ERROR(MFX_ERR_NOT_FOUND, nEncSurfIdx, MFX_ERR_MEMORY_ALLOC);

        sts = LoadRawFrame(pEncSurfaces[nEncSurfIdx], fSource);
        MSDK_BREAK_ON_ERROR(sts);

        for (;;) {
            // Encode a frame asychronously (returns immediately)
            sts = mfxENC.EncodeFrameAsync(NULL, pEncSurfaces[nEncSurfIdx], &mfxBS, &syncp);

            if (MFX_ERR_NONE < sts && !syncp) {     // Repeat the call if warning and no output
                if (MFX_WRN_DEVICE_BUSY == sts)
                    MSDK_SLEEP(1);  // Wait if device is busy, then repeat the same call
            } else if (MFX_ERR_NONE < sts && syncp) {
                sts = MFX_ERR_NONE;     // Ignore warnings if output is available
                break;
            } else if (MFX_ERR_NOT_ENOUGH_BUFFER == sts) {
                // Allocate more bitstream buffer memory here if needed...
                break;
            } else
                break;
        }

        if (MFX_ERR_NONE == sts) {
            sts = session.SyncOperation(syncp, 60000);      // Synchronize. Wait until encoded frame is ready
            MSDK_CHECK_RESULT(sts, MFX_ERR_NONE, sts);

            ++nFrame;
            if (bEnableOutput) {
                // =========== ffmpeg muxer integration ============
                muxCtrl.nProcessedFramesNum = nFrame;
                sts = ffmpegWriteFrame(&mfxBS, &muxCtrl);
                MSDK_BREAK_ON_ERROR(sts);

                printf("Frame number: %d\r", nFrame);
                fflush(stdout);
            }
            mfxBS.DataLength = 0;
            // =========== ffmpeg muxer integration end ============
        }
    }

    // MFX_ERR_MORE_DATA means that the input file has ended, need to go to buffering loop, exit in case of other errors
    MSDK_IGNORE_MFX_STS(sts, MFX_ERR_MORE_DATA);
    MSDK_CHECK_RESULT(sts, MFX_ERR_NONE, sts);

    //
    // Stage 2: Retrieve the buffered encoded frames
    //
    while (MFX_ERR_NONE <= sts) {
        for (;;) {
            // Encode a frame asychronously (returns immediately)
            sts = mfxENC.EncodeFrameAsync(NULL, NULL, &mfxBS, &syncp);

            if (MFX_ERR_NONE < sts && !syncp) {     // Repeat the call if warning and no output
                if (MFX_WRN_DEVICE_BUSY == sts)
                    MSDK_SLEEP(1);  // Wait if device is busy, then repeat the same call
            } else if (MFX_ERR_NONE < sts && syncp) {
                sts = MFX_ERR_NONE;     // Ignore warnings if output is available
                break;
            } else
                break;
        }

        if (MFX_ERR_NONE == sts) {
            sts = session.SyncOperation(syncp, 60000);      // Synchronize. Wait until encoded frame is ready
            MSDK_CHECK_RESULT(sts, MFX_ERR_NONE, sts);
            
            ++nFrame;
            if (bEnableOutput) {
            // =========== ffmpeg muxer integration ============
                muxCtrl.nProcessedFramesNum = nFrame;
                sts = ffmpegWriteFrame(&mfxBS, &muxCtrl);
                MSDK_BREAK_ON_ERROR(sts);

                printf("Frame number: %d\r", nFrame);
                fflush(stdout);
            }
            mfxBS.DataLength = 0;
            // =========== ffmpeg muxer integration end ============
        }
    }

    // MFX_ERR_MORE_DATA indicates that there are no more buffered frames, exit in case of other errors
    MSDK_IGNORE_MFX_STS(sts, MFX_ERR_MORE_DATA);
    MSDK_CHECK_RESULT(sts, MFX_ERR_NONE, sts);

    mfxGetTime(&tEnd);
    double elapsed = TimeDiffMsec(tEnd, tStart) / 1000;
    double fps = ((double)nFrame / elapsed);
    printf("\nExecution time: %3.2f s (%3.2f fps)\n", elapsed, fps);

    // ===================================================================
    // 9. Clean up resources
    //  - It is recommended to close Media SDK components first, before releasing allocated surfaces, since
    //    some surfaces may still be locked by internal Media SDK resources.

    mfxENC.Close();
    // session closed automatically on destruction

    for (int i = 0; i < nEncSurfNum; i++)
        delete pEncSurfaces[i];
    MSDK_SAFE_DELETE_ARRAY(pEncSurfaces);
    MSDK_SAFE_DELETE_ARRAY(mfxBS.Data);

    MSDK_SAFE_DELETE_ARRAY(surfaceBuffers);

    if (fSource) fclose(fSource);

    // =========== ffmpeg muxer integration ============
    if (bEnableOutput) closeMuxControl(&muxCtrl);
    // =========== ffmpeg muxer integration end ============

    Release();

    return 0;
}
