#include "camera.h"

Camera::Camera()
: FViewMatrix(MatrixInit::Identity)
, FProjection(MatrixInit::Identity)
, FProjectionView(MatrixInit::Identity)
{

}

Camera::~Camera()
{
	
}

void Camera::DefinePerspective(double parFovy, double parAspect, double parNear, double parFar)
{
	FProjection.AsPerspective(parFovy,parAspect, parNear, parFar);
	FProjectionView = FProjection*FViewMatrix;
}
void Camera::Roll(double parAngle)
{
	FViewMatrix = TMatrix4<double>::rotateZAxis(parAngle)*FViewMatrix;
	FProjectionView = FProjection*FViewMatrix;
}

void Camera::Yaw(double parAngle)
{
	FViewMatrix = TMatrix4<double>::rotateYAxis(parAngle)*FViewMatrix;
	FProjectionView = FProjection*FViewMatrix;
}

void Camera::Pitch(double parAngle)
{
	FViewMatrix = TMatrix4<double>::rotateXAxis(parAngle)*FViewMatrix;
	FProjectionView = FProjection*FViewMatrix;
}

void Camera::Translate(const TVec3<double>& parDir)
{
	FViewMatrix = TMatrix4<double>::translate(parDir)*FViewMatrix;
	FProjectionView = FProjection*FViewMatrix;
}