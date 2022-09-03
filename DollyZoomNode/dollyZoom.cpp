#include "dollyZoom.h"

const double DollyZoom::PI{ 3.141592653589793238463 }; //defined for conversion between radians and degrees

double DollyZoom::milimetetersToinches(const double& focalLength)
{
    return focalLength / 25.4;
}

double DollyZoom::focalLengthToAngleOfView(const double &focalLength, const double &cameraAperture)
{
    double angleOfView{ 2.0f * atan(cameraAperture / (2.0f * focalLength)) };
    
    angleOfView = angleOfView * 180.0f / PI; //radians to degrees conversion

    return angleOfView;
}
double DollyZoom::angleOfViewToFocalLength(const double &angleOfView, const double &cameraAperture)
{
    double angleRad{angleOfView *PI / 180.0}; //conversion from degrees to radians

    double focalLength{ cameraAperture / (2.0 * tan(0.5 * angleRad))};

    return focalLength;
}
double DollyZoom::calculateAOV(const double &width,const double &distance)
{
    double angleOfView = 2.0f * atan(0.5f * width / distance);
    
    angleOfView = angleOfView * 180.0f / PI; //radians to degrees conversion

    return angleOfView;
}
