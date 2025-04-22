#include "Internal/Messaging.hpp"
// #include "Internal/Hooks.hpp"

namespace Internal::Messaging
{
	void Callback(F4SE::MessagingInterface::Message* a_msg)
	{
		switch (a_msg->type) {
			case F4SE::MessagingInterface::kPostLoadGame: {
				// logger::info("installing hooks..."sv);
				// Internal::Hooks::Install();
				// logger::info("hooks installed."sv);
				break;
			}
			default: {
				break;
			}
		}
	}
} // namespace Internal::Messaging
