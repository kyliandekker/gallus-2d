#pragma once

#include "graphics/dx12/DX12PCH.h"
#include "core/EngineResource.h"

#include <string>
#include <wrl.h>
#include <memory>

#include "utils/file_abstractions.h"

namespace gallus
{
	namespace graphics
	{
		namespace dx12
		{
			class CommandList;

			class Shader : public core::EngineResource
			{
			public:
				Shader() = default;
				void Bind(std::shared_ptr<CommandList> a_pCommandList);

				static Microsoft::WRL::ComPtr<ID3DBlob> CompileShader(const fs::path& a_sFilePath, const std::string& a_sEntryPoint, const std::string& a_sTarget);

				bool IsValid() const
				{
					return m_pPipelineState.Get();
				};

				const std::string& GetPixelPath() const;
				const std::string& GetVertexPath() const;

				bool LoadByName(const std::string& a_sVertexShader, const std::string& a_sPixelShader);
				bool LoadByPath(const fs::path& a_VertexShaderPath, const fs::path& a_PixelShaderPath);
			private:
				Microsoft::WRL::ComPtr<ID3D12PipelineState> m_pPipelineState = nullptr;
				std::string m_sPixelName;

				friend class ResourceAtlas;
			};
		}
	}
}