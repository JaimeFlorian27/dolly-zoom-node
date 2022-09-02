#include "dollyZoomNode.h"
#include <iostream>


MTypeId DollyZoomNode::id(0x8000);
MObject DollyZoomNode::aCameraHorizontalAperture;
MObject DollyZoomNode::aCameraWorldMatrix;
MObject DollyZoomNode::aTargetWorldMatrix;
MObject DollyZoomNode::aWidth;
MObject DollyZoomNode::aOutputFocalLength;


DollyZoomNode::DollyZoomNode()
{
}
DollyZoomNode::~DollyZoomNode()
{
}
void* DollyZoomNode::creator()
{
    return new DollyZoomNode();
}
MStatus DollyZoomNode::compute(const MPlug& plug, MDataBlock& data)
{
    MStatus status(MS::kSuccess);

    if (plug != aOutputFocalLength)
    {
        return MS::kUnknownParameter;
    }
    
    MTransformationMatrix cameraWorldMatrix = data.inputValue(aCameraWorldMatrix, &status).asMatrix();
    MTransformationMatrix targetWorldMatrix = data.inputValue(aTargetWorldMatrix, &status).asMatrix();

    double width = data.inputValue(aWidth, &status).asDouble();

    if (width <= 0.0)
    {
        width = 0.001;
    }

    double cameraAperture = data.inputValue(aCameraHorizontalAperture, &status).asDouble();

    if (cameraAperture<= 0.0)
    {
        cameraAperture= 0.001;
    }

    //Extracting the first row of the world matrix(translate) and storing it in MPoints to calculate their distance
    MPoint cameraTranslate(cameraWorldMatrix.getTranslation(MSpace::kWorld));
    MPoint targetTranslate(targetWorldMatrix.getTranslation(MSpace::kWorld));
    double distance = cameraTranslate.distanceTo(targetTranslate);
    MStreamUtils::stdOutStream() << distance<< '\n';
    if (distance <= 0.0)
    {
        MGlobal::displayInfo("distanceBroken");
        distance = 0.001;
    }

    if (distance >= 0.0)
    {
        MGlobal::displayInfo("distanceWorking");
    }
    //Calculate new AOV
    double newFOV{ DollyZoom::calculateAOV(width, distance) }; //returns new AOV, we need to convert to Focal Length
    
    newFOV = DollyZoom::angleOfViewToFocalLength(newFOV, cameraAperture);

    MDataHandle hOutput = data.outputValue(aOutputFocalLength, &status);

    CHECK_MSTATUS_AND_RETURN_IT(status);

    hOutput.setDouble(newFOV);
    hOutput.setClean();
    data.setClean(plug);


    return status;
}
MStatus DollyZoomNode::initialize()
{
    MStatus status(MS::kSuccess);


    MFnNumericAttribute numericAttr;

    aWidth = numericAttr.create("width", "width", MFnNumericData::kDouble);
    numericAttr.setDefault(4.0);
    numericAttr.setKeyable(true);
    addAttribute(aWidth);

    aCameraHorizontalAperture = numericAttr.create("horizontalAperture", "horizontalAperture", MFnNumericData::kDouble);
    numericAttr.setDefault(36.0);
    numericAttr.setKeyable(true);
    addAttribute(aCameraHorizontalAperture);

    aOutputFocalLength = numericAttr.create("focalLength", "focalLength", MFnNumericData::kDouble);
    numericAttr.setWritable(false);
    numericAttr.setStorable(false);
    addAttribute(aOutputFocalLength);

    MFnMatrixAttribute matrixAttr;

    aCameraWorldMatrix = matrixAttr.create("CameraWorldMatrix", "CameraWorldMatrix", MFnMatrixAttribute::kDouble);
    matrixAttr.setKeyable(true);
    addAttribute(aCameraWorldMatrix);

    aTargetWorldMatrix = matrixAttr.create("TargetWorldMatrix", "TargetWorldMatrix", MFnMatrixAttribute::kDouble);
    matrixAttr.setKeyable(true);
    addAttribute(aTargetWorldMatrix);

    attributeAffects(aCameraHorizontalAperture, aOutputFocalLength);
    attributeAffects(aCameraWorldMatrix, aOutputFocalLength);
    attributeAffects(aTargetWorldMatrix, aOutputFocalLength);
    attributeAffects(aWidth, aOutputFocalLength);

    return status;
}


