#pragma once

#include "DX12PCH.h"

#include "DX12Transform.h"

namespace gallus
{
	namespace graphics
	{
		namespace dx12
		{
#ifdef _EDITOR
			enum CameraMode
			{
				CameraMode_Editor,
				CameraMode_Game
			};
#endif // _EDITOR

			class Camera
			{
			public:
				Camera();

				void Init(float a_fWidth, float a_fHeight);
				void SetProjection(float a_fWidth, float a_fHeight, float a_fNearPlane, float a_fFarPlane);

				DX12Transform& GetTransform();
				const DX12Transform& GetTransform() const;
				DirectX::XMMATRIX GetViewMatrix() const;
				DirectX::XMMATRIX GetProjectionMatrix() const;

			private:
				DX12Transform m_Transform;
				DirectX::XMMATRIX m_ProjectionMatrix;
			};
		}
	}
}