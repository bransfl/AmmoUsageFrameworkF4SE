#include "Internal/Messaging.hpp"
#include "Internal/Parser.hpp"

namespace Internal
{
	void Messaging::Callback(F4SE::MessagingInterface::Message* a_msg)
	{
		switch (a_msg->type) {
			case F4SE::MessagingInterface::kGameDataReady: {
				Parser::ParsePrepMapsToMaps();
				break;
			}
			default: {
				break;
			}
		}
	}
} // namespace Internal::Messaging
