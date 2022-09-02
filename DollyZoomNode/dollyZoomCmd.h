#include <maya/MPxCommand.h>
#pragma once

class DollyZoomCmd : public MPxCommand
{
public:
	DollyZoomCmd();
	static MStatus doIt(const MArgList& args);
};