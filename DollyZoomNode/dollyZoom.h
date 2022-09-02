#pragma once
#include <math.h>
class DollyZoom
{
public:
	static double focalLengthToAngleOfView(const double focalLength, const double cameraAperture);
	static double angleOfViewToFocalLength(const double angleOfView, const double cameraAperture);
	static double calculateAOV(double width, double distance);
	static const double PI;
};
