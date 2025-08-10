#pragma once

#include "graphics/dx12/DX12System3D.h"

namespace tool
{
	namespace graphics
	{
		namespace dx12
		{
			enum RootParameters
			{
				CBV,                // ConstantBuffer<ModelViewProjection> ModelViewProjectionCB : register(b0);
				TEX_SRV,            // Texture2D texture0 : register(t0);
				MATERIAL,            // ConstantBuffer<DirectionalLightCB> DirectionalLightCBCB : register(b1);
				LIGHT,            // ConstantBuffer<DirectionalLightCB> DirectionalLightCBCB : register(b2);
				GLOBAL_SETTINGS,    // ConstantBuffer<GlobalSettings> GlobalSettingsCB : register(b3); 
				NumRootParameters
			};

			//-----------------------------------------------------------------------------
			// DX12System3D
			//-----------------------------------------------------------------------------
			/// <summary>
			/// Represents a DirectX 12 rendering window, managing device resources, rendering, and synchronization. 3D rendering, but has pure virtual functions.
			/// </summary>
			class DX12System3DGame : public DX12System3D
			{
			protected:
				bool CreateRootSignature() override;

				void Loop() override;
			};
		}
	}
}