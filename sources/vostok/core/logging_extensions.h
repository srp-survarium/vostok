////////////////////////////////////////////////////////////////////////////
//	Created 	: 03.11.2025
////////////////////////////////////////////////////////////////////////////

#ifndef LOGGING_EXTENSIONS_H_INCLUDED
#define LOGGING_EXTENSIONS_H_INCLUDED

namespace vostok {

namespace logging {
	class log_file;
}

namespace core {

enum log_flags_enum {
	log_to_console		=	1 << 0,
	log_to_stderr		=	1 << 1,
};

typedef void (*debug_log_callback_type)(pcstr, bool, bool, pcstr);
typedef	void (*log_callback_type)(
	pvoid,					// user_data (log_flags_enum)
	pcstr,					// file
	u32	 ,					// line
	pcstr,					// function signature
	pcstr,					// initiator
	logging::verbosity,		// verbosity
	pcstr,					// log string
	u32	 ,					// log string length
	logging::callback_flag  // first/last string
);


extern logging::filter_tree*		g_log_filter_tree;
extern log_callback_type			g_log_callback;
extern log_flags_enum				g_log_flags;
extern logging::log_format			g_log_format;

extern logging::log_file*			g_log_file;
extern logging::log_file_usage_enum g_log_file_usage;


void	logging_initialize		( );
void	logging_finalize		( );

} // namespace core
} // namespace vostok

#endif // #ifndef LOGGING_EXTENSIONS_H_INCLUDED