#include "stdafx.h"
#pragma once


static std::vector<float> RGBtoHSL(std::vector<float> classColor)
{
    std::vector<float> hsl;
    float rprime = classColor[0] / 255.0;
    float gprime = classColor[1] / 255.0;
    float bprime = classColor[2] / 255.0;
    float cMax = max(max(rprime, gprime), bprime);
    float cMin = min(min(rprime, gprime), bprime);
    float cDelta = cMax - cMin;
    float saturation, lightness = 0;
    int hue = 0;

    // get hue
    if (cDelta == 0)
    {
        hue = 0;
    }
    else if (cMax == rprime)
    {
        hue = 60 * fmod(((gprime - bprime) / cDelta), 6);
    }
    else if (cMax == gprime)
    {
        hue = 60 * ((bprime - rprime) / cDelta + 2);
    }
    else if (cMax == bprime)
    {
        hue = 60 * ((rprime - gprime) / cDelta + 4);
    }

    // get lightness
    lightness = (cMax + cMin) / 2;

    // get saturation
    if (cDelta == 0)
    {
        saturation = 0;
    }
    else
    {
        saturation = cDelta / (1 - abs(2 * lightness - 1));
    }

    if (saturation > 1.0f)
    {
        saturation = 1.0;
    }
    hsl.push_back(hue % 360);
    hsl.push_back(saturation);
    hsl.push_back(lightness);
    return hsl;
}


static std::vector<GLubyte> HSLtoRGB(std::vector<float> hsl)
{

    std::vector<float> rgbPrimeVector;
    std::vector<GLubyte> rgbVector;
    float hue = hsl[0];
    float saturation = hsl[1];
    float lightness = hsl[2];

    float c = (1 - abs(2 * lightness - 1)) * saturation;
    float x = c * (1 - abs(fmod((hue / 60.0), 2) - 1));
    float m = lightness - (c / 2.0);

    if (hue >= 0 && hue < 60)
    {
        rgbPrimeVector.push_back(c);
        rgbPrimeVector.push_back(x);
        rgbPrimeVector.push_back(0);
    }
    else if (hue >= 60 && hue < 120)
    {
        rgbPrimeVector.push_back(x);
        rgbPrimeVector.push_back(c);
        rgbPrimeVector.push_back(0);
    }
    else if (hue >= 120 && hue < 180)
    {
        rgbPrimeVector.push_back(0);
        rgbPrimeVector.push_back(c);
        rgbPrimeVector.push_back(x);
    }
    else if (hue >= 180 && hue < 240)
    {
        rgbPrimeVector.push_back(0);
        rgbPrimeVector.push_back(x);
        rgbPrimeVector.push_back(c);
    }
    else if (hue >= 240 && hue < 300)
    {
        rgbPrimeVector.push_back(x);
        rgbPrimeVector.push_back(0);
        rgbPrimeVector.push_back(c);
    }
    else if (hue >= 300 && hue < 360)
    {
        rgbPrimeVector.push_back(c);
        rgbPrimeVector.push_back(0);
        rgbPrimeVector.push_back(x);
    }

    rgbVector.push_back((rgbPrimeVector[0] + m) * 255);
    rgbVector.push_back((rgbPrimeVector[1] + m) * 255);
    rgbVector.push_back((rgbPrimeVector[2] + m) * 255);

    std::cout << "debug" << std::endl;

    return rgbVector;
}