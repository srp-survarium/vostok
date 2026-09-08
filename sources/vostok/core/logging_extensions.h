// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef LOGGING_EXTENSIONS_H_INCLUDED
#define LOGGING_EXTENSIONS_H_INCLUDED

namespace vostok {

namespace logging {
	class log_file;
	class filter_tree;
}

namespace core {

enum log_flags_enum {
	log_to_stdout		=	0,
	log_to_console		=	1 << 0,
	log_to_stderr		=	1 << 1,
};

enum stdstream_enum { stdstream_out, stdstream_error };

extern VOSTOK_CORE_API logging::filter_tree*		g_log_filter_tree;
extern VOSTOK_CORE_API logging::log_callback_type	g_log_callback;
extern VOSTOK_CORE_API log_flags_enum				g_log_flags;
extern VOSTOK_CORE_API logging::log_format			g_log_format;

extern logging::log_file*			g_log_file;
extern logging::log_file_usage_enum g_log_file_usage;

VOSTOK_CORE_API void logging_preinitialize( );
VOSTOK_CORE_API void write_to_stdstream( stdstream_enum stream, pcstr format, ... );
VOSTOK_CORE_API bool use_console_for_logging( );

// sushi@TODO: Not sure those are needed here!
		void		logging_initialize		( );
		void		logging_finalize		( );

} // namespace core
} // namespace vostok

#endif // #ifndef LOGGING_EXTENSIONS_H_INCLUDED
