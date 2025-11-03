////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.08.2008
//	Author		: Sergey Chechin
//	Copyright (C) GSC Game World - 2009
////////////////////////////////////////////////////////////////////////////

#ifndef VOSTOK_LOGGING_HELPER_H_INCLUDED
#define VOSTOK_LOGGING_HELPER_H_INCLUDED

#include <vostok/logging/api.h>
#include <vostok/logging/format.h>

namespace vostok {
namespace logging {

class logger : public boost::noncopyable {
public:
	inline	logger		(
						boost::function<void(void *,pcstr,u32,pcstr,pcstr,enum verbosity,pcstr,u32,enum callback_flag)> const&	arg_0,
						void*								arg_1,
						log_format const*					arg_2,
						pcstr								arg_3,
						u32									arg_4,
						pcstr								arg_5,
						pcstr								arg_6,
						verbosity							arg_7
					) : m_log_callback( arg_0 ) { /* no source */ }

			void	operator()	( pcstr format, char* args );

	inline	~logger		( ) { /* no source */ }


private:
	/* 0x0000 */	/* boost::noncopyable */
	/* 0x0000 */	log_format							m_log_format;
	/* 0x0228 */	boost::function<void(void *,pcstr,u32,pcstr,pcstr,enum verbosity,pcstr,u32,enum callback_flag)> const&	m_log_callback;
	/* 0x022c */	log_format const*					m_log_format_ptr;
	/* 0x0230 */	void*								m_user_data;

	/* 0x0234 */	pcstr								m_initiator;
	/* 0x0238 */	pcstr								m_file;
	/* 0x023c */	pcstr								m_function_signature;
	/* 0x0240 */	u32									m_line;
	/* 0x0244 */	verbosity							m_verbosity;
}; // class logger

// sushi@TODO: STATIC_SIZE_ASSERT(logger, 0x248);

#if 0
class VOSTOK_LOGGING_API helper_data : private core::noncopyable {
	pcstr			m_initiator;
	pcstr			m_file;
	pcstr			m_function_signature;
	int				m_line;
	verbosity		m_verbosity;

					helper_data	(
						pcstr 		file,
						pcstr 		function_signature,
						int   		line,
						pcstr 		major_initiator,
						verbosity	verbosity
					);
}; // struct helper_data

enum	log_flags_enum;

bool VOSTOK_LOGGING_API check_verbosity (pcstr initiator, verbosity verbosity);

class VOSTOK_LOGGING_API logger : private helper_data {
public:
					logger		(
						pcstr 		file,
						pcstr 		function_signature,
						int   		line,
						pcstr 		major_initiator,
						verbosity	verbosity
					);
	void __cdecl	operator( )	( log_flags_enum const log_flags, pcstr format, ... );
	void __cdecl	operator( )	( format_specifier const & log_format, pcstr format, ... );
	void __cdecl	operator( )	( format_specifier const & log_format, int const log_flags, pcstr format, ... );
	void __cdecl	operator( )	( log_format *  format_list, int const log_flags, pcstr format, ... );
	void __cdecl	operator( )	( pcstr format, ... );
}; // class helper
#endif


} // namespace logging
} // namespace vostok

#endif // #ifndef VOSTOK_LOGGING_HELPER_H_INCLUDED