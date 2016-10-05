#include "videothread.h"
#include <wchar.h>
#include <locale.h>
#include <stdlib.h>
#include <stdio.h>
#include <ueye.h>
#include <iostream>
#include <QDebug>
#include "mainwindow.h"

using namespace std;

HIDS m_hCam;                                    // handle to cam
HWND m_hWndDisplay;                             // handle to display window
int m_nColorMode = IS_CM_BGR565_PACKED;         // Y8/RGB16/RGB24/REG32
int m_nBitsPerPixel=16;                         // number of bits needed store one pixel
int m_nSizeX = 640;                             // width of video (max 2560)
int m_nSizeY = 480;                             // height of video (max 1920)
int m_lMemoryId[2];                             // grabber memory - buffer   ID
char* m_pcImageMemory[2];                       // grabber memory - pointer to buffer
uint AwbType = IS_AWB_GREYWORLD;                // Awb type
INT nAOISupported;
SENSORINFO sInfo;
double enable = 1;
double defaultExposure = 6.0;



VideoThread::VideoThread()
{
    INT nRet = is_InitCamera(&m_hCam, m_hWndDisplay);
    if(nRet != IS_SUCCESS)
    {
        qDebug() << "Error: Camera not found!" << endl;
        abort();
    }
    int sat=130;
    is_Saturation(m_hCam, SATURATION_CMD_SET_VALUE, (void*)&sat, sizeof(sat));

    double ret;
    is_Saturation(m_hCam, SATURATION_CMD_GET_VALUE, (void*)&ret, sizeof(ret));

    is_SetColorMode(m_hCam, m_nColorMode);

    // auto exposure
    is_Exposure(m_hCam, IS_EXPOSURE_CMD_SET_EXPOSURE, &defaultExposure, sizeof(defaultExposure));
    is_SetAutoParameter(m_hCam, IS_SET_ENABLE_AUTO_SHUTTER, &enable, 0);
    // auto white balance
    is_AutoParameter(m_hCam, IS_AWB_CMD_SET_TYPE, (void*)&AwbType, sizeof(AwbType));
    is_SetAutoParameter(m_hCam, IS_SET_ENABLE_AUTO_WHITEBALANCE, &enable, 0);

    // Sensor scaling for better fps and control
    is_SetSubSampling(m_hCam, IS_SUBSAMPLING_4X_VERTICAL | IS_SUBSAMPLING_4X_HORIZONTAL);

    // Allocating memory for 3 frames and adding them to sequence
    for (int bufferIndex = 0; bufferIndex< 2; bufferIndex++){
        nRet = is_AllocImageMem(m_hCam,m_nSizeX,m_nSizeY,m_nBitsPerPixel,&m_pcImageMemory[bufferIndex],&m_lMemoryId[bufferIndex]);
        is_AddToSequence(m_hCam, m_pcImageMemory[bufferIndex], m_lMemoryId[bufferIndex]);
    }

    nRet = is_SetDisplayMode(m_hCam, IS_SET_DM_DIB);
    if(nRet != IS_SUCCESS)
    {
        qDebug() << "Error: is_SetDisplayMode" << endl;
    }
}

VideoThread::~VideoThread()
{
    is_StopLiveVideo(m_hCam, IS_DONT_WAIT);
    if(m_hCam != 0)
    {
        for (int bufferIndex = 0; bufferIndex<2; bufferIndex++){
            is_FreeImageMem(m_hCam,m_pcImageMemory[bufferIndex],m_lMemoryId[bufferIndex]);
            }
        is_ExitCamera(m_hCam);
    }
}

void VideoThread::stopVideo()
{
    is_StopLiveVideo(m_hCam, IS_WAIT);
}

void VideoThread::run()
{
    void* imageData;
    is_CaptureVideo(m_hCam,IS_DONT_WAIT);
    is_EnableEvent(m_hCam, IS_SET_EVENT_FRAME);
    while(is_WaitEvent(m_hCam,IS_SET_EVENT_FRAME,1000)==IS_SUCCESS)
    {
        is_GetImageMem(m_hCam, &imageData);
        emit frame(imageData);
    }
}
