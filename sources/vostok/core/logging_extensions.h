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

extern logging::filter_tree*		g_log_filter_tree;
extern logging::log_callback_type	g_log_callback;
extern log_flags_enum				g_log_flags;
extern logging::log_format			g_log_format;

extern logging::log_file*			g_log_file;
extern logging::log_file_usage_enum g_log_file_usage;

// sushi@TODO: Not sure those are needed here!
		void		logging_initialize		( );
		void		logging_finalize		( );

} // namespace core
} // namespace vostok

#endif // #ifndef LOGGING_EXTENSIONS_H_INCLUDED
