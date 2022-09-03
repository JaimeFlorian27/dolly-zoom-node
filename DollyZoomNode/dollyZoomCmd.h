#include <maya/MPxCommand.h>
#include <maya/MSyntax.h>
#include <maya/MArgDatabase.h>
#include <maya/MSelectionList.h>
#include <maya/MFnDependencyNode.h>
#include <maya/MGlobal.h>
#include <maya/MDagPath.h>
#include <maya/MPlug.h>
#include <maya/MDGModifier.h>
#pragma once

class DollyZoomCmd : public MPxCommand
{
public:
	DollyZoomCmd();
	virtual ~DollyZoomCmd();
	static void* creator();
	virtual MStatus doIt(const MArgList &args); 
	virtual bool isUndoable() const;
	virtual MStatus redoIt();
	virtual MStatus undoIt();
	static MSyntax newSyntax();
private:
	MSelectionList selectionList;
	MObject nodeObj;
	MString nodeName;
};