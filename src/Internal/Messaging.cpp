#include "Internal/Messaging.hpp"
#include "Internal/Parser.hpp"

namespace Internal
{
	void Messaging::RegisterCallback()
	{
		logger::info("Registering for F4SE events..."sv);

		F4SE::GetMessagingInterface()->RegisterListener(Messaging::Callback);

		logger::info("Registered for F4SE events."sv);
	}
	
	void Messaging::Callback(F4SE::MessagingInterface::Message* a_msg)
	{
		switch (a_msg->type) {
			case F4SE::MessagingInterface::kGameDataReady: {
				Parser::ParsePrepMapsToMaps();

				logger::info("registering ammo event sink..."sv);
				break;
			}
			default: {
				break;
			}
		}
	}
} // namespace Internal::Messaging
