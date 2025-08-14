#pragma once

#include "gameplay/systems/components/Component.h"

#include "graphics/dx12/DX12Transform.h"

namespace gallus
{
	namespace gameplay
	{
		//---------------------------------------------------------------------
		// TransformComponent
		//---------------------------------------------------------------------
		class TransformComponent : public Component
		{
		public:
			/// <summary>
			/// Retrieves the transform.
			/// </summary>
			/// <returns>Reference to the transform used in the transform component.</returns>
			graphics::dx12::DX12Transform& Transform();

			/// <summary>
			/// Serialized the component to a json document.
			/// </summary>
			/// <param name="a_Document">The json document that the data will be put into.</param>
			/// <param name="a_Allocator">The allocator used by the json document.</param>
			void Serialize(rapidjson::Value& a_Document, rapidjson::Document::AllocatorType& a_Allocator) const override;

			/// <summary>
			/// Deserializes data from a json document and loads it into the component.
			/// </summary>
			/// <param name="a_Document">The json document that contains the data.</param>
			/// <param name="a_Allocator">The allocator used by the json document.</param>
			void Deserialize(const rapidjson::Value& a_Document, rapidjson::Document::AllocatorType& a_Allocator) override;
		private:
			graphics::dx12::DX12Transform m_Transform;
		};
	}
}