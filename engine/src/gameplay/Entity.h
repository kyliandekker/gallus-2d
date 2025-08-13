#pragma once

#include <string>

#include "EntityID.h"

namespace gallus
{
	namespace gameplay
	{
		//-----------------------------------------------------------------------------
		// Entity
		//-----------------------------------------------------------------------------
		/// <summary>
		/// Wrapper class that contains specific info all entities have.
		/// </summary>
		class Entity
		{
		public:
			Entity() = default;
			Entity(const EntityID& a_EntityID, const std::string& a_sName) :
				m_EntityID(a_EntityID),
				m_sName(a_sName)
			{}

			EntityID& GetEntityID()
			{
				return m_EntityID;
			}

			const EntityID& GetEntityID() const
			{
				return m_EntityID;
			}

			const std::string& GetName() const
			{
				return m_sName;
			}

			bool IsDestroyed() const
			{
				return m_bIsDestroyed;
			}

			void Destroy()
			{
				m_bIsDestroyed = true;
			}

			bool IsActive() const
			{
				return m_bIsActive;
			}

			void SetIsActive(bool a_bIsActive)
			{
				m_bIsActive = a_bIsActive;
			}
		private:
			EntityID m_EntityID;
			std::string m_sName;
			bool m_bIsDestroyed = false;
			bool m_bIsActive = true;
		};
	}
}