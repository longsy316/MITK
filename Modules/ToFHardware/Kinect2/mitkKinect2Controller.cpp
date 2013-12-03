/*===================================================================

The Medical Imaging Interaction Toolkit (MITK)

Copyright (c) German Cancer Research Center,
Division of Medical and Biological Informatics.
All rights reserved.

This software is distributed WITHOUT ANY WARRANTY; without
even the implied warranty of MERCHANTABILITY or FITNESS FOR
A PARTICULAR PURPOSE.

See LICENSE.txt or http://www.mitk.org for details.

===================================================================*/
#include "mitkKinect2Controller.h"
#include <Kinect.h>
#include "stdafx.h"

namespace mitk
{
class Kinect2Controller::Kinect2ControllerPrivate
{
public:
  Kinect2ControllerPrivate();
  ~Kinect2ControllerPrivate();

  bool ErrorText(unsigned int error);
  //OpenNI related stuff
  //  xn::Context m_Context; ///< OpenNI context
  //  xn::DepthGenerator m_DepthGenerator; ///< Depth generator to access depth image of kinect
  //  xn::ImageGenerator m_ImageGenerator; ///< Image generator to access RGB image of kinect
  //  xn::IRGenerator m_IRGenerator; ///< IR generator to access IR image of kinect
  // Depth reader
  IDepthFrameReader* m_pDepthFrameReader;
  IKinectSensor* m_pKinectSensor;

  bool m_ConnectionCheck; ///< check if camera is connected or not

  bool m_UseIR; ///< flag indicating whether IR image is used or not

