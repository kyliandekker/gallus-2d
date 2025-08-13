#pragma once

#include "graphics/dx12/DX12PCH.h"
#include "graphics/dx12/DX12Resource.h"

#include <string>
#include <vector>
#include <cstdint>
#include <memory>

#include "utils/file_abstractions.h"
#include "graphics/dx12/DX12Transform.h"
#include "graphics/dx12/IndexBuffer.h"
#include "graphics/dx12/VertexBuffer.h"

namespace gallus
{
	namespace graphics
	{
		namespace dx12
		{
			struct VertexPosUV
			{
				DirectX::XMFLOAT2 Position;
				DirectX::XMFLOAT2 UV;
			};

			struct MeshPartData
			{
				std::vector<VertexPosUV> m_aVertices;
				std::vector<uint16_t> m_aIndices;

				VertexBuffer m_VertexBuffer;
				IndexBuffer m_IndexBuffer;

				Microsoft::WRL::ComPtr<ID3D12Resource> m_pIntermediateVertexBuffer;
				Microsoft::WRL::ComPtr<ID3D12Resource> m_pIntermediateIndexBuffer;
			};

			class CommandList;

			class Mesh : public core::EngineResource
			{
			public:
				Mesh();
				void Render(std::shared_ptr<CommandList> a_pCommandList, const DX12Transform& a_Transform, const DirectX::XMMATRIX& a_CameraView, const DirectX::XMMATRIX& a_CameraProjection);
				bool IsValid() const override;

				bool LoadByName(const std::string& a_sName, const std::shared_ptr<CommandList> a_pCommandList);
			private:
				std::vector<MeshPartData*> m_aMeshData;
			};
		}
	}
}