////////////////////////////////////////////////////////////////////////////
//	Created 	: 03.11.2025
////////////////////////////////////////////////////////////////////////////

#ifndef LOGGING_EXTENSIONS_H_INCLUDED
#define LOGGING_EXTENSIONS_H_INCLUDED

#include <fastdelegate/fastdelegate.h>
#include <vostok/core/core.h>

namespace vostok {
namespace core {

// PUBLIC

void		logging_initialize		( );
void		logging_finalize		( );

// sushi@TODO: Those all are private? Some of them no need to even export

void		logging_callback		(
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

void		logging_preinitialize	( );




// PRIVATE

void		generate_log_file_name	( fs_new::native_path_string* out_result, pcstr extension );
void		write_to_stdstream		( logging::stdstream_enum stream, pcstr format, ... );
_iobuf*		get_stdstream_handle	( logging::stdstream_enum stream );

bool		initialize_console		( );
void		finalize_console		( );
void		push_logging_filters	( );
bool		use_console_for_logging	( );

bool		is_logging_initialized	( );


} // namespace core
} // namespace vostok

#endif // #ifndef LOGGING_EXTENSIONS_H_INCLUDED