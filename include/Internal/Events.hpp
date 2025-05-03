#pragma once

#include "RE\Bethesda\Events.hpp"

namespace Internal
{
	class Events
	{
		/**
		 * @brief Asd.
		 */
		static void RegisterEvents() noexcept;

		class AmmoCountEventHandler
			: public RE::BSTEventSink<RE::PlayerAmmoCountEvent>
		{
		public:
			/**
			 * @brief Singleton getter for class.
			 * @return Class Singleton.
			 */
			[[nodiscard]] static AmmoCountEventHandler* GetSingleton()
			{
				static AmmoCountEventHandler singleton;
				return std::addressof(singleton);
			}

			/**
			 * @brief asd.
			 * @param a_event asd.
			 * @return Event continue statement.
			 */
			virtual RE::BSEventNotifyControl ProcessEvent(const RE::PlayerAmmoCountEvent& a_event, RE::BSTEventSource<RE::PlayerAmmoCountEvent>*) override;

			AmmoCountEventHandler() = default;
			AmmoCountEventHandler(const AmmoCountEventHandler&) = delete;
			AmmoCountEventHandler(AmmoCountEventHandler&&) = delete;
			~AmmoCountEventHandler() = default;
			AmmoCountEventHandler& operator=(const AmmoCountEventHandler&) = delete;
			AmmoCountEventHandler& operator=(AmmoCountEventHandler&&) = delete;
		};
	};
};
