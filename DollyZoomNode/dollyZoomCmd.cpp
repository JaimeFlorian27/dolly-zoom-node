#include "dollyZoomCmd.h"

MSelectionList selectionList;
MObject nodeObj;
MString nodeName;

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
	//selected objects can be used as arguments
	syntax.useSelectionAsDefault(true);
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

bool DollyZoomCmd::isUndoable() const
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


	//check if nodes are transform nodes  -- IF CAMERA IS NEITHER A KTRANSFORM OR CAMERA SHAPE AND TARGET IS NOT TRANSFORM
	if ((!cameraDagPath.node().hasFn(MFn::kTransform) && !cameraDagPath.node().hasFn(MFn::kCamera)) && !targetDagPath.node().hasFn(MFn::kTransform))
	{
		status = MS::kFailure;
		MString msg("the provided arguments are invalid");
		displayError(msg);
		return status;


	}
	
	//check is children is a camera
	if (cameraDagPath.node().hasFn(MFn::kTransform))
	{
		cameraDagPath.extendToShape();	// IMPORTANT -- From here on, we are storing the dagPath of the cameraShape, regardless if a transform was passed
		if (!cameraDagPath.node().hasFn(MFn::kCamera)) 
		{
			status = MS::kFailure;
			MString msg("the first argument provided is not a camera ");
			displayError(msg);
			return status;
		}
	}



	//create dolly zoom node

	MFnDependencyNode dependNodeFn;
	MObject dependNodeObj;

	dependNodeObj = dependNodeFn.create("dollyZoom");

	//store its name
	nodeObj = MObject(dependNodeObj);
	nodeName = dependNodeFn.name();


	//connect attributes

	MPlug inCameraMatrixPlug, inTargetMatrixPlug, outFocalLenghtPlug, outHorizontalAperturePlug, //plugs part of the dollyZoom Node
		  outCameraMatrixPlug, outTargetMatrixPlug, //plugs part of the transform nodes
	 	  inFocalLenghtPlug, inHorizontalAperturePlug; //plugs part of the camera shape node

	

	//------------------find plugs part of the dolly zoom------------------

	// Camera matrix input in dollyZoom
	inCameraMatrixPlug = dependNodeFn.findPlug("cameraWorldMatrix",true,&status);

	// target matrix input in dollyZoom
	inTargetMatrixPlug = dependNodeFn.findPlug("targetWorldMatrix", true, &status);

	// focal lenght
	outFocalLenghtPlug = dependNodeFn.findPlug("focalLength", true, &status);

	// horizontal aperture
	outHorizontalAperturePlug = dependNodeFn.findPlug("outputAperture", true, &status);


	
	//------------------find plugs part of my cameraShape (camera and target)------------------

	dependNodeFn.setObject(cameraDagPath.node());
	
	inFocalLenghtPlug = dependNodeFn.findPlug("focalLength", true, &status);
	inHorizontalAperturePlug =  dependNodeFn.findPlug("horizontalFilmAperture", true, &status);



	//------------------find plugs part of my kTransforms (camera and target)------------------
	

	
	//We have been using the camera shape dag path, we now pop 1 (go one dag path up) so we can access the transform node
	cameraDagPath.pop(1);

	//now cameraDagPath is the transforms dag path
	dependNodeFn.setObject(cameraDagPath.node());

	// camera world matrix output from camera transform

	outCameraMatrixPlug = dependNodeFn.findPlug("worldMatrix",false, &status);
	outCameraMatrixPlug = outCameraMatrixPlug.elementByLogicalIndex(0, &status); //equivalent to getting item 0 from world matrix array

	// target world matrix output from target transform

	dependNodeFn.setObject(targetDagPath.node());

	outTargetMatrixPlug = dependNodeFn.findPlug("worldMatrix", false, &status);
	outTargetMatrixPlug = outTargetMatrixPlug.elementByLogicalIndex(0, &status); //equivalent to getting item 0 from world matrix array
	




	// -----Connect all the plugs-----

	MDGModifier dgMod;

	//connect world matrixes to the dolly zoom
	status = dgMod.connect(outCameraMatrixPlug, inCameraMatrixPlug);
	CHECK_MSTATUS_AND_RETURN_IT(status);

	status = dgMod.connect(outTargetMatrixPlug, inTargetMatrixPlug);
	CHECK_MSTATUS_AND_RETURN_IT(status);

	//connect focal length and aperture to the camera shape

	status = dgMod.connect(outFocalLenghtPlug, inFocalLenghtPlug);
	CHECK_MSTATUS_AND_RETURN_IT(status);

	status = dgMod.connect(outHorizontalAperturePlug, inHorizontalAperturePlug);
	CHECK_MSTATUS_AND_RETURN_IT(status);


	dgMod.doIt();

	//returns the node name as a resunt
	setResult(nodeName);

	return status;
} 

MStatus DollyZoomCmd::undoIt()
{
	MStatus status(MS::kSuccess);
	MDGModifier dgMod;

	dgMod.deleteNode(nodeObj);
	status = dgMod.doIt();
	

	return status;
}

