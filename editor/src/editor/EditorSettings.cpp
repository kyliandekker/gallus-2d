#include "EditorSettings.h"

#include <rapidjson/document.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/prettywriter.h>
#include <rapidjson/utils.h>
#include <string>

#define JSON_CONSOLE_VAR "console"
#define JSON_CONSOLE_SCROLLTOBOTTOM_VAR "scrollToBottom"
#define JSON_CONSOLE_INFO_VAR "info"
#define JSON_CONSOLE_TEST_VAR "test"
#define JSON_CONSOLE_WARNING_VAR "warning"
#define JSON_CONSOLE_ERROR_VAR "error"
#define JSON_CONSOLE_ASSERT_VAR "assert"
#define JSON_CONSOLE_SUCCESS_VAR "success"
#define JSON_CONSOLE_INFOSUCCESS_VAR "infoSuccess"
#define JSON_CONSOLE_AWESOME_VAR "awesome"

#define JSON_USE_LIGHTING_VAR "useLighting"
#define JSON_USE_GRID_VAR "useGrid"

namespace gallus
{
    namespace editor
    {
        //-----------------------------------------------------------------------------
        // Settings
        //-----------------------------------------------------------------------------
        bool EditorSettings::LoadVars(const rapidjson::Document& a_Document)
        {

            if (a_Document.HasMember(JSON_CONSOLE_VAR) && a_Document[JSON_CONSOLE_VAR].IsObject())
            {
                // Scroll to Bottom
                GetBool(a_Document[JSON_CONSOLE_VAR], JSON_CONSOLE_SCROLLTOBOTTOM_VAR, m_bScrollToBottom);

                GetBool(a_Document[JSON_CONSOLE_VAR], JSON_CONSOLE_ASSERT_VAR, m_bShowAssert);
                GetBool(a_Document[JSON_CONSOLE_VAR], JSON_CONSOLE_ERROR_VAR, m_bShowError);
                GetBool(a_Document[JSON_CONSOLE_VAR], JSON_CONSOLE_WARNING_VAR, m_bShowWarning);
                GetBool(a_Document[JSON_CONSOLE_VAR], JSON_CONSOLE_INFO_VAR, m_bShowInfo);
                GetBool(a_Document[JSON_CONSOLE_VAR], JSON_CONSOLE_TEST_VAR, m_bShowTest);
                GetBool(a_Document[JSON_CONSOLE_VAR], JSON_CONSOLE_SUCCESS_VAR, m_bShowSuccess);
                GetBool(a_Document[JSON_CONSOLE_VAR], JSON_CONSOLE_INFOSUCCESS_VAR, m_bShowInfoSuccess);
                GetBool(a_Document[JSON_CONSOLE_VAR], JSON_CONSOLE_AWESOME_VAR, m_bShowAwesome);
            }

            // Use lighting
            if (a_Document.HasMember(JSON_USE_LIGHTING_VAR) && a_Document[JSON_USE_LIGHTING_VAR].IsBool())
            {
                GetBool(a_Document, JSON_USE_LIGHTING_VAR, m_bUseLighting);
            }

            // Use grid
            if (a_Document.HasMember(JSON_USE_GRID_VAR) && a_Document[JSON_USE_GRID_VAR].IsBool())
            {
                GetBool(a_Document, JSON_USE_GRID_VAR, m_bUseGrid);
            }

            return true;
        }

        //-----------------------------------------------------------------------------------------------------
        bool EditorSettings::SaveVars(rapidjson::Document& a_Document, rapidjson::Document::AllocatorType& a_Allocator) const
        {
            a_Document.AddMember(JSON_CONSOLE_VAR, rapidjson::Value().SetObject(), a_Allocator);
            a_Document[JSON_CONSOLE_VAR].AddMember(JSON_CONSOLE_SCROLLTOBOTTOM_VAR, m_bScrollToBottom, a_Allocator);
            a_Document[JSON_CONSOLE_VAR].AddMember(JSON_CONSOLE_ASSERT_VAR, m_bShowAssert, a_Allocator);
            a_Document[JSON_CONSOLE_VAR].AddMember(JSON_CONSOLE_ERROR_VAR, m_bShowError, a_Allocator);
            a_Document[JSON_CONSOLE_VAR].AddMember(JSON_CONSOLE_WARNING_VAR, m_bShowWarning, a_Allocator);
            a_Document[JSON_CONSOLE_VAR].AddMember(JSON_CONSOLE_INFO_VAR, m_bShowInfo, a_Allocator);
            a_Document[JSON_CONSOLE_VAR].AddMember(JSON_CONSOLE_TEST_VAR, m_bShowTest, a_Allocator);
            a_Document[JSON_CONSOLE_VAR].AddMember(JSON_CONSOLE_SUCCESS_VAR, m_bShowSuccess, a_Allocator);
            a_Document[JSON_CONSOLE_VAR].AddMember(JSON_CONSOLE_INFOSUCCESS_VAR, m_bShowInfoSuccess, a_Allocator);
            a_Document[JSON_CONSOLE_VAR].AddMember(JSON_CONSOLE_AWESOME_VAR, m_bShowAwesome, a_Allocator);

            a_Document.AddMember(JSON_USE_LIGHTING_VAR, m_bUseLighting, a_Allocator);
            a_Document.AddMember(JSON_USE_GRID_VAR, m_bUseGrid, a_Allocator);

            return true;
        }

