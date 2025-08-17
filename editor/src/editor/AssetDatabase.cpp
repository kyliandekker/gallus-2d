#include "AssetDatabase.h"

#include "logger/Logger.h"
#include "core/Tool.h"

namespace gallus
{
	namespace editor
	{
		//---------------------------------------------------------------------
		// AssetDatabase
		//---------------------------------------------------------------------
		bool AssetDatabase::Initialize()
		{
			m_AssetsRoot.m_Path = core::TOOL->GetResourceAtlas().GetResourceFolder();

			LOG(LOGSEVERITY_INFO_SUCCESS, LOG_CATEGORY_EDITOR, "Initialized asset database.");
			return System::Initialize();
		}

		//---------------------------------------------------------------------
		bool AssetDatabase::Destroy()
		{
			LOG(LOGSEVERITY_INFO_SUCCESS, LOG_CATEGORY_EDITOR, "Destroyed asset database.");
			return System::Destroy();
		}

		//---------------------------------------------------------------------
		void AssetDatabase::Rescan()
		{
			m_bRescan = true;
		}

		//---------------------------------------------------------------------
		void AssetDatabase::CheckAssetDatabase()
		{
			if (m_bRescan)
			{
				std::lock_guard<std::recursive_mutex> lock(m_AssetMutex);
				if (Scan())
				{
					m_bRescan = false;
				}
			}
		}

		//---------------------------------------------------------------------
		FileResource& AssetDatabase::GetRoot()
		{
			std::lock_guard<std::recursive_mutex> lock(m_AssetMutex);
			return m_AssetsRoot;
		}

		//---------------------------------------------------------------------
		bool AssetDatabase::Scan()
		{
			if (m_AssetsRoot.Scan())
			{
				LOG(LOGSEVERITY_INFO, LOG_CATEGORY_EDITOR, "Scanned asset database.");
				m_eOnScanCompleted();
				return true;
			}

			return false;
		}
	}
}