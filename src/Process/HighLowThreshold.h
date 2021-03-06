#pragma once

#include "TinkerVision_export.h"

#include "../Core/ImageType.h"
#include "../Core/Image.h"
#include "../Core/ImageGPU.h"

#include "../Core/VectorMath.h"

namespace TnkrVis
{
namespace Process
{
	
class TINKERVISION_EXPORT HighLowThreshold
{
    private: 
        class Internal;
        Internal* internal;

	public:
		HighLowThreshold(); 
        ~HighLowThreshold(); 
        void SetLowThreshold(float t);
        void SetHighThreshold(float t);
        void SetLowThreshold(Color t);
        void SetHighThreshold(Color t);
		void Run(ImageGPU* input, ImageGPU* output); 
        void Run(Image* input, Image* output); 
};
	
}
}