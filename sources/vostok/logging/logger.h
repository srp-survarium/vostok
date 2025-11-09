////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.08.2008
//	Author		: Sergey Chechin
//	Copyright (C) GSC Game World - 2009
////////////////////////////////////////////////////////////////////////////

#ifndef VOSTOK_LOGGING_HELPER_H_INCLUDED
#define VOSTOK_LOGGING_HELPER_H_INCLUDED

#include <vostok/logging/api.h>
#include <vostok/logging/format.h>
#include <vostok/logging/extensions.h>

namespace vostok {
namespace logging {

class logger : public boost::noncopyable {
public:
	inline			logger		(
						log_callback_boost const&			log_callback,
						void*								user_data,
						log_format const*					log_format_ptr,
						pcstr								initiator,
						u32									line,
						pcstr								file,
						pcstr								function_signature,
						verbosity							verbosity
					) :
					m_log_callback		( log_callback ),
					m_log_format_ptr	( log_format_ptr ),
					m_user_data			( user_data ),

					m_initiator			( initiator ),
					m_file				( file ),
					m_function_signature( function_signature ),
					m_line				( line ),
					m_verbosity			( verbosity )
					{
						if ( m_log_format_ptr )
							m_log_format = *m_log_format_ptr;
						else
							m_log_format.set( format_message );

					}

			void	operator()	( pcstr const format, pstr const args );

	inline			~logger		( ) {}


public:
	/* 0x0000 */	log_format							m_log_format;
	/* 0x0228 */	log_callback_boost const&			m_log_callback;
	/* 0x022c */	log_format const*					m_log_format_ptr;
	/* 0x0230 */	void*								m_user_data;

	/* 0x0234 */	pcstr								m_initiator;
	/* 0x0238 */	pcstr								m_file;
	/* 0x023c */	pcstr								m_function_signature;
	/* 0x0240 */	u32									m_line;
	/* 0x0244 */	verbosity							m_verbosity;
}; // class logger

STATIC_SIZE_ASSERT(logger, 0x248);

void VOSTOK_LOGGING_API append(
	log_callback_boost const&	log_callback,
	void* const					user_data,
	log_format const*			log_format,
	pcstr						file,
	u32							line,
	pcstr						function_signature,
	pcstr						initiator,
	verbosity					verbosity,
	pcstr						format,
	...
);

void VOSTOK_LOGGING_API append(
	log_callback_boost const&	log_callback,
	void* const					user_data,
	format_specifier const&		format_specifier,
	pcstr						file,
	u32							line,
	pcstr						function_signature,
	pcstr						initiator,
	verbosity					verbosity,
	pcstr						format,
	...
);

} // namespace logging
} // namespace vostok

#endif // #ifndef VOSTOK_LOGGING_HELPER_H_INCLUDED