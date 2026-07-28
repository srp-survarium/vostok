#include "pch.h"
#include "anchor.h"

namespace vostok
{
	void use_log()
	{
		LOG_WARNING("ERROR %d", 10);
		logging::append(
			logging::log_callback_boost( core::g_log_callback ),
			(void*)0,
			logging::format_specifier( logging::format_specifier_time_brief ),
			"file",
			41,
			"use_log",
			"game_core:",
			logging::warning,
			"%s",
			"Hello!"
		);

		logging::log_format lf;

		logging::append(
			logging::log_callback_boost( core::g_log_callback ),
			(void*)0,
			&lf,
			"file",
			41,
			"use_log",
			"game_core:",
			logging::warning,
			"%s",
			"Hello!"
		);

	}

	void anchor_logging( )
	{
		use_log();
	}
}
