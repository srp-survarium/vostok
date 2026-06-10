////////////////////////////////////////////////////////////////////////////
//	Created 	: 24.10.2025
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include <vostok/core/logging_extensions.h>

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

#include <vostok/os_include.h>
#include <fcntl.h>

// sushi@NOTE: Only exists on old VS builds and makes this file platform dependant
extern "C" FILE* __iob_func(void);

namespace vostok {

namespace logging {
	class filter_tree;
	class log_file;
} // namespace logging

namespace core {


enum stdstream_enum { stdstream_out, stdstream_error };

		void		generate_log_file_name	( fs_new::native_path_string* const out_result, pcstr const extension );
static	_iobuf*		get_stdstream_handle	( stdstream_enum stream );
		void		write_to_stdstream		( stdstream_enum stream, pcstr format, ... );
static	bool		is_logging_initialized	( );
		bool		use_console_for_logging	( );

		void		logging_preinitialize	( ); // sushi@NOTE: Called in other modules (sometimes even in unrelated dependencies like vobris) near allocators. Might be public.

static	void		push_logging_filters	( );
static	bool		initialize_console		( );
static	void		finalize_console		( );


static	void		logging_callback		(
												void*						user_data,
												pcstr						file,
												u32							line,
												pcstr						function_signature,
												pcstr						initiator,
												logging::verbosity			verbosity,
												pcstr						log_string,
												u32							log_string_length,
												logging::callback_flag		flag
											);

