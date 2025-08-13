#include "DX12Transform.h"

#include <iostream>

namespace gallus
{
	namespace graphics
	{
		namespace dx12
		{
			//-----------------------------------------------------------------------------
			// DX12Transform
			//-----------------------------------------------------------------------------
			DX12Transform::DX12Transform()
				: m_vPosition{ 0.0f, 0.0f },
				m_fRotationDegrees(0),
				m_vScale{ 1.0f, 1.0f }
			{}

			//-----------------------------------------------------------------------------------------------------
			void DX12Transform::SetPosition(const DirectX::XMFLOAT2& a_vPosition)
			{
				m_vPosition = a_vPosition;
			}

			//-----------------------------------------------------------------------------------------------------
			void DX12Transform::SetRotation(float a_fRotationDegrees)
			{
				m_fRotationDegrees = a_fRotationDegrees;
			}

			//-----------------------------------------------------------------------------------------------------
			void DX12Transform::SetScale(const DirectX::XMFLOAT2& a_Scale)
			{
				m_vScale = a_Scale;
			}

			//-----------------------------------------------------------------------------------------------------
			const DirectX::XMFLOAT2& DX12Transform::GetPosition() const
			{
				return m_vPosition;
			}

			//-----------------------------------------------------------------------------------------------------
			float DX12Transform::GetRotation() const
			{
				return m_fRotationDegrees;
			}

			//-----------------------------------------------------------------------------------------------------
			const DirectX::XMFLOAT2& DX12Transform::GetScale() const
			{
				return m_vScale;
			}

			//-----------------------------------------------------------------------------------------------------
			const DirectX::XMMATRIX DX12Transform::GetWorldMatrix() const
			{
				//const DirectX::XMFLOAT2 origin = { 0.5f, 0.5f };
				//const DirectX::XMMATRIX originMatrix = DirectX::XMMatrixTranslation(origin.x, origin.y, 0.0f);
				//const DirectX::XMMATRIX negativeOriginMatrix = DirectX::XMMatrixTranslation(-origin.x, -origin.y, 0.0f);

				//const DirectX::XMMATRIX scaleMatrix = DirectX::XMMatrixScaling(m_vScale.x, m_vScale.y, 1.0f);
				//const float radians = DirectX::XMConvertToRadians(m_fRotationDegrees);
				//const DirectX::XMMATRIX rotationMatrix = DirectX::XMMatrixRotationZ(radians);
				//const DirectX::XMMATRIX translationMatrix = DirectX::XMMatrixTranslation(m_vPosition.x, m_vPosition.y, 0.0f);

				//return negativeOriginMatrix * rotationMatrix * scaleMatrix * translationMatrix * originMatrix;

				const DirectX::XMMATRIX scaleMatrix = DirectX::XMMatrixScaling(m_vScale.x, m_vScale.y, 1.0f);
				const float radians = DirectX::XMConvertToRadians(m_fRotationDegrees);
				const DirectX::XMMATRIX rotationMatrix = DirectX::XMMatrixRotationZ(radians);
				const DirectX::XMMATRIX translationMatrix = DirectX::XMMatrixTranslation(m_vPosition.x, m_vPosition.y, 0.0f);

				return scaleMatrix * rotationMatrix * translationMatrix;
			}

			//-----------------------------------------------------------------------------------------------------
			void DX12Transform::SetWorldMatrix(const DirectX::XMMATRIX& worldMatrix)
			{}
		}
	}
}