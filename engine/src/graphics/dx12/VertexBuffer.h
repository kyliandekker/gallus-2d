#pragma once

#include "graphics/dx12/Buffer.h"

#include <string>

namespace gallus
{
	namespace graphics
	{
		namespace dx12
		{
			class VertexBuffer : public Buffer
			{
			public:
				VertexBuffer(const std::string& a_sName = "");
				virtual ~VertexBuffer();

				void CreateViews(size_t a_iNumElements, size_t a_iElementSize) override;
				D3D12_VERTEX_BUFFER_VIEW& GetVertexBufferView();
				size_t GetNumVertices() const;
				size_t GetVertexStride() const;
			private:
				size_t m_iNumVertices = 0;
				size_t m_iVertexStride = 0;

				D3D12_VERTEX_BUFFER_VIEW m_VertexBufferView;
			};
		}
	}
}