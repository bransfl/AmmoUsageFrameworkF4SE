#include "Internal\Events.hpp"

namespace Internal
{
	void Events::RegisterEvents() noexcept
	{
		// register player ammo count sink

		// auto eventDispatcher4 = GET_EVENT_DISPATCHER(PlayerAmmoCountEvent);
		//eventDispatcher4->AddEventSink(&playerAmmoCountEventSink);
	}

	/**
	 * @brief asd.
	 * @param a_event asd.
	 * @return Event continue statement.
	 */
	RE::BSEventNotifyControl Events::AmmoCountEventHandler::ProcessEvent(const RE::PlayerAmmoCountEvent& a_event, RE::BSTEventSource<RE::PlayerAmmoCountEvent>*)
	{
		bool eventHasValue = a_event.optionalValue.has_value();

		if (eventHasValue) {
			auto eventValue = a_event.optionalValue.value();
			uint32_t eventClipAmmo = eventValue.clipAmmo;
			uint32_t eventReserveAmmo = eventValue.reserveAmmo;

			logger::info("AmmoCountEventHandler did have value. clipAmmo: {}, reserveAmmo: {}"sv,
				eventClipAmmo, eventReserveAmmo);
		}
		else {
			logger::info("AmmoCountEventHandler did not have value"sv);
		}

		return RE::BSEventNotifyControl::kContinue;
	}
}
