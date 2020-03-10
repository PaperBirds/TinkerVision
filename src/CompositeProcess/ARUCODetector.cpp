#include "ARUCODetector.h"

#include "../Core/ComputeShader.h"
#include "../Core/ProcessHelper.h"

#include "../Process/GaussianBlur.h"
#include "../Process/GrayScale.h"
#include "../Process/AdaptiveThreshold.h"
#include "../Process/FindContours.h"
#include "../Process/MarkerBitExtract.h"
#include "../Process/MarkerDictionary.h"
#include "../Process/Renderer.h"

#include "../IO/ImageFile.h"

#include <string>
#include <iostream>
#include <map>

namespace Visi
{
namespace CompositeProcess
{

using namespace Process;

class ARUCODetector::Internal
{
    private:
        ImageGPU tempGPU[4]; 
        Image temp[3];

        std::vector<Contour> contours;
        std::vector<Visi::Contour> contoursFiltered; 
        std::vector<Visi::Contour> contoursSimplified; 
        std::vector<Visi::Contour> contoursMerged; 
        std::vector<Visi::Contour> contoursQuads; 
        std::vector<Visi::Contour> arucoQuads; 
        std::vector<Visi::Image> bitImages;  

        GrayScale gray; 
        AdaptiveThreshold adaptiveThreshold; 
        FindContours findContours; 
        MarkerBitExtract markerBitExtract; 
        MarkerDictionary markerDictionary; 

        Renderer render;

    public:
        Internal(); 
        void Run(ImageGPU* input);
        void AddDictionaryEntry(Image* entry, int id);
};


ARUCODetector::Internal::Internal()
{
}

void ARUCODetector::Internal::Run(ImageGPU* input)
{
    bool grayConverted = false; 
    if(input->GetType() != ImageType::GRAYSCALE8 && input->GetType() != ImageType::GRAYSCALE16 && input->GetType() != ImageType::GRAYSCALE32F)
    {
        gray.Run(input, &tempGPU[0]); 
        input = &tempGPU[0]; 
    }

    adaptiveThreshold.SetThreshold(0.02); 
	adaptiveThreshold.SetSize(7); 
    adaptiveThreshold.Run(input, &tempGPU[1]);

    temp[0].Copy(&tempGPU[1]); 

    findContours.Run(&temp[0], &temp[1], &contours); 

    Visi::Contour::ContoursVertCountFilter(&contours, &contoursFiltered, 100);
    Visi::Contour::ContoursSimplify(&contoursFiltered, &contoursSimplified, 3);
    Visi::Contour::ContoursMergeVerticies(&contoursSimplified, &contoursMerged, 4);
    Visi::Contour::ContoursSimplify(&contoursMerged, &contoursSimplified, 5);
    Visi::Contour::ContoursVertCountFilter(&contoursSimplified, &contoursQuads, 4, 4);

    temp[1].Copy(input); 

    markerBitExtract.Run(&temp[1], &contoursQuads, &bitImages); 


    for(int i = 0; i < bitImages.size(); i++)
    {
        int id = markerDictionary.Lookup(&bitImages[i]); 
        if(id != -1)
        {
            
        } 
    }
    /*
    for(int i = 0; i < bitImages.size(); i++)
    {
        std::string fn = "bitImage";
        fn += std::to_string(i); 
        fn += ".png"; 
        Visi::IO::ImageFile::Write(fn, &bitImages[i]); 
    }
    
    
    render.AddContours(&contoursQuads);
    render.Run(&temp[0], &temp[2]); 
    Visi::IO::ImageFile::Write("testTemp.png", &temp[2]); 
    */
}

void ARUCODetector::Internal::AddDictionaryEntry(Image* entry, int id)
{
    markerDictionary.AddEntry(entry, id);
}



ARUCODetector::ARUCODetector()
{
    internal = new Internal(); 
}

ARUCODetector::~ARUCODetector()
{ 
    delete internal; 
}

void ARUCODetector::Run(ImageGPU* input)
{
    internal->Run(input); 
}

void ARUCODetector::AddDictionaryEntry(Image* entry, int id)
{
    internal->AddDictionaryEntry(entry, id);
}


}
}