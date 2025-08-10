#pragma once

#include "core/Settings.h"

namespace tool
{
	namespace editor
	{
		class EditorSettings : public core::Settings
		{
		public:
			/// <summary>
			/// Constructs settings with file name.
			/// </summary>
			/// <param name="a_sFileName">The name the settings file should have.</param>
			EditorSettings(const std::string& a_sFileName) : core::Settings(a_sFileName)
			{ }

			/// <summary>
			/// Sets whether the console should automatically scroll to the bottom.
			/// </summary>
			/// <param name="a_bScrollToBottom">True to enable auto-scrolling, otherwise false.</param>
			void SetScrollToBottom(bool a_bScrollToBottom);

			/// <summary>
			/// Checks if the console is set to auto-scroll to the bottom.
			/// </summary>
			/// <returns>True if auto-scrolling is enabled, otherwise false.</returns>
			bool GetScrollToBottom() const;

			/// <summary>
			/// Sets whether info log messages are displayed.
			/// </summary>
			/// <param name="a_bShowInfo">True to show info messages, otherwise false.</param>
			void SetShowInfo(bool a_bShowInfo);

			/// <summary>
			/// Checks if info log messages are displayed.
			/// </summary>
			/// <returns>True if info messages are visible, otherwise false.</returns>
			bool GetShowInfo() const;

			/// <summary>
			/// Sets whether test log messages are displayed.
			/// </summary>
			/// <param name="a_bShowText">True to show test messages, otherwise false.</param>
			void SetShowTest(bool a_bShowText);

			/// <summary>
			/// Checks if test log messages are displayed.
			/// </summary>
			/// <returns>True if test messages are visible, otherwise false.</returns>
			bool GetShowTest() const;

			/// <summary>
			/// Sets whether warning log messages are displayed.
			/// </summary>
			/// <param name="a_Warning">True to show warnings, otherwise false.</param>
			void SetShowWarning(bool a_Warning);

			/// <summary>
			/// Checks if warning log messages are displayed.
			/// </summary>
			/// <returns>True if warnings are visible, otherwise false.</returns>
			bool GetShowWarning() const;

			/// <summary>
			/// Sets whether error log messages are displayed.
			/// </summary>
			/// <param name="a_bShowError">True to show errors, otherwise false.</param>
			void SetShowError(bool a_bShowError);

			/// <summary>
			/// Checks if error log messages are displayed.
			/// </summary>
			/// <returns>True if errors are visible, otherwise false.</returns>
			bool GetShowError() const;

			/// <summary>
			/// Sets whether assertion log messages are displayed.
			/// </summary>
			/// <param name="a_bShowAssert">True to show assertion messages, otherwise false.</param>
			void SetShowAssert(bool a_bShowAssert);

			/// <summary>
			/// Checks if assertion log messages are displayed.
			/// </summary>
			/// <returns>True if assertions are visible, otherwise false.</returns>
			bool GetShowAssert() const;

			/// <summary>
			/// Sets whether success log messages are displayed.
			/// </summary>
			/// <param name="a_bShowSuccess">True to show success messages, otherwise false.</param>
			void SetShowSuccess(bool a_bShowSuccess);

			/// <summary>
			/// Checks if success log messages are displayed.
			/// </summary>
			/// <returns>True if success messages are visible, otherwise false.</returns>
			bool GetShowSuccess() const;

			/// <summary>
			/// Sets whether combined info and success log messages are displayed.
			/// </summary>
			/// <param name="a_bShowInfoSuccess">True to show combined info-success messages, otherwise false.</param>
			void SetShowInfoSuccess(bool a_bShowInfoSuccess);

			/// <summary>
			/// Checks if combined info and success log messages are displayed.
			/// </summary>
			/// <returns>True if combined info-success messages are visible, otherwise false.</returns>
			bool GetShowInfoSuccess() const;

			/// <summary>
			/// Sets whether "awesome" log messages are displayed.
			/// </summary>
			/// <param name="a_bShowAwesome">True to show "awesome" messages, otherwise false.</param>
			void SetShowAwesome(bool a_bShowAwesome);

			/// <summary>
			/// Checks if "awesome" log messages are displayed.
			/// </summary>
			/// <returns>True if "awesome" messages are visible, otherwise false.</returns>
			bool GetShowAwesome() const;

			/// <summary>
			/// Sets whether lighting it enabled.
			/// </summary>
			/// <param name="a_bUseLighting">True to enable lighting, otherwise false.</param>
			void SetUseLighting(bool a_bUseLighting);

			/// <summary>
			/// Checks if lighting is enabled.
			/// </summary>
			/// <returns>True if lighting is enabled, otherwise false.</returns>
			bool GetUseLighting() const;

			/// <summary>
			/// Sets whether grid it enabled.
			/// </summary>
			/// <param name="a_bUseGrid">True to enable grid, otherwise false.</param>
			void SetUseGrid(bool a_bUseGrid);

			/// <summary>
			/// Checks if grid is enabled.
			/// </summary>
			/// <returns>True if grid is enabled, otherwise false.</returns>
			bool GetUseGrid() const;
		private:
			/// <summary>
			/// Virtual method for loading specific vars.
			/// </summary>
			/// <returns>True if the settings were loaded successfully, otherwise false.</returns>
			bool LoadVars(const rapidjson::Document& a_Document) override;

			/// <summary>
			/// Virtual method for saving specific vars.
			/// </summary>
			/// <returns>True if the settings were saved successfully, otherwise false.</returns>
			bool SaveVars(rapidjson::Document& a_Document, rapidjson::Document::AllocatorType& a_Allocator) const override;

			bool m_bScrollToBottom = false; /// Auto-scroll setting for the console.
			bool m_bShowInfo = true; /// Visibility toggle for info log messages.
			bool m_bShowTest = true; /// Visibility toggle for test log messages.
			bool m_bShowWarning = true; /// Visibility toggle for warning log messages.
			bool m_bShowError = true; /// Visibility toggle for error log messages.
			bool m_bShowAssert = true; /// Visibility toggle for assertion log messages.
			bool m_bShowSuccess = true; /// Visibility toggle for success log messages.
			bool m_bShowInfoSuccess = true; /// Visibility toggle for combined info-success messages.
			bool m_bShowAwesome = true; /// Visibility toggle for "awesome" log messages.

			bool m_bUseLighting = true; /// Toggle for enabling lighting.
			bool m_bUseGrid = true; /// Toggle for enabling grid.

		};
	}
}