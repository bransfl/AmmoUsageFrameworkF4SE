#include "Internal/Messaging.hpp"
// #include "Internal/Hooks.hpp"

namespace Internal::Messaging
{
	void Callback(F4SE::MessagingInterface::Message* a_msg)
	{
		switch (a_msg->type) {
			case F4SE::MessagingInterface::kGameDataReady: {
				// todo:
				//	parse from prepMaps to actual map
				break;
			}
			default: {
				break;
			}
		}
	}
} // namespace Internal::Messaging
