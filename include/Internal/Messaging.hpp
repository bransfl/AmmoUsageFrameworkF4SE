#pragma once

namespace Internal
{
	class Messaging
	{
	public:
		/**
		 * @brief Asd.
		 */
		static void RegisterCallback();

		/**
		 * @brief Asd.
		 */
		static void Callback(F4SE::MessagingInterface::Message* a_msg);
	};
} // namespace Internal::Messaging
