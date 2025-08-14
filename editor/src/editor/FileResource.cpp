#ifdef _EDITOR

#include "FileResource.h"

#include <rapidjson/document.h>
#include <rapidjson/utils.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/prettywriter.h>
#include <unordered_map>

#include "AssetType.h"
#include "core/DataStream.h"
#include "logger/Logger.h"
#include "AssetDatabase.h"

#define JSON_FILE_RESOURCE_ASSETTYPE_VAR "assetType"

namespace gallus
{
	namespace editor
	{
		//---------------------------------------------------------------------
		// FileResource
		//---------------------------------------------------------------------
		FileResource::~FileResource()
		{}

		//---------------------------------------------------------------------
		const fs::path& FileResource::GetPath() const
		{
			return m_Path;
		}

		//---------------------------------------------------------------------
		FileResourceType FileResource::GetResourceType() const
		{
			return m_ResourceType;
		}

		//---------------------------------------------------------------------
		FileResource* FileResource::GetParent() const
		{
			return m_Parent;
		}

		const std::unordered_map<std::string, std::vector<AssetType>> FILE_ATLAS =
		{
			{ ".scene", { AssetType::Scene } },
			{ ".mat", { AssetType::Material } },
			{ ".png", { AssetType::Texture } },
			{ ".bmp", { AssetType::Texture } },
			{ ".wav", { AssetType::Sound, AssetType::Song, AssetType::VO } },
			{ ".anim", { AssetType::Animation } },
			{ ".hlsl", { AssetType::Shader } },
		};

		//---------------------------------------------------------------------
		bool loadMetadata(const fs::path a_Path, rapidjson::Document& a_Document)
		{
			core::DataStream data;
			fs::path metaPath = a_Path.generic_string() + ".meta";
			if (!file::LoadFile(metaPath, data))
			{
				LOGF(LOGSEVERITY_ERROR, LOG_CATEGORY_EDITOR, "Failed loading meta file '%s'.", metaPath.generic_string().c_str());
				return false;
			}

			a_Document.Parse(reinterpret_cast<char*>(data.data()), data.size());

			if (a_Document.HasParseError())
			{
				LOGF(LOGSEVERITY_ERROR, LOG_CATEGORY_EDITOR, "Failed loading data in meta file '%s'.", metaPath.generic_string().c_str());
				return false;
			}

			return true;
		}

		//---------------------------------------------------------------------
		bool FileResource::Scan()
		{
			m_Path = m_Path;

			if (!fs::exists(m_Path))
			{
				return false;
			}

			m_ResourceType = fs::is_directory(m_Path) ? FileResourceType::Folder : FileResourceType::File;

			// Scan the folder.
			if (m_ResourceType == FileResourceType::Folder)
			{
				m_aResources.clear();

				// Go through each file/folder and check their status.
				fs::directory_iterator ds = fs::directory_iterator(m_Path, std::filesystem::directory_options::skip_permission_denied);
				for (const auto& dirEntry : ds)
				{
					// Check for hidden or system files. These should not be added.
					DWORD attributes = GetFileAttributes(dirEntry.path().generic_string().c_str());
					if (attributes & FILE_ATTRIBUTE_HIDDEN || attributes & FILE_ATTRIBUTE_SYSTEM)
					{
						continue;
					}

					// If it is not a directory, it is a file and needs to get past the meta checks.
					if (!fs::is_directory(dirEntry.path()))
					{
						std::string extension = dirEntry.path().extension().generic_string();

						// Get the extension. If the extension is not recognized, it will just be ignored.
						if (FILE_ATLAS.find(extension) == FILE_ATLAS.end())
						{
							continue;
						}

						auto it = FILE_ATLAS.find(extension);

						// Use default asset type if not set.
						AssetType assetType = it->second[0];

						rapidjson::Document document;
						document.SetObject();
						bool hasMetadata = loadMetadata(dirEntry.path(), document);
						if (hasMetadata)
						{
							int iAssetType = 0;
							rapidjson::GetInt(document, JSON_FILE_RESOURCE_ASSETTYPE_VAR, iAssetType);
							assetType = static_cast<AssetType>(iAssetType);
						}

						FileResource resource;
						resource.m_Path = dirEntry.path();
						resource.m_Parent = this;
						resource.m_ResourceType = FileResourceType::File;
						resource.m_AssetType = assetType;
						if (!hasMetadata)
						{
							resource.SaveMetadata(document, document.GetAllocator());
						}
						m_aResources.push_back(resource);
					}
					else
					{
						// Create the resource that will be added.
						FileResource folderResource;
						folderResource.m_Path = dirEntry.path();
						folderResource.m_Parent = this;
						folderResource.m_ResourceType = FileResourceType::Folder;

						m_aResources.push_back(folderResource);
					}
				}
			}

			for (auto& resource : m_aResources)
			{
				resource.Scan();
			}

			return true;
		}

		void FileResource::Rename(const std::string& a_sName)
		{
			fs::path newPath = m_Path.parent_path().generic_string() + a_sName + m_Path.extension().generic_string();

			fs::rename(m_Path, newPath);
			m_Path = newPath;

			// TODO: Rescan.
		}

		void FileResource::Delete()
		{
			fs::remove(m_Path.c_str());
		}

		AssetType FileResource::GetAssetType() const
		{
			return m_AssetType;
		}

		void FileResource::SetAssetType(AssetType a_AssetType)
		{
			m_AssetType = a_AssetType;
		}

		bool FileResource::SaveMetadata(rapidjson::Document& a_Document, rapidjson::Document::AllocatorType& a_Allocator) const
		{
			fs::path metaPath = m_Path.generic_string() + ".meta";

			int assetType = (int) m_AssetType;
			a_Document.AddMember(JSON_FILE_RESOURCE_ASSETTYPE_VAR, assetType, a_Allocator);

			rapidjson::StringBuffer buffer;
			rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(buffer);
			a_Document.Accept(writer);

			return file::SaveFile(metaPath, core::Data(buffer.GetString(), buffer.GetSize()));
		}
	}
}

#endif // _EDITOR