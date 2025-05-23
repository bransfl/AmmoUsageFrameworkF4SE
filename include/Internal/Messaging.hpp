#pragma once

namespace Internal
{
	class Messaging
	{
	public:
		/**
		 * @brief Registers Callback() for message callbacks.
		 */
		static void RegisterCallback();

		/**
		 * @brief Processes F4SE callback messages.
		 */
		static void Callback(F4SE::MessagingInterface::Message* a_msg);
	};
}
