#include <maya/MPxCommand.h>
#include <maya/MSyntax.h>
#include <maya/MArgDatabase.h>
#include <maya/MSelectionList.h>
#include <maya/MFnDependencyNode.h>

#pragma once

class DollyZoomCmd : public MPxCommand
{
public:
	DollyZoomCmd();
	virtual ~DollyZoomCmd();
	static void* creator();
	static MStatus doIt(const MArgList &args); 
	static MStatus redoIt();
	static MStatus undoIt();
	static MSyntax newSyntax();
private:
	static MSelectionList selectionList;
};