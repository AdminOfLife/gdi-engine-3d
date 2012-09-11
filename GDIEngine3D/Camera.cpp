
#include "stdafx.h"
#include "Camera.h"

// ============================================================================
// CCamera partial implementation:

void CCamera::InitDefaultValues(CCamera *cam)
{
	cam->projectionType	= PT_PARALLEL;
	cam->hFOV			= (float)M_PI_2;
	cam->vFOV			= (float)M_PI_2;
	cam->nearClip		= 1.0F;
	cam->farClip		= 1000.0F;
	cam->fDist			= 499.5F; // (farClip - nearClip) / 2
}

CCamera::CCamera() 
	: CObject(CLS_CAMERA) 
{ 
	InitDefaultValues(this);
}

CCamera::CCamera(PROJECTION_TYPE projType, float horizFov, float vertFov) 
	: CObject(CLS_CAMERA) 
{ 
	InitDefaultValues(this);
	projectionType	= projType;
	hFOV			= horizFov;
	vFOV			= vertFov;
}

void CCamera::GetViewMatrix(MATRIX3D &mOut)
{
	mOut._11 = rWd.x;
	mOut._12 = uWd.x;
	mOut._13 = fWd.x;
	mOut._14 = .0f;

	mOut._21 = rWd.y;
	mOut._22 = uWd.y;
	mOut._23 = fWd.y;
	mOut._24 = .0f;

	mOut._31 = rWd.z;
	mOut._32 = uWd.z;
	mOut._33 = fWd.z;
	mOut._34 = .0f;

	mOut._41 = -Vector3DMultS(rWd, pos);
	mOut._42 = -Vector3DMultS(uWd, pos);
	mOut._43 = -Vector3DMultS(fWd, pos);
	mOut._44 = 1.0f;
}

void CCamera::GetProjectionMatrix(MATRIX3D &mOut)
{
	mOut.SetIdentity();

	mOut._11 = 1 / tan(hFOV / 2.0f);
	mOut._22 = 1 / tan(vFOV / 2.0f);
	mOut._33 = farClip / (farClip - nearClip);
	mOut._43 = mOut._33 * (-nearClip);
	mOut._34 = fDist;
	if ( projectionType == PT_CENTRAL ) 
	{
		mOut._11 *= mOut._34;
		mOut._22 *= mOut._34;
		mOut._44 = -mOut._44;
	}
}