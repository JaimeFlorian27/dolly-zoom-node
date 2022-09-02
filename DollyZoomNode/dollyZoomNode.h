#pragma once
#include <maya/MPxNode.h>
#include <maya/MFnNumericAttribute.h>
#include <maya/MFnMatrixAttribute.h>
#include <maya/MMatrix.h>
#include <maya/MPoint.h>
#include "dollyZoom.h"
#include <maya/MGlobal.h>
#include <maya/MStreamUtils.h>
class DollyZoomNode : public MPxNode
{
public:
	//constructor and destructor
	DollyZoomNode();
	virtual ~DollyZoomNode();


	virtual MStatus compute(const MPlug& plug, MDataBlock& data);

	static MStatus initialize();
	static void *creator();

	static MTypeId id;
	static MObject aCameraHorizontalAperture;
	static MObject aCameraWorldMatrix;
	static MObject aTargetWorldMatrix;
	static MObject aWidth;
	static MObject aOutputFocalLength;
	static MObject aDistance;

};