        //-----------------------------------------------------------------------------------------------------
        void EditorSettings::SetScrollToBottom(bool a_bScrollToBottom)
        {
            m_bScrollToBottom = a_bScrollToBottom;
        }

        //-----------------------------------------------------------------------------------------------------
        bool EditorSettings::GetScrollToBottom() const
        {
            return m_bScrollToBottom;
        }

        //-----------------------------------------------------------------------------------------------------
        void EditorSettings::SetShowInfo(bool a_bShowInfo)
        {
            m_bShowInfo = a_bShowInfo;
        }

        //-----------------------------------------------------------------------------------------------------
        bool EditorSettings::GetShowInfo() const
        {
            return m_bShowInfo;
        }

        //-----------------------------------------------------------------------------------------------------
        void EditorSettings::SetShowTest(bool a_bShowText)
        {
            m_bShowTest = a_bShowText;
        }

        //-----------------------------------------------------------------------------------------------------
        bool EditorSettings::GetShowTest() const
        {
            return m_bShowTest;
        }

        //-----------------------------------------------------------------------------------------------------
        void EditorSettings::SetShowWarning(bool a_bShowWarning)
        {
            m_bShowWarning = a_bShowWarning;
        }

        //-----------------------------------------------------------------------------------------------------
        bool EditorSettings::GetShowWarning() const
        {
            return m_bShowWarning;
        }

        //-----------------------------------------------------------------------------------------------------
        void EditorSettings::SetShowError(bool a_bShowError)
        {
            m_bShowError = a_bShowError;
        }

        //-----------------------------------------------------------------------------------------------------
        bool EditorSettings::GetShowError() const
        {
            return m_bShowError;
        }

        //-----------------------------------------------------------------------------------------------------
        void EditorSettings::SetShowAssert(bool a_bShowAssert)
        {
            m_bShowAssert = a_bShowAssert;
        }

        //-----------------------------------------------------------------------------------------------------
        bool EditorSettings::GetShowAssert() const
        {
            return m_bShowAssert;
        }

        //-----------------------------------------------------------------------------------------------------
        void EditorSettings::SetShowSuccess(bool a_bShowSuccess)
        {
            m_bShowSuccess = a_bShowSuccess;
        }

        //-----------------------------------------------------------------------------------------------------
        bool EditorSettings::GetShowSuccess() const
        {
            return m_bShowSuccess;
        }

        //-----------------------------------------------------------------------------------------------------
        void EditorSettings::SetShowInfoSuccess(bool a_bShowInfoSuccess)
        {
            m_bShowInfoSuccess = a_bShowInfoSuccess;
        }

        //-----------------------------------------------------------------------------------------------------
        bool EditorSettings::GetShowInfoSuccess() const
        {
            return m_bShowInfoSuccess;
        }

        //-----------------------------------------------------------------------------------------------------
        void EditorSettings::SetShowAwesome(bool a_bShowAwesome)
        {
            m_bShowAwesome = a_bShowAwesome;
        }

        //-----------------------------------------------------------------------------------------------------
        bool EditorSettings::GetShowAwesome() const
        {
            return m_bShowAwesome;
        }

        //-----------------------------------------------------------------------------------------------------
        void EditorSettings::SetUseLighting(bool a_bUseLighting)
        {
            m_bUseLighting = a_bUseLighting;
        }

        //-----------------------------------------------------------------------------------------------------
        bool EditorSettings::GetUseLighting() const
        {
            return m_bUseGrid;
        }

        //-----------------------------------------------------------------------------------------------------
        void EditorSettings::SetUseGrid(bool a_bUseGrid)
        {
            m_bUseGrid = a_bUseGrid;
        }

        //-----------------------------------------------------------------------------------------------------
        bool EditorSettings::GetUseGrid() const
        {
            return m_bUseGrid;
        }
    }
}