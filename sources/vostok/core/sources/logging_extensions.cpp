////////////////////////////////////////////////////////////////////////////
//	Created 	: 24.10.2025
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "logging_extensions.h"

#include <vostok/console_command.h>
#include <vostok/console_command_processor.h>
#include <vostok/debug/log_callback.h>
#include <vostok/fs_path.h>
#include <vostok/fs_utils.h>
#include <vostok/logging/api.h>
#include <vostok/logging/extensions.h>
#include <vostok/logging/log_file.h>
#include <vostok/logging/logging_filters_console_command.h>
#include "core_entry_point.h"
#include "testing_impl.h"

// sushi@TODO: Only exists on old VS builds and makes it platform dependant
extern "C" FILE* __iob_func(void);

namespace vostok {

namespace logging {
class filter_tree;
class log_file;
} // namespace logging

namespace core {

typedef void (*log_callback_type)(pcstr, bool, bool, pcstr);
typedef	void (*log_callback)(
	pvoid,					// ???
	pcstr,					// file
	u32	 ,					// line
	pcstr,					// function signature
	pcstr,					// ???
	logging::verbosity,		// verbosity
	pcstr,					// log string
	u32	 ,					// ???
	logging::callback_flag  // first/last string
);


logging::filter_tree*	g_log_filter_tree	= NULL; // sushi@NOTE: `logging::filter_tree` is stored privately
logging::log_file*		g_log_file			= NULL; // sushi@NOTE: `logging::log_file` is not included anywhere and is in the root
logging::log_flags_enum g_log_flags			= logging::log_to_console;
log_callback			g_log_callback		= NULL;

static vostok::command_line::key	s_use_console				("console",       "", "logging", "turns on console output"   );
static vostok::command_line::key	s_log_to_stdout				("log_to_stdout", "", "logging", "turns on writing to stdout");
static vostok::command_line::key	s_log_verbosity				("log_verbosity", "", "logging", "one of: [trace|debug|info|warning|error|silent]");
static vostok::command_line::key	s_write_errors_to_stderr	("write_errors_to_stderr", "", "logging", "");

static bool													s_console_initialized			=	false;
static vostok::logging::logging_filters_console_command*	s_logging_console_command		=	NULL;
// static bool   s_tried_to_initialize_console	=	false;

#if 0
// Statics with dynamic initializers
s_logging_preinitializer

void (__cdecl *s_logging_preinitializer_initializer_)();
#endif

// STATE[STUB]
void generate_log_file_name( fs_new::native_path_string* out_result, pcstr extension )
{
	ASSERT									(extension);
	ASSERT									(out_result);
	* out_result							= 	fs_new::native_path_string::convert(core::user_data_directory());
	out_result->append_path					(core::application_name());
	fs_new::native_path_string user_name	=	core::user_name();
	if ( user_name.length() )
		out_result->appendf					("_%s", user_name.c_str());

	out_result->appendf						(".%s", extension);
}

// STATE[STUB]
_iobuf* get_stdstream_handle( logging::stdstream_enum stream ) // stick@TODO: stdstream_enum moved to core
{
	if ( stream == logging::stdstream_out )
		return				&__iob_func()[1];
	else if ( stream == logging::stdstream_error )
		return				&__iob_func()[2];

	return					NULL;
}

// STATE[STUB]
void write_to_stdstream( logging::stdstream_enum stream, pcstr format, ... )
{
	_iobuf* handle		=	get_stdstream_handle( stream );	// <0x671d90>|0x000|0x000:'78'
	if ( !handle )											// <0x671dab>|0x01b|0x01b:'79'
		return;												// <1>
															// <2>
	va_list					mark;							// <3>
	va_start				( mark, format );				// <4>
															// <5>
	vfprintf				( handle, format, mark );		// <0x671daf>|0x01f|0x004:'85'
															// <1>
	va_end					( mark );						// <2>
}

// STATE[STUB]
bool is_logging_initialized( )
{
	return g_log_filter_tree != NULL;	// <0x671d60>|0x000|0x000:'92'
}

// STATE[STUB]
bool use_console_for_logging( )
{
	if ( !g_log_filter_tree )																		// <0x672611>|0x000|0x000:'97'
		return false;																				// <0x67261a>|0x009|0x009:'98'
																									// <1>
	static bool s_use_console_for_logging	=	testing::run_tests_command_line() || s_use_console;	// <0x67261f>|0x00e|0x005:'100'
	return						s_use_console_for_logging;											// <0x672671>|0x060|0x052:'101'
}

// STATE[STUB]
void logging_callback(
	void*						user_data,
	pcstr						file,
	u32							line,
	pcstr						function_signature,
	pcstr						initiator,
	logging::verbosity			verbosity,
	pcstr						log_string,
	u32							log_string_length,
	logging::callback_flag		flag
)
{
	// LOCALS
	// bool 						logged_to_stdout
	// bool 						log_to_console_settings
	// ******

	// STATICS
	// static bool 					first_time = <0xa72eac>;
	// static bool 					s_tried_to_initialize_console = <0x4c26022>;
	// static bool 					s_initialized_console = <0x4c26021>;
	// ******

	// FUNCTION BODY
	// 1
	// 2
	// <0x672759>|0x000|0x000:'121'
	// 1
	// 2
	// 3
	// 4
	// 5
	// 6
	// <0x672768>|0x00f|0x00f:'128'
	// <0x67276b>|0x012|0x003:'129'
	// <0x672772>|0x019|0x007:'130'
	// 1
	// 2
	// <0x67277a>|0x021|0x008:'133'
	// 1
	// 2
	// 3
	// <0x67278c>|0x033|0x012:'137'
	// <0x67279f>|0x046|0x013:'138'
	// <0x6727ac>|0x053|0x00d:'139'
	// 1
	// 2
	// 3
	// <0x6727be>|0x065|0x012:'143'
	// <0x6727c8>|0x06f|0x00a:'144'
	// 1
	// <0x6727cd>|0x074|0x005:'146'
	// <0x6727e0>|0x087|0x013:'147'
	// 1
	// 2
	// <0x6727e7>|0x08e|0x007:'150'
	// 1
	// 2
	// 3
	// <0x6727ec>|0x093|0x005:'154'
	// 1
	// 2
	// 3
	// <0x6727fa>|0x0a1|0x00e:'158'
	// 1
	// <0x672803>|0x0aa|0x009:'160'
	// <0x67280d>|0x0b4|0x00a:'161'
	// 1
	// 2
	// <0x672814>|0x0bb|0x007:'164'
	// 1
	// <0x67281d>|0x0c4|0x009:'166'
	// <0x67282d>|0x0d4|0x010:'167'
	// 1
	// 2
	// 3
	// <0x672831>|0x0d8|0x004:'171'
	// 1
	// <0x67283e>|0x0e5|0x00d:'173'
	// 1
	// 2
	// <0x672851>|0x0f8|0x013:'176'
	// 1
	// <0x67288c>|0x133|0x03b:'178'
	// 1
	// 2
	// ******
}

// STATE[STUB]
// void vostok::core::debug_log_callback(char const*, bool, bool, char const*)
void debug_log_callback(
	pcstr		initiator,
	bool		is_error_verbosity,
	bool		log_only_user_string,
	pcstr		message
)
{
	// LOCALS
	// log_flags_enum 				log_flags
	// strings::detail::tuples 		STR_JOINA_tuples_unique_identifier

//	pstr					key = 0;
//	STR_JOINA				( key, "-", key_raw );

	// ******

	logging::log_flags_enum const log_flags	=	s_write_errors_to_stderr ?
									logging::log_to_stderr : (logging::log_flags_enum)0;

	// STR_JOINA( initiator );
	// THIS MOST LIKELY CALLS INTO LOG MACRO?

	if ( g_log_file )
		g_log_file->flush( NULL );

	// FUNCTION BODY
	// 1
	// 2
	// 3
	// <0x672453>|0x000|0x000:'191'
	// 1
	// <0x672482>|0x02f|0x02f:'193'
	// 1
	// 2
	// <0x6724bc>|0x069|0x03a:'196'
	// 1
	// <0x672560>|0x10d|0x0a4:'198'
	// 1
	// <0x672565>|0x112|0x005:'200'
	// 1
	// 2
	// <0x6725eb>|0x198|0x086:'203'
	// <0x6725f5>|0x1a2|0x00a:'204'
	// ******
}

// STATE[STUB]
// void vostok::core::logging_preinitialize()
void logging_preinitialize( )
{
	// FUNCTION BODY
	// <0x6728b0>|0x000|0x000:'209'
	// 1
	// 2
	// 3
	// <0x6728b9>|0x009|0x009:'213'
	// ******
}

// STATE[STUB]
void push_logging_filters( )
{
	using namespace vostok;																									// <1>
	logging::verbosity	verbosity		=	logging::trace;																	// <2>
	fixed_string512		verbosity_string;																					// <0x6722c6>|0x000|0x000:'220'
	bool const log_verbosity_key_is_set	=	s_log_verbosity.is_set_as_string(& verbosity_string);							// <0x6722e6>|0x020|0x020:'221'
	if ( log_verbosity_key_is_set )																							// <0x67230b>|0x045|0x025:'222'
		verbosity						= logging::string_to_verbosity(verbosity_string.c_str());							// <0x67230f>|0x049|0x004:'223'
	else if ( testing::run_tests_command_line() && !vostok::debug::is_debugger_present() )									// <0x672323>|0x05d|0x014:'224'
		verbosity						= logging::warning;																	// <1>
																															// <2>
	//	logging::verbosity const verbosity_for_resources	=	log_verbosity_key_is_set ? verbosity : logging::warning;	// <3>
	logging::push_filter			( *g_log_filter_tree, "", verbosity, u32(-1) );											// <0x672331>|0x06b|0x00e:'228'
	//	logging::push_filter		( "core:fs", verbosity_for_resources, & memory::g_mt_allocator );						// <1>
	//	logging::push_filter		( "core:resources", verbosity_for_resources, & memory::g_mt_allocator );				// <2>
	//	logging::push_filter		( "core:resources:test", verbosity_for_resources, & memory::g_mt_allocator );			// <3>
	//	logging::push_filter		( "core:resources:device_manager", verbosity_for_resources, & memory::g_mt_allocator );	// <4>
																															// <5>
																															// <6> sushi@NOTE: New or empty line
	fs_new::native_path_string	cfg_file_path;																				// <0x672345>|0x07f|0x014:'235'
	if ( fs_new::convert_to_absolute_path(& cfg_file_path,																	// <1>
										  fs_new::native_path_string::convert("../../user_data/user.cfg"),					// <2>
										  assert_on_fail_false) )															// <0x672355>|0x08f|0x010:'238'
	{																														// <1>
		console_commands::execute_console_commands	( cfg_file_path, console_commands::execution_filter_early );			// <0x6723f2>|0x12c|0x09d:'240'
	}																														// <1>
																															// <2>
}

// STATE[STUB]
// void vostok::core::logging_initialize()
void logging_initialize( )
{
	// LOCALS
	// fs_new::native_path_string 	log_file_name
	// fs_new::device_file_system_no_watcher_proxy device
	// ******

	// FUNCTION BODY
	// 1
	// 2
	// 3
	// <0x672687>|0x000|0x000:'252'
	// 1
	// 2
	// 3
	// 4
	// 5
	// 6
	// 7
	// <0x672699>|0x012|0x012:'260'
	// 1
	// <0x6726d1>|0x04a|0x038:'262'
	// 1
	// 2
	// <0x6726d6>|0x04f|0x005:'265'
	// <0x6726df>|0x058|0x009:'266'
	// <0x6726ef>|0x068|0x010:'267'
	// <0x6726fc>|0x075|0x00d:'268'
	// <0x67271e>|0x097|0x022:'269'
	// 1
	// ******
}

// STATE[STUB]
void logging_finalize( )
{
	finalize_console( );									// <0x671dd0>|0x000|0x000:'275'
	logging::delete_log_file( g_log_file );					// <0x671e03>|0x033|0x033:'276'

	// VOSTOK_DELETE_IMPL( s_logging_console_command );	// <0x671e0d>|0x03d|0x00a:'278' // sushi@TODO: I am not sure which allocator (if any) is used here

	logging::delete_filter_tree( g_log_filter_tree );		// <0x671e47>|0x077|0x03a:'280'
	debug::set_log_callback( NULL );						// <0x671e51>|0x081|0x00a:'281'
}

// STATE[STUB]
bool initialize_console( )
{
	// OTHER SYMBOLS
	// CallSiteInfo(CallSiteInfoSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x660ffb }, type_index: TypeIndex(0x47cf7) })
	// CallSiteInfo(CallSiteInfoSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x66100b }, type_index: TypeIndex(0x1271b) })
	// CallSiteInfo(CallSiteInfoSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x661019 }, type_index: TypeIndex(0xc342) })
	// CallSiteInfo(CallSiteInfoSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x6610e3 }, type_index: TypeIndex(0x28f7) })
	// CallSiteInfo(CallSiteInfoSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x661178 }, type_index: TypeIndex(0x28f7) })
	// CallSiteInfo(CallSiteInfoSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x661208 }, type_index: TypeIndex(0x28f7) })
	// ******

	return false;
	// FUNCTION BODY
	// 1
	// <0x671fe9>|0x000|0x000:'294'
	// 1
	// <0x671ff4>|0x00b|0x00b:'296'
	// 1
	// <0x671ffb>|0x012|0x007:'298'
	// <0x672009>|0x020|0x00e:'299'
	// <0x672019>|0x030|0x010:'300'
	// <0x671fda>|-0x00f|-0x03f:'301'
	// <0x6720c7>|0x0de|0x0ed:'302'
	// <0x6720ce>|0x0e5|0x007:'303'
	// 1
	// 2
	// 3
	// 4
	// <0x6720d6>|0x0ed|0x008:'308'
	// <0x6720ee>|0x105|0x018:'309'
	// <0x6720f3>|0x10a|0x005:'310'
	// <0x67214f>|0x166|0x05c:'311'
	// 1
	// 2
	// 3
	// 4
	// <0x672171>|0x188|0x022:'316'
	// 1
	// <0x672180>|0x197|0x00f:'318'
	// <0x6721dc>|0x1f3|0x05c:'319'
	// 1
	// 2
	// 3
	// <0x672201>|0x218|0x025:'323'
	// <0x672213>|0x22a|0x012:'324'
	// <0x67221c>|0x233|0x009:'325'
	// <0x672278>|0x28f|0x05c:'326'
	// 1
	// 2
	// 3
	// <0x67229d>|0x2b4|0x025:'330'
	// 1
	// 2
	// 3
	// 4
	// 5
	// ******
}

// STATE[STUB]
// void vostok::core::finalize_console()
void finalize_console( )
{
	// FUNCTION BODY
	// <0x671d20>|0x000|0x000:'340'
	// 1
	// <0x671d2a>|0x00a|0x00a:'342'
	// <0x671d3e>|0x01e|0x014:'343'
	// <0x671d45>|0x025|0x007:'344'
	// <0x671d4e>|0x02e|0x009:'345'
	// 1
	// ******
}

} // namespace core
} // namespace vostok
