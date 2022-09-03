#include "dollyZoomCmd.h"
DollyZoomCmd::DollyZoomCmd()
{
}
DollyZoomCmd::~DollyZoomCmd()
{
}
void* DollyZoomCmd::creator()
{
	return nullptr;
}
MStatus DollyZoomCmd::doIt(const MArgList& args)
{
	return MStatus();
}
