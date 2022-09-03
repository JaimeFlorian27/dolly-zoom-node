#include "dollyZoomCmd.h"
DollyZoomCmd::DollyZoomCmd()
{
}
DollyZoomCmd::~DollyZoomCmd()
{
}
void* DollyZoomCmd::creator()
{
	return new DollyZoomCmd();
}

MSyntax DollyZoomCmd::newSyntax()
{
	MSyntax syntax;
	syntax.addArg(MSyntax::kString);
	syntax.addArg(MSyntax::kString);
	syntax.setObjectType(MSyntax::kStringObjects, 2, 2);
	return syntax;
}
MStatus DollyZoomCmd::doIt(const MArgList& args)
{
	return MStatus();
}
