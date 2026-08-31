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

// Required by the Visual Studio CRT used for the target build.
extern "C" FILE* __iob_func(void);

namespace vostok {
namespace core {

enum stdstream_enum { stdstream_out, stdstream_error };

		void		logging_preinitialize	( );
static	bool		initialize_console		( );
static	void		finalize_console		( );

struct logging_preinitializer
{
	logging_preinitializer	( )
	{
		debug::preinitialize	( ); // Maybe `debug_preinitilizer` can be constructed instead?
		logging_preinitialize	( );
	}
};

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
log_flags_enum				g_log_flags			= log_to_stdout;
logging::log_format			g_log_format;
logging::log_file*				g_log_file			= NULL;
logging::log_file_usage_enum	g_log_file_usage;

void generate_log_file_name( fs_new::native_path_string* const out_result, pcstr const extension )
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

static _iobuf* get_stdstream_handle( stdstream_enum stream )
{
	if ( stream == stdstream_out )
		return				&__iob_func()[1];
	else if ( stream == stdstream_error )
		return				&__iob_func()[2];

	return					NULL;
}

void write_to_stdstream( stdstream_enum stream, pcstr format, ... )
{
	_iobuf* handle		=	get_stdstream_handle( stream );
	if ( !handle )
		return;
	va_list					mark;
	va_start				( mark, format );

	vfprintf				( handle, format, mark );
	va_end					( mark );
}

static bool is_logging_initialized( )
{
	return g_log_filter_tree != NULL;
}

bool use_console_for_logging( )
{
	if ( !is_logging_initialized( ) )
		return false;
	// Preserve target evaluation order: command-line key before the test flag.
	static bool s_use_console_for_logging = s_use_console || testing::run_tests_command_line();
	return						s_use_console_for_logging;
}

static void logging_callback(
	void* const					user_data,			// log_flags_enum bitmask
	pcstr const					file,
	u32 const					line,
	pcstr const					function_signature,
	pcstr const					initiator,
	logging::verbosity const	verbosity,
	pcstr const					log_string,
	u32 const					log_string_length,
	logging::callback_flag const flag
)
{
	static bool first_time						= true;
	static bool s_tried_to_initialize_console	= false;
	static bool s_initialized_console			= false;

	if ( debug::is_debugger_present( ) )
	{
		u32 const buffer_size = (log_string_length + 2);
		pstr const buffer = static_cast<pstr>( ALLOCA( buffer_size ) );
		memory::copy( buffer, buffer_size, log_string, log_string_length );
		buffer[log_string_length] = '\n';
		buffer[log_string_length + 1] = '\0';
		debug::output( buffer );
	}

	if ( g_log_file && g_log_file->initialized( ) ) {
		g_log_file->append( log_string, log_string_length );
		g_log_file->append( "\r\n", 2 );
	}

	bool log_to_console_settings = ( (intptr_t)user_data & log_to_console ) != 0;
	bool should_use_console_for_logging = use_console_for_logging( );

	if ( first_time && ( log_to_console_settings || should_use_console_for_logging ) )
		first_time = false;

	bool logged_to_stdout = false;
	bool log_to_stderr_settings = ( (intptr_t)user_data & log_to_stderr ) != 0;

	if ( log_to_console_settings || should_use_console_for_logging )
	{
		if ( !s_tried_to_initialize_console )
		{
			s_initialized_console = initialize_console( );
			s_tried_to_initialize_console = true;
		}

		if ( s_initialized_console )
		{
			write_to_stdstream( stdstream_out, "%s\r\n", log_string );
			logged_to_stdout = true;
		}
	}

	if ( g_log_filter_tree && log_to_stderr_settings )
		write_to_stdstream( stdstream_error, "%s\r\n", log_string );

	if ( g_log_filter_tree && s_log_to_stdout && !logged_to_stdout )
		write_to_stdstream( stdstream_out, "%s\r\n", log_string );
}

void debug_log_callback(
	pcstr		initiator,
	bool		is_error_verbosity,
	bool		log_only_user_string,
	pcstr		message
)
{
	core::log_flags_enum const log_flags = s_write_errors_to_stderr.is_set( ) ?
									core::log_to_stderr : core::log_flags_enum(0);
	pstr debug_log = NULL;
	STR_JOINA( debug_log, initiator, ":" );
	if ( log_only_user_string )	__LOG_FORCED( is_error_verbosity ? logging::error : logging::info,
									logging::format_message, &log_flags,
									debug_log, "%s", message );
	else
		__LOG_FORCED( is_error_verbosity ? logging::error : logging::info,
									&g_log_format, &log_flags,
									debug_log, "%s", message );

	if ( g_log_file )
		g_log_file->flush( NULL );
}

void logging_preinitialize( )
{
	if ( !g_log_callback )
	{
		g_log_callback = logging_callback;
		debug::set_log_callback( debug_log_callback );
	}
}

static void push_logging_filters( )
{
	using namespace vostok;
	logging::verbosity	verbosity		=	logging::warning;
	fixed_string512		verbosity_string;
	bool const log_verbosity_key_is_set	=	s_log_verbosity.is_set_as_string(& verbosity_string);
	if ( log_verbosity_key_is_set )
		verbosity						= logging::string_to_verbosity(verbosity_string.c_str());
	else if ( testing::run_tests_command_line() && !vostok::debug::is_debugger_present() )
		verbosity						= logging::warning;
	//	logging::verbosity const verbosity_for_resources	=	log_verbosity_key_is_set ? verbosity : logging::warning;

	logging::push_filter			( *g_log_filter_tree, "", verbosity, u32(-1) );
	//	logging::push_filter		( "core:fs", verbosity_for_resources, & memory::g_mt_allocator );
	//	logging::push_filter		( "core:resources", verbosity_for_resources, & memory::g_mt_allocator );
	//	logging::push_filter		( "core:resources:test", verbosity_for_resources, & memory::g_mt_allocator );
	//	logging::push_filter		( "core:resources:device_manager", verbosity_for_resources, & memory::g_mt_allocator );

	fs_new::native_path_string	cfg_file_path;
	if ( fs_new::convert_to_absolute_path(& cfg_file_path,
										  fs_new::native_path_string::convert("../../user_data/user.cfg"),
										  assert_on_fail_false) )
	{
		console_commands::execute_console_commands	( cfg_file_path, console_commands::execution_filter_early );
	}
}

void logging_initialize( )
{
	g_log_filter_tree = logging::new_filter_tree( memory::g_mt_allocator );

	s_logging_console_command = VOSTOK_NEW_IMPL( memory::g_mt_allocator, logging::logging_filters_console_command )(
		*g_log_filter_tree,
		"logging_rule",
		true,
		console_commands::command_type_user_specific,
		console_commands::execution_filter_early
	);

	push_logging_filters( );

	if ( g_log_file_usage ) {
		fs_new::device_file_system_proxy	device( get_core_device_file_system( ), fs_new::watcher_enabled_true );
		fs_new::native_path_string 			log_file_name;
		generate_log_file_name	( &log_file_name, "log" );
		g_log_file = logging::new_log_file( memory::g_mt_allocator, device, log_file_name.c_str( ), g_log_file_usage );
	}
}

void logging_finalize( )
{
	finalize_console( );
	logging::delete_log_file( g_log_file );

	VOSTOK_DELETE_IMPL( memory::g_mt_allocator, s_logging_console_command );

	logging::delete_filter_tree( g_log_filter_tree );
	debug::set_log_callback( NULL );
}

static bool initialize_console( )
{
	if ( s_tried_to_initialize_console )
		return s_console_initialized;
	s_tried_to_initialize_console = true;

	if ( !GetConsoleWindow( ) )
		if ( !AttachConsole( ATTACH_PARENT_PROCESS ) )
			if ( !AllocConsole( ) ) {
				LOG_WARNING( "cannot neither attach parent console, nor create new" );
				s_console_initialized = false;
				return false;
			}

	int os_input_handle = _open_osfhandle( (intptr_t)GetStdHandle( STD_INPUT_HANDLE ), _O_TEXT );
	if ( os_input_handle != -1 ) {
		__iob_func()[2] = *_fdopen(os_input_handle, "rt");				// Target assigns stdin to the stderr slot.
		setvbuf(&__iob_func()[0], 0, _IONBF, 0);
	}

	int os_output_handle = _open_osfhandle( (intptr_t)GetStdHandle( STD_OUTPUT_HANDLE ), _O_TEXT );
	__iob_func()[1] = *_fdopen(os_output_handle, "wt");
	setvbuf(&__iob_func()[1], 0, _IONBF, 0);

	int os_error_handle = _open_osfhandle( (intptr_t)GetStdHandle( STD_ERROR_HANDLE ), _O_TEXT );
	if ( os_error_handle != -1 ) {
		__iob_func()[2] = *_fdopen(os_error_handle, "wt");
		setvbuf(&__iob_func()[2], 0, _IONBF, 0);
	}

	std::ios_base::sync_with_stdio( true );

	s_console_initialized = true;
	return true;
}

static void finalize_console( )
{
	if ( s_console_initialized )
	{
		CloseHandle( GetStdHandle( STD_INPUT_HANDLE ) );
		CloseHandle( GetStdHandle( STD_OUTPUT_HANDLE ) );
		CloseHandle( GetStdHandle( STD_ERROR_HANDLE ) );
		FreeConsole( );
	}
}

} // namespace core
} // namespace vostok
