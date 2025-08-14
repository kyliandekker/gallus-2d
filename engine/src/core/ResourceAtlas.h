#pragma once

#include "graphics/dx12/DX12PCH.h"

#include <vector>
#include <memory>

#include "utils/file_abstractions.h"

namespace gallus
{
	namespace graphics
	{
		namespace dx12
		{
			class Texture;
			class Shader;
			class Mesh;

			class CommandList;
		}
	}
	namespace core
	{
		constexpr uint32_t MAX_RESOURCES = 64;
		constexpr uint32_t MISSING = 0;

		//---------------------------------------------------------------------
		// ResourceAtlas
		//---------------------------------------------------------------------
		class ResourceAtlas
		{
		public:
			template<class T>
			std::shared_ptr<T> GetResource(std::vector<std::shared_ptr<T>>& a_vVector, const std::string& a_sName, const fs::path& a_Path);

			template<class T>
			int32_t HasResource(std::vector<std::shared_ptr<T>>& a_vVector, const std::string& a_sName, const fs::path& a_Path);

			std::shared_ptr<graphics::dx12::Texture> LoadTexture(const std::string& a_sName, std::shared_ptr<graphics::dx12::CommandList> a_pCommandList);
			std::shared_ptr<graphics::dx12::Texture> LoadTextureByDescription(const std::string& a_sName, D3D12_RESOURCE_DESC& a_Description);
			std::shared_ptr<graphics::dx12::Texture> LoadTextureEmpty(const std::string& a_sName);
			bool HasTexture(const std::string& a_sName);

			std::shared_ptr<graphics::dx12::Shader> LoadShader(const std::string& a_sVertexShader, const std::string& a_sPixelShader);
			bool HasShader(const std::string& a_sName);

			std::shared_ptr<graphics::dx12::Mesh> LoadMesh(const std::string& a_sName, std::shared_ptr<graphics::dx12::CommandList> a_pCommandList);
			bool HasMesh(const std::string& a_sName);

			std::shared_ptr<graphics::dx12::Shader> GetDefaultShader();
			std::shared_ptr<graphics::dx12::Texture> GetDefaultTexture();
			std::shared_ptr<graphics::dx12::Mesh> GetDefaultMesh();

			void TransitionResources(std::shared_ptr<graphics::dx12::CommandList> a_pCommandList);

			const std::vector<std::shared_ptr<graphics::dx12::Texture>>& GetTextures() const;
			const std::vector<std::shared_ptr<graphics::dx12::Shader>>& GetShaders() const;
			const std::vector<std::shared_ptr<graphics::dx12::Mesh>>& GetMeshes() const;

			void SetResourceFolder(const std::string& a_sResourceFolder)
			{
				m_sResourceFolder = a_sResourceFolder;
			}

			const std::string& GetResourceFolder() const
			{
				return m_sResourceFolder;
			}
		private:
			std::vector<std::shared_ptr<graphics::dx12::Texture>> m_aTextures;
			std::vector<std::shared_ptr<graphics::dx12::Shader>> m_aShaders;
			std::vector<std::shared_ptr<graphics::dx12::Mesh>> m_aMeshes;

			std::string m_sResourceFolder;
		};
	}
}