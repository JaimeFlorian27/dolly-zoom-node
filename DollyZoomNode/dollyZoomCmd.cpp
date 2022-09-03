#include "dollyZoomCmd.h"

MSelectionList DollyZoomCmd::selectionList;

DollyZoomCmd::DollyZoomCmd()
{
}
DollyZoomCmd::~DollyZoomCmd()
{
}
void* DollyZoomCmd::creator()
{
	return new DollyZoomCmd;
}

/*MSyntax DollyZoomCmd::newSyntax()
{	//Define syntax for command (2 arguments needed, camera name and target name)
	MSyntax syntax;
	syntax.addArg(MSyntax::kString);
	syntax.addArg(MSyntax::kString);
	//return the objects as a selection list
	syntax.setObjectType(MSyntax::kSelectionList, 2, 2);
	return syntax;
}*/
MStatus DollyZoomCmd::doIt(const MArgList& args)
{
	MStatus status(MS::kSuccess);
	//use the database to store the objects taken in using our syntax into my internal class.
	//MArgDatabase argData(newSyntax(), args);
	//status = argData.getObjects(selectionList);
	//CHECK_MSTATUS_AND_RETURN_IT(status);
	//call to redoIt for execution of command
	
	return redoIt();
}

/*bool DollyZoomCmd::isUndoable()
{
	return true;
}*/

MStatus DollyZoomCmd::redoIt()
{
	MStatus status(MS::kSuccess);

	//check if there are less than two arguments
	/*if (selectionList.length() < 2) {
		status = MS::kFailure;
		CHECK_MSTATUS_AND_RETURN_IT(status);
	}*/

	//create node

	MFnDependencyNode dollyFn;
	MObject dollyObj;

	dollyObj = dollyFn.create("dollyZoom");

	return status;
}

/*MStatus DollyZoomCmd::undoIt()
{
	MStatus status;

	return status;
}*/
