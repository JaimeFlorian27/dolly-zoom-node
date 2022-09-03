#include <maya/MPxCommand.h>
#pragma once

class DollyZoomCmd : public MPxCommand
{
public:
	DollyZoomCmd();
	virtual ~DollyZoomCmd();
	static void* creator();
	static MStatus doIt(const MArgList &args); 
};