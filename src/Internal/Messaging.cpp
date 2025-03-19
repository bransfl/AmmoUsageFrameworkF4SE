#include "Internal/Messaging.hpp"
// #include "Internal/Hooks.hpp"

namespace Internal::Messaging
{
	void Callback(F4SE::MessagingInterface::Message* a_msg)
	{
		switch (a_msg->type) {
			case F4SE::MessagingInterface::kPostLoad: {
				break;
			}
			case F4SE::MessagingInterface::kPostPostLoad: {
				break;
			}
			case F4SE::MessagingInterface::kPreLoadGame: {
				break;
			}
			case F4SE::MessagingInterface::kPostLoadGame: {
				// logger::info("installing hooks..."sv);
				// Internal::Hooks::Install();
				// logger::info("hooks installed."sv);
				break;
			}
			case F4SE::MessagingInterface::kPreSaveGame: {
				break;
			}
			case F4SE::MessagingInterface::kPostSaveGame: {
				break;
			}
			case F4SE::MessagingInterface::kDeleteGame: {
				break;
			}
			case F4SE::MessagingInterface::kInputLoaded: {
				break;
			}
			case F4SE::MessagingInterface::kNewGame: {
				break;
			}
			case F4SE::MessagingInterface::kGameLoaded: {
				break;
			}
			case F4SE::MessagingInterface::kGameDataReady: {
				break;
			}
			default: {
				break;
			}
		}
	}
} // namespace Internal::Messaging
