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

MSyntax DollyZoomCmd::newSyntax()
{	//Define syntax for command (2 arguments needed, camera name and target name)
	MSyntax syntax;
	syntax.setObjectType(MSyntax::kSelectionList, 2, 2); //return the objects as a selection list
	return syntax;
}
MStatus DollyZoomCmd::doIt(const MArgList& args)
{
	MStatus status(MS::kSuccess);
	//use the database to store the objects taken in using our syntax into my internal class.
	MArgDatabase argData(DollyZoomCmd::syntax(), args);
	status = argData.getObjects(selectionList);
	CHECK_MSTATUS_AND_RETURN_IT(status);
	//call to redoIt for execution of command
	
	return redoIt();
}

bool DollyZoomCmd::isUndoable()
{
	return true;
}

MStatus DollyZoomCmd::redoIt()
{
	MStatus status(MS::kSuccess);

	//check if there are less than two arguments
	if (selectionList.length() < 2) {
		status = MS::kFailure;
		CHECK_MSTATUS_AND_RETURN_IT(status);
	}
	// retreive dag path
	MDagPath cameraDagPath, targetDagPath;
	
	selectionList.getDagPath(0, cameraDagPath);
	selectionList.getDagPath(1, targetDagPath);


	//check if nodes are transform nodes  -- IF CAMERA IS NOT A KTRANSFORM OR CAMERA SHAPE AND
	if ((!cameraDagPath.node().hasFn(MFn::kTransform) && !cameraDagPath.node().hasFn(MFn::kCamera)) && !targetDagPath.node().hasFn(MFn::kTransform))
	{
		status = MS::kFailure;
		CHECK_MSTATUS_AND_RETURN_IT(status);
		
		//TO-DO : implement error message


	}

	//create dolly zoom node

	MFnDependencyNode dependNodeFn;
	MObject dependNodeObj;

	dependNodeObj = dependNodeFn.create("dollyZoom");

	//connect attributes

	MPlug inCameraMatrixPlug, inTargetMatrixPlug,
		  outCameraMatrixPlug, outTargetMatrixPlug;

	//find plugs part of the dolly zoom 
	


	// Camera matrix input in dollyZoom
	inCameraMatrixPlug = dependNodeFn.findPlug("cameraWorldMatrix",true,&status);

	// target matrix input in dollyZoom
	inTargetMatrixPlug = dependNodeFn.findPlug("targetWorldMatrix", true, &status);

	// camera world matrix output from camera transform
	 dependNodeFn.setObject(cameraDagPath.node());

	outCameraMatrixPlug = dependNodeFn.findPlug("worldMatrix",false, &status);
	outCameraMatrixPlug = outCameraMatrixPlug.elementByLogicalIndex(0, &status); //equivalent to getting item 0 from world matrix array

	// target world matrix output from target transform

	dependNodeFn.setObject(targetDagPath.node());

	outTargetMatrixPlug = dependNodeFn.findPlug("worldMatrix", false, &status);
	outTargetMatrixPlug = outTargetMatrixPlug.elementByLogicalIndex(0, &status); //equivalent to getting item 0 from world matrix array
	
	

	MDGModifier dgMod;
	status = dgMod.connect(outCameraMatrixPlug, inCameraMatrixPlug);
	CHECK_MSTATUS_AND_RETURN_IT(status);
	status = dgMod.connect(outTargetMatrixPlug, inTargetMatrixPlug);
	CHECK_MSTATUS_AND_RETURN_IT(status);
	dgMod.doIt();
	return status;
}

MStatus DollyZoomCmd::undoIt()
{
	MStatus status(MS::kSuccess);

	return status;
}