		void		debug_log_callback		(
												pcstr		initiator,
												bool		is_error_verbosity,
												bool		log_only_user_string,
												pcstr		message
											);


struct logging_preinitializer
{
	logging_preinitializer	( )
	{
		debug::preinitialize	( ); // Maybe `debug_preinitilizer` can be constructed instead?
		logging_preinitialize	( );
	}
};

// claude@NOTE: the five dynamic initializers below all exist in the base (??__E... symbols,
// see the logging README) - their None scores are the objdiff ??__E demangle-pairing gap.
static logging_preinitializer		s_logging_preinitializer;

static vostok::command_line::key	s_log_verbosity				("log_verbosity",			"", "logging", "one of: [trace|debug|info|warning|error|silent]");
static vostok::command_line::key	s_write_errors_to_stderr	("write_errors_to_stderr",	"", "logging", "");
static vostok::command_line::key	s_use_console				("console",					"", "logging", "turns on console output"   );
static vostok::command_line::key	s_log_to_stdout				("log_to_stdout",			"", "logging", "turns on writing to stdout");

static bool													s_console_initialized			=	false;
static vostok::logging::logging_filters_console_command*	s_logging_console_command		=	NULL;
static bool													s_tried_to_initialize_console	=	false;


logging::filter_tree*		g_log_filter_tree	= NULL;
logging::log_callback_type	g_log_callback		= NULL;
log_flags_enum				g_log_flags			= log_to_console;
logging::log_format			g_log_format;

logging::log_file*				g_log_file			= NULL;
logging::log_file_usage_enum	g_log_file_usage;

// STATE[55.6%|DONE]: paired by adding the QAV/QBD consts (was None purely on the mangled name);
// residual is /Ox LTCG inlining user_data_directory (s_engine vtable call) + the whole
// native_path_string::convert loop into the target body - link-set dependent, banked.
void generate_log_file_name( fs_new::native_path_string* const out_result, pcstr const extension )
{
	ASSERT									(extension);
	ASSERT									(out_result);
	* out_result							= 	fs_new::native_path_string::convert(core::user_data_directory());	// <0x671e66>|0x000|0x000:'51'
	out_result->append_path					(core::application_name());												// <0x671eea>|0x084|0x084:'52'
	fs_new::native_path_string user_name	=	core::user_name();													// <0x671efe>|0x098|0x014:'53'
	if ( user_name.length() )																						// <0x671f85>|0x11f|0x087:'54'
		out_result->appendf					("_%s", user_name.c_str());												// <0x671f9f>|0x139|0x01a:'55'

	out_result->appendf						(".%s", extension);														// <0x671fae>|0x148|0x00f:'57'
}

// STATE[100%|DONE]
static _iobuf* get_stdstream_handle( stdstream_enum stream )
{
	if ( stream == stdstream_out )				// <0x671d70>|0x000|0x000:'68'
		return				&__iob_func()[1];	// <0x671d74>|0x004|0x004:'69'
	else if ( stream == stdstream_error )		// <0x671d7d>|0x00d|0x009:'70'
		return				&__iob_func()[2];	// <0x671d82>|0x012|0x005:'71'

	return					NULL;				// <0x671d8b>|0x01b|0x009:'73'
}

// STATE[100%|DONE]
void write_to_stdstream( stdstream_enum stream, pcstr format, ... )
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

// STATE[100%|DONE]
static bool is_logging_initialized( )
{
	return g_log_filter_tree != NULL;	// <0x671d60>|0x000|0x000:'92'
}

// STATE[98%|DONE]: fixed operand order (key first); residual = key::operator bool called out-of-line
// in base vs target's inlined is_set guard (LTCG; explicit is_set() inlines a WRONG shape, 98 -> 50).
bool use_console_for_logging( )
{
	if ( is_logging_initialized( ) )															// <0x672611>|0x000|0x000:'97'
		return false;																			// <0x67261a>|0x009|0x009:'98'
																								// <1>
	// claude@MATCH: target evaluates the key FIRST (cmp key-state,1 before call run_tests_command_line).
	// Keep the operator-bool form: explicit is_set() inlines a different guard shape and drops 98 -> 50.
	static bool s_use_console_for_logging = s_use_console || testing::run_tests_command_line();	// <0x67261f>|0x00e|0x005:'100'
	return						s_use_console_for_logging;										// <0x672671>|0x060|0x052:'101'
}

// STATE[91%|DONE]: fixed user_data BIT tests + stderr/stdout tail restructure; residual =
// s_log_to_stdout.is_set() called out-of-line in base vs inlined guard in target (LTCG).
static void logging_callback(
	void* const					user_data,			// claude@NOTE: a BITMASK (and 1 / shr 1) - debug_log_callback even passes &log_flags
	pcstr const					file,
	u32 const					line,
	pcstr const					function_signature,
	pcstr const					initiator,
	logging::verbosity const	verbosity,
	pcstr const					log_string,
	u32 const					log_string_length,
	logging::callback_flag const flag				// sushi@TODO: Linker removed this arg.
)
{
	(void)file;	// sushi@NOTE: Unused arguments.
	(void)line;
	(void)function_signature;
	(void)initiator;
	(void)verbosity;

	static bool first_time						= true;
	static bool s_tried_to_initialize_console	= false; // sushi@NOTE: Conflicts with the one defined in the module
	static bool s_initialized_console			= false;

	if ( debug::is_debugger_present( ) )													// <0x672759>|0x000|0x000:'121'
	{
		u32 const buffer_size = (log_string_length + 2);									// <0x672768>|0x00f|0x00f:'128'
		pstr const buffer = static_cast<pstr>( ALLOCA( buffer_size ) );						// <0x67276b>|0x012|0x003:'129'
		memory::copy( buffer, buffer_size, log_string, log_string_length );					// <0x672772>|0x019|0x007:'130'
		buffer[log_string_length] = '\n';
		buffer[log_string_length + 1] = '\0';
		debug::output( buffer );															// <0x67277a>|0x021|0x008:'133'
	}


	if ( g_log_file && g_log_file->initialized( ) ) {										// <0x67278c>|0x033|0x012:'137'
		g_log_file->append( log_string, log_string_length );								// <0x67279f>|0x046|0x013:'138'
		g_log_file->append( "\r\n", 2 );													// <0x6727ac>|0x053|0x00d:'139'
	}


	// claude@MATCH: target tests the BITS (and al,1 / shr,and 1), not equality - log_flags_enum is a mask
	bool log_to_console_settings = ( (intptr_t)user_data & log_to_console ) != 0;			// <0x6727be>|0x065|0x012:'143'
	bool should_use_console_for_logging = use_console_for_logging( );						// <0x6727c8>|0x06f|0x00a:'144'

	if ( first_time && ( log_to_console_settings || should_use_console_for_logging ) )		// <0x6727cd>|0x074|0x005:'146'
		first_time = false;																	// <0x6727e0>|0x087|0x013:'147'

	bool logged_to_stdout = false;
	bool log_to_stderr_settings = ( (intptr_t)user_data & log_to_stderr ) != 0;				// <0x6727e7>|0x08e|0x007:'150'



	if ( log_to_console_settings || should_use_console_for_logging )						// <0x6727ec>|0x093|0x005:'154'
	{


		if ( !s_tried_to_initialize_console )												// <0x6727fa>|0x0a1|0x00e:'158'
		{
			s_initialized_console = initialize_console( );									// <0x672803>|0x0aa|0x009:'160'
			s_tried_to_initialize_console = true;											// <0x67280d>|0x0b4|0x00a:'161'
		}

		if ( s_initialized_console )														// <0x672814>|0x0bb|0x007:'164'
		{
			write_to_stdstream( stdstream_out, "%s\r\n", log_string );						// <0x67281d>|0x0c4|0x009:'166'
			logged_to_stdout = true;														// <0x67282d>|0x0d4|0x010:'167'
		}
	}

	// claude@MATCH: target gates the stderr write on the stderr bit being SET (test bl,bl; je skip),
	// inside an outer g_log_filter_tree check; the stdout block re-checks the tree (redundant, original)
	if ( g_log_filter_tree )																// <0x672831>|0x0d8|0x004:'171'
	{
		if ( log_to_stderr_settings )
			write_to_stdstream( stdstream_error, "%s\r\n", log_string );					// <0x67283e>|0x0e5|0x00d:'173'

		if ( g_log_filter_tree ) {															// <0x672851>|0x0f8|0x013:'176'
			if ( s_log_to_stdout.is_set( ) && !logged_to_stdout )							// <1>
				write_to_stdstream( stdstream_out, "%s\r\n", log_string );					// <0x67288c>|0x133|0x03b:'178'
		}																					// <1>
	}																						// <2>
}

// STATE[52%|PARTIAL]: the % stays low because the target INLINES the boost function1 ctor+dtor
// machinery around each append call while our LTCG keeps the cloned out-of-line ctor - link-set
// residual, banked.
// claude@MATCH: spelled via __LOG_FORCED - the one macro that takes a RUNTIME initiator (the public
// LOG*/LOGI* wrappers only concatenate literals) and expands argument-for-argument to the target's
// append calls: callback temp, (void*)&log_flags, format, __FILE__/__LINE__/__FUNCSIG__, debug_log,
// is_error ? error : info (sete; lea [ecx*2+2]), "%s", message. Line evidence (sushi, PR #286): the
// __LINE__ pushes are 0C4h/0C8h = 196/200, the 0xa4 record at '196' covers the if-test PLUS the whole
// first arm and the jmp-over-else is the lone 5-byte record at '198' - so the first call STARTED on
// the if line and closed on 198, else on 199, second call at 200: each arm spanned <= 3 compact lines.
// sushi@TODO: __LOG_FORCED vs a hand-expanded logging::append is byte-undecidable (textually identical
// expansion); spelled as the macro since it is the one form that fits the 3-line layout naturally.
void debug_log_callback(
	pcstr		initiator,
	bool		is_error_verbosity,
	bool		log_only_user_string,
	pcstr		message
)
{
	// claude@MATCH: not-set arm is 0, not log_to_console(=1<<0): the target's dec;neg;sbb;and 2
	// idiom can only yield {0,2}; a log_to_console arm ({1,2}) would need an extra inc/or after it
	core::log_flags_enum const log_flags = s_write_errors_to_stderr.is_set( ) ?
									core::log_to_stderr : core::log_flags_enum(0);	// <0x672453>|0x003|0x02f:'191'
	pstr debug_log = NULL;
	STR_JOINA( debug_log, initiator, ":" );											// <0x672482>|0x032|0x03a:'193'

	if ( log_only_user_string )	__LOG_FORCED( is_error_verbosity ? logging::error : logging::info,
									logging::format_message, &log_flags,
									debug_log, "%s", message );						// <0x6724bc>|0x06c|0x0a4:'196'..'198'
	else
		__LOG_FORCED( is_error_verbosity ? logging::error : logging::info,
									&g_log_format, &log_flags,
									debug_log, "%s", message );						// <0x672565>|0x115|0x086:'200'

	if ( g_log_file )																// <0x6725eb>|0x19b|0x00a:'203'
		g_log_file->flush( NULL );													// <0x6725f5>|0x1a5|0x006:'204'
}

// STATE[100%|DONE]
void logging_preinitialize( )
{
	if ( !g_log_callback )
	{
		g_log_callback = logging_callback;
		debug::set_log_callback( debug_log_callback );
	}
}

// STATE[36%|PARTIAL]: /Ox target inlines native_path_string::convert("../../user_data/user.cfg") +
// the absolute-path machinery (rows 296-301 ~0x94 bytes bigger) and splits the key-check lines our
// build merges - fs-side + LTCG inlining, banked. Statement order matches.
static void push_logging_filters( )
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

// STATE[85%|DONE]: /Ox line-record splits around generate_log_file_name + LTCG conv; shape matches.
void logging_initialize( )
{
	g_log_filter_tree = logging::new_filter_tree( memory::g_mt_allocator );												// <0x672687>|0x000|0x000:'252'

	s_logging_console_command = VOSTOK_NEW_IMPL( memory::g_mt_allocator, logging::logging_filters_console_command )(	// sushi@NOTE: This opens up into `pt3malloc`
		*g_log_filter_tree,
		"logging_rule",
		true,
		console_commands::command_type_user_specific,
		console_commands::execution_filter_early
	);																													// <0x672699>|0x012|0x012:'260'

	push_logging_filters( );																							// <0x6726d1>|0x04a|0x038:'262': sushi@NOTE: Initialized in `vostok::core::preinitialize`


	if ( g_log_file_usage ) {																							// <0x6726d6>|0x04f|0x005:'265'
		fs_new::device_file_system_proxy	device( NULL, fs_new::watcher_enabled_true );								// <0x6726df>|0x058|0x009:'266': sushi@TODO: `NULL` is `fs_new::s_hdd` in `core\sources\core_entry_point_win_xbox360.cpp`
		fs_new::native_path_string 			log_file_name;																// <0x6726ef>|0x068|0x010:'267'
		generate_log_file_name	( &log_file_name, "log" );																// <0x6726fc>|0x075|0x00d:'268'
		g_log_file = logging::new_log_file( memory::g_mt_allocator, device, log_file_name.c_str( ), g_log_file_usage );	// <0x67271e>|0x097|0x022:'269'
	}
}

// STATE[100%|DONE]: closed by the debug::log_callback raw-pointer fix (set_log_callback(NULL) is a plain push 0).
void logging_finalize( )
{
	finalize_console( );														// <0x671dd0>|0x000|0x000:'275'
	logging::delete_log_file( g_log_file );										// <0x671e03>|0x033|0x033:'276'

	VOSTOK_DELETE_IMPL( memory::g_mt_allocator, s_logging_console_command );	// <0x671e0d>|0x03d|0x00a:'278'

	logging::delete_filter_tree( g_log_filter_tree );							// <0x671e47>|0x077|0x03a:'280'
	debug::set_log_callback( NULL );											// <0x671e51>|0x081|0x00a:'281'
}

// STATE[83%|PARTIAL]: /Ox merges/reorders the per-handle statements (23 vs 30 line records) and sinks
// the LOG_WARNING arm; instruction content matches per-block - optimizer scheduling, banked.
static bool initialize_console( )
{
	if ( s_tried_to_initialize_console )
		return s_console_initialized;

	s_tried_to_initialize_console = true;

	if ( GetConsoleWindow( ) )
	{
		s_console_initialized = true;
		return true;
	}

	if ( AttachConsole( ATTACH_PARENT_PROCESS ) || AllocConsole( ) )
	{
		HANDLE input_handle = GetStdHandle( STD_INPUT_HANDLE );
		int os_input_handle = _open_osfhandle( (intptr_t)input_handle, _O_TEXT );
		if ( os_input_handle != -1 ) {
			FILE input_file = *_fdopen(os_input_handle, "rt");
			__iob_func()[2] = input_file;				// sushi@NOTE: Bug in target? `add eax, 40h`
			setvbuf(&__iob_func()[0], 0, _IONBF, 0);
		}

		HANDLE output_handle = GetStdHandle( STD_OUTPUT_HANDLE );
		int os_output_handle = _open_osfhandle( (intptr_t)output_handle, _O_TEXT );
		FILE output_file = *_fdopen(os_output_handle, "wt");
		__iob_func()[1] = output_file;
		setvbuf(&__iob_func()[1], 0, _IONBF, 0);

		HANDLE error_handle = GetStdHandle( STD_ERROR_HANDLE );
		int os_error_handle = _open_osfhandle( (intptr_t)error_handle, _O_TEXT );
		if ( os_error_handle != -1 ) {
			FILE error_file = *_fdopen(os_error_handle, "wt");
			__iob_func()[2] = error_file;
			setvbuf(&__iob_func()[2], 0, _IONBF, 0);
		}

		std::ios_base::sync_with_stdio( true );

		s_console_initialized = true;
		return true;
	}

	LOG_WARNING( "cannot neither attach parent console, nor create new" );
	s_console_initialized = false;
	return false;
}

// STATE[100%|DONE]
static void finalize_console( )
{
	if ( s_console_initialized )							// <0x671d20>|0x000|0x000:'340'
	{
		CloseHandle( GetStdHandle( STD_INPUT_HANDLE ) );	// <0x671d2a>|0x00a|0x00a:'342'
		CloseHandle( GetStdHandle( STD_OUTPUT_HANDLE ) );	// <0x671d3e>|0x01e|0x014:'343'
		CloseHandle( GetStdHandle( STD_ERROR_HANDLE ) );	// <0x671d45>|0x025|0x007:'344'
		FreeConsole( );										// <0x671d4e>|0x02e|0x009:'345'
	}
}

} // namespace core
} // namespace vostok
