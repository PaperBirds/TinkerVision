#pragma once

#include "Visi_export.h"

#include "ImageType.h"
#include "Image.h"
#include "ImageGPU.h"

#include <string>

namespace Visi
{

class VISI_EXPORT VideoFile
{
    private: 
        class Internal;
        Internal* internal;

	public:
		VideoFile(); 
        ~VideoFile(); 
        bool Open(std::string fileSrc);
        bool Close();
        bool LoadNextFrame(); 
        void SwapBuffers(); 
        bool GetFrame(Image* frameImage, int streamInx=0);

        bool AtEnd(); 
        bool IsOpen(); 
};
	
}