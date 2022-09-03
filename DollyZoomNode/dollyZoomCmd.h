#include <maya/MPxCommand.h>
#include <maya/MSyntax.h>
#include <maya/MArgDatabase.h>
#include <maya/MSelectionList.h>
#include <maya/MFnDependencyNode.h>
#include <maya/MGlobal.h>

#pragma once

class DollyZoomCmd : public MPxCommand
{
public:
	DollyZoomCmd();
	virtual ~DollyZoomCmd();
	static void* creator();
	virtual MStatus doIt(const MArgList &args); 
	//static bool isUndoable();
	virtual MStatus redoIt();
	//static MStatus undoIt();
	//static MSyntax newSyntax();
private:
	static MSelectionList selectionList;
};