  unsigned int m_CaptureWidth; ///< image width
  unsigned int m_CaptureHeight; ///< image height
};

Kinect2Controller::Kinect2ControllerPrivate::Kinect2ControllerPrivate():
  m_pKinectSensor(NULL),
  m_ConnectionCheck(false),
  m_UseIR(false),
  m_CaptureWidth(640),
  m_CaptureHeight(480)
{
}

Kinect2Controller::Kinect2ControllerPrivate::~Kinect2ControllerPrivate()
{
}

bool Kinect2Controller::Kinect2ControllerPrivate::ErrorText(unsigned int error)
{
  //  if(error != XN_STATUS_OK)
  //  {
  //    MITK_ERROR << "Kinect Camera Error " << xnGetStatusString(error);
  //    mitkThrow() << "Kinect Camera Error " << xnGetStatusString(error);
  //    return false;
  //  }
  //  else return true;
  return true;
}

Kinect2Controller::Kinect2Controller(): d(new Kinect2ControllerPrivate)
{
}

Kinect2Controller::~Kinect2Controller()
{
  delete d;
}

bool Kinect2Controller::OpenCameraConnection()
{
  HRESULT hr;
  if (!d->m_ConnectionCheck)
  {
    hr = GetDefaultKinectSensor(&d->m_pKinectSensor);
    MITK_INFO << "innen drin:" << hr;

    if (FAILED(hr))
    {
      d->m_ConnectionCheck = false;
    }

    if (d->m_pKinectSensor)
    {
      // Initialize the Kinect and get the depth reader
      IDepthFrameSource* pDepthFrameSource = NULL;

      hr = d->m_pKinectSensor->Open();

      if (SUCCEEDED(hr))
      {
        hr = d->m_pKinectSensor->get_DepthFrameSource(&pDepthFrameSource);
      }

      if (SUCCEEDED(hr))
      {
        hr = pDepthFrameSource->OpenReader(&d->m_pDepthFrameReader);
      }

      SafeRelease(pDepthFrameSource);
    }

    if (!d->m_pKinectSensor || FAILED(hr))
    {
      d->m_ConnectionCheck = false;
      MITK_INFO << "No Kinect 2 ready!";
    }

    //      // Initialize the OpenNI status
    //      d->m_ConnectionCheck = d->ErrorText(d->m_Context.Init());
    //      // Create a depth generator and set its resolution
    //      XnMapOutputMode DepthMode;
    //      d->m_ConnectionCheck = d->ErrorText(d->m_DepthGenerator.Create(d->m_Context));
    //      if (!d->m_ConnectionCheck) return false;
    //      d->m_DepthGenerator.GetMapOutputMode(DepthMode);
    //      DepthMode.nXRes = xn::Resolution((XnResolution)XN_RES_VGA).GetXResolution();
    //      DepthMode.nYRes = xn::Resolution((XnResolution)XN_RES_VGA).GetYResolution();
    //      d->m_ConnectionCheck = d->ErrorText(d->m_DepthGenerator.SetMapOutputMode(DepthMode));
    //      if (!d->m_ConnectionCheck) return false;

    //      if (d->m_UseIR)
    //      {
    //        // Create the IR generator and set its resolution
    //        d->m_ConnectionCheck = d->ErrorText(d->m_IRGenerator.Create(d->m_Context));
    //        if (!d->m_ConnectionCheck) return false;
    //        XnMapOutputMode IRMode;
    //        d->m_IRGenerator.GetMapOutputMode(IRMode);
    //        IRMode.nXRes = XN_VGA_X_RES;
    //        IRMode.nYRes = XN_VGA_Y_RES;
    //        IRMode.nFPS = 30;
    //        d->m_ConnectionCheck = d->ErrorText(d->m_IRGenerator.SetMapOutputMode(IRMode));
    //        if (!d->m_ConnectionCheck) return false;
    //      }
    //      else
    //      {
    //        // Create an image generator and set its resolution
    //        XnMapOutputMode ImageMode;
    //        d->m_ConnectionCheck = d->ErrorText(d->m_ImageGenerator.Create(d->m_Context));
    //        if (!d->m_ConnectionCheck) return false;
    //        d->m_ImageGenerator.GetMapOutputMode(ImageMode);
    //        ImageMode.nXRes = xn::Resolution((XnResolution)XN_RES_VGA).GetXResolution();
    //        ImageMode.nYRes = xn::Resolution((XnResolution)XN_RES_VGA).GetYResolution();
    //        d->m_ConnectionCheck = d->ErrorText(d->m_ImageGenerator.SetMapOutputMode(ImageMode));
    //        if (!d->m_ConnectionCheck) return false;
    //      }

    //      // Camera registration
    //      if ( d->m_DepthGenerator.IsCapabilitySupported(XN_CAPABILITY_ALTERNATIVE_VIEW_POINT) )
    //      {
    //        if (d->m_UseIR)
    //        {
    //          d->m_ConnectionCheck = d->ErrorText(d->m_DepthGenerator.GetAlternativeViewPointCap().SetViewPoint(d->m_IRGenerator));
    //          //if (!d->m_ConnectionCheck) return false;
    //        }
    //        else
    //        {
    //          d->m_ConnectionCheck = d->ErrorText(d->m_DepthGenerator.GetAlternativeViewPointCap().SetViewPoint(d->m_ImageGenerator));
    //          //if (!d->m_ConnectionCheck) return false;
    //        }
    //      }
    //      else
    //      {
    //        std::cout << "Alternative view point not supported by the depth generator..." << std::endl;
    //      }
    //      if (d->m_UseIR)
    //      {
    //        if ( d->m_IRGenerator.IsCapabilitySupported(XN_CAPABILITY_ALTERNATIVE_VIEW_POINT) )
    //        {
    //          d->m_ConnectionCheck = d->ErrorText(d->m_IRGenerator.GetAlternativeViewPointCap().SetViewPoint(d->m_DepthGenerator));
    //          //if (!d->m_ConnectionCheck) return false;
    //        }
    //        else
    //        {
    //          std::cout << "Alternative view point not supported by the depth generator..." << std::endl;
    //        }
    //      }

    //      // Start data generation
    //      d->m_ConnectionCheck = d->ErrorText(d->m_Context.StartGeneratingAll());
    //      if (!d->m_ConnectionCheck) return false;

    ////      // Update the connected flag
    d->m_ConnectionCheck = true;
  }
  return d->m_ConnectionCheck;
}

bool Kinect2Controller::CloseCameraConnection()
{
  //    d->m_ConnectionCheck = !d->ErrorText(d->m_Context.StopGeneratingAll());
  //    return !d->m_ConnectionCheck;
  return true;
}

bool Kinect2Controller::UpdateCamera()
{
  //    bool updateSuccessful = d->ErrorText(d->m_Context.WaitAndUpdateAll());
  //    xn::DepthMetaData DepthMD;
  //    d->m_DepthGenerator.GetMetaData(DepthMD);
  //    d->m_CaptureWidth = DepthMD.XRes();
  //    d->m_CaptureHeight = DepthMD.YRes();
  //    return updateSuccessful;
      if (!d->m_pDepthFrameReader)
    {
        return false;
    }

    IDepthFrame* pDepthFrame = NULL;

    HRESULT hr = d->m_pDepthFrameReader->AcquireLatestFrame(&pDepthFrame);

    if (SUCCEEDED(hr))
    {
        INT64 nTime = 0;
        IFrameDescription* pFrameDescription = NULL;
        int nWidth = 0;
        int nHeight = 0;
        USHORT nDepthMinReliableDistance = 0;
        USHORT nDepthMaxReliableDistance = 0;
        UINT nBufferSize = 0;
        UINT16 *pBuffer = NULL;

        hr = pDepthFrame->get_RelativeTime(&nTime);

        if (SUCCEEDED(hr))
        {
            hr = pDepthFrame->get_FrameDescription(&pFrameDescription);
        }

        if (SUCCEEDED(hr))
        {
            hr = pFrameDescription->get_Width(&nWidth);
        }

        if (SUCCEEDED(hr))
        {
            hr = pFrameDescription->get_Height(&nHeight);
        }

        if (SUCCEEDED(hr))
        {
            hr = pDepthFrame->get_DepthMinReliableDistance(&nDepthMinReliableDistance);
        }

        if (SUCCEEDED(hr))
        {
            hr = pDepthFrame->get_DepthMaxReliableDistance(&nDepthMaxReliableDistance);
        }

        if (SUCCEEDED(hr))
        {
            hr = pDepthFrame->AccessUnderlyingBuffer(&nBufferSize, &pBuffer);
        }

        if (SUCCEEDED(hr))
        {
            //ProcessDepth(nTime, pBuffer, nWidth, nHeight, nDepthMinReliableDistance, nDepthMaxReliableDistance);
        }

        SafeRelease(pFrameDescription);
    }

    SafeRelease(pDepthFrame);


  return true;
}

void Kinect2Controller::GetDistances(float* distances)
{
  //    xn::DepthMetaData DepthMD;
  //    d->m_DepthGenerator.GetMetaData(DepthMD);
  //    const XnDepthPixel* DepthData = DepthMD.Data();

  //    for (unsigned int i=0; i<d->m_CaptureWidth*d->m_CaptureHeight; i++)
  //    {
  //      distances[i] = DepthData[i];
  //    }
}

void Kinect2Controller::GetRgb(unsigned char* rgb)
{
  //    if (!d->m_UseIR)
  //    {
  //      xn::ImageMetaData ImageMD;
  //      d->m_ImageGenerator.GetMetaData(ImageMD);
  //      const XnRGB24Pixel* rgbPixelArray = ImageMD.RGB24Data();
  //      for (int i=0; i<d->m_CaptureWidth*d->m_CaptureHeight; i++)
  //      {
  //        rgb[i*3] = rgbPixelArray[i].nRed;
  //        rgb[i*3+1] = rgbPixelArray[i].nGreen;
  //        rgb[i*3+2] = rgbPixelArray[i].nBlue;
  //      }
  //    }
}

void Kinect2Controller::GetAllData(float* distances, float* amplitudes, unsigned char* rgb)
{
  //    // get current distance data
  //    xn::DepthMetaData DepthMD;
  //    d->m_DepthGenerator.GetMetaData(DepthMD);
  //    const XnDepthPixel* DepthData = DepthMD.Data();
  //    // IR data
  //    xn::IRMetaData IRData;
  //    const XnIRPixel* IRPixelData;
  //    // Image data
  //    xn::ImageMetaData ImageMD;
  //    const XnRGB24Pixel* rgbPixelArray;
  //    if (d->m_UseIR)
  //    {
  //      d->m_IRGenerator.GetMetaData(IRData);
  //      IRPixelData = IRData.Data();
  //    }
  //    else
  //    {
  //      // get current rgb data
  //      d->m_ImageGenerator.GetMetaData(ImageMD);
  //      rgbPixelArray = ImageMD.RGB24Data();
  //    }

  //    for (unsigned int i=0; i<d->m_CaptureWidth*d->m_CaptureHeight; i++)
  //    {
  //      distances[i] = DepthData[i];
  //      if (d->m_UseIR)
  //      {
  //        amplitudes[i] = IRPixelData[i];
  //      }
  //      else
  //      {
  //        rgb[i*3] = rgbPixelArray[i].nRed;
  //        rgb[i*3+1] = rgbPixelArray[i].nGreen;
  //        rgb[i*3+2] = rgbPixelArray[i].nBlue;
  //      }
  //    }
}

void Kinect2Controller::GetAmplitudes( float* amplitudes )
{
  //    if (d->m_UseIR)
  //    {
  //      xn::IRMetaData IRData;
  //      d->m_IRGenerator.GetMetaData(IRData);
  //      const XnIRPixel* IRPixelData = IRData.Data();

  //      for (unsigned int i=0; i<d->m_CaptureWidth*d->m_CaptureHeight; i++)
  //      {
  //        amplitudes[i] = IRPixelData[i];
  //      }
  //    }
}

void Kinect2Controller::GetIntensities( float* intensities )
{
}
unsigned int Kinect2Controller::GetCaptureWidth() const
{
  return d->m_CaptureWidth;
}

unsigned int Kinect2Controller::GetCaptureHeight() const
{
  return d->m_CaptureHeight;
}

bool Kinect2Controller::GetUseIR() const
{
  return d->m_UseIR;
}
void Kinect2Controller::SetUseIR(bool useIR)
{
  if (d->m_UseIR!=useIR)
  {
    d->m_UseIR = useIR;
    this->Modified();
  }
}
}
