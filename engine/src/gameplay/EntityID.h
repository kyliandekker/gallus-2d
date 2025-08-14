#pragma once

#include <string>

namespace gallus
{
	namespace gameplay
	{
		class TransformComponent;

		//---------------------------------------------------------------------
		// EntityID
		//---------------------------------------------------------------------
		struct EntityID
		{
			EntityID(unsigned int a_ID) : m_iID(a_ID)
			{};
			EntityID()
			{};
			~EntityID() = default;

			/// <summary>
			/// Checks whether the entity is valid or not.
			/// </summary>
			/// <returns>True if the entity was valid, otherwise false.</returns>
			bool IsValid() const
			{
				return m_iID != INVALID;
			};

			/// <summary>
			/// Sets the entity to invalid.
			/// </summary>
			void SetInvalid()
			{
				m_iID = INVALID;
			}

			/// <summary>
			/// Retrieves the ID (as an integer).
			/// </summary>
			/// <returns>An integer containing the entity id.</returns>
			unsigned int GetID() const
			{
				return m_iID;
			}

			bool operator==(const EntityID& a_Other) const
			{
				return m_iID == a_Other.m_iID;
			}

			bool operator!=(const EntityID& a_Other) const
			{
				return m_iID != a_Other.m_iID;
			}

			bool operator<(const EntityID& a_Other) const
			{
				return m_iID < a_Other.m_iID;
			}
		protected:
			enum ID_State : unsigned int
			{
				INVALID = 0
			};
			unsigned int m_iID = INVALID;
		};
	}
}