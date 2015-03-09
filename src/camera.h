#ifndef CAMERA
#define CAMERA

#include "matrix.h"

class Camera
{
	public:
		Camera();
		~Camera();

		void Yaw(double parAngle);
		void Pitch(double parAngle);
		void Roll(double parAngle);
		void Translate(const TVec3<double>& parDir);

		const TMatrix4<double>& GetViewMatrix() const{ return FViewMatrix;}
		const TMatrix4<double>& GetProjectionMatrix() const{ return FProjection;}
		const TMatrix4<double>& GetProjectionViewMatrix() const{ return FProjectionView;}

		void DefinePerspective(double parFovy, double parAspect, double parNear, double parFar);

		protected:
			TMatrix4<double> FViewMatrix;
			TMatrix4<double> FProjection;
			TMatrix4<double> FProjectionView;
};

#endif //CAMERA
