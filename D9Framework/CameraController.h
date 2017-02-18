#pragma once
#include "Types.h"
#include "Frustum.h"
#include <memory>
#include <vector>

class BaseCamera;
class CameraController : public Singleton<CameraController>
{
public:
	CameraController();
	~CameraController();
	void AddCamera(std::shared_ptr<BaseCamera> newCamera);
	void RemoveCamera();
	void ComputeCamera();
	void Update(float elapsedTime);
	void SetWorld(const Matrix& world);
	void ResetWorld();
	void CreateViewMatrix();
	void CreateProjectionMatrix();
	std::shared_ptr<BaseCamera>	GetCurrentCamera() const;
	const Matrix& GetWorld() const;
	const Matrix& GetView() const;
	void SetProjection(const Matrix& newMat);
	void SetView(const Matrix& newMat);
	const Matrix& GetProjection() const;
	const Matrix& GetViewProjection() const;
	const Vector3D& GetCurrentCameraPosition() const;
	const Frustum& GetFrustum() const;
private:
	static const int c_MaxCameraCount = 16;
	std::shared_ptr<BaseCamera> m_currentCamera;
	std::vector<std::shared_ptr<BaseCamera>> m_cameras;
	int m_cameraCount;
	Matrix m_world;
	Matrix m_view;
	Matrix m_projection;
	Frustum m_viewFrustum;
};

