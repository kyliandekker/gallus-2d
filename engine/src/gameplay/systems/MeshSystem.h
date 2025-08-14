#pragma once

#include "gameplay/ECSBaseSystem.h"
#include "gameplay/systems/components/MeshComponent.h"

namespace gallus
{
	namespace gameplay
	{
		//---------------------------------------------------------------------
		// MeshSystem
		//---------------------------------------------------------------------
		class MeshSystem : public ECSBaseSystem<MeshComponent>
		{
		public:
			/// <summary>
			/// Initializes the system, setting up necessary resources.
			/// </summary>
			/// <returns>True if the initialization was successful, otherwise false.</returns>
			bool Initialize() override;

			/// <summary>
			/// Retrieves the property name of the system (used in serialization).
			/// </summary>
			/// <returns>A string containing the property name of the system.</returns>
			std::string GetPropertyName() const override;

			/// <summary>
			/// Retrieves the name of the system.
			/// </summary>
			/// <returns>A string containing the name of the system.</returns>
			std::string GetSystemName() const override;

			/// <summary>
			/// Updates the system.
			/// </summary>
			/// <param name="a_fDeltaTime">The time that has passed since the last frame.</param>
			void Update(float a_fDeltaTime) override;
		};
	}
}