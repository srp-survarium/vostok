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
						boost::function<log_callback_type> const&			log_callback,
						void*								user_data,
						log_format const*					log_format_ptr,
						pcstr								initiator, // sushi@TODO: Order of pcstr might be different
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
					m_verbosity			( verbosity ) {}

			void	operator()	( pcstr format, char* args );

	inline			~logger		( ) {}


private:
	/* 0x0000 */	log_format							m_log_format;
	/* 0x0228 */	boost::function<log_callback_type> const&		m_log_callback;
	/* 0x022c */	log_format const*					m_log_format_ptr;
	/* 0x0230 */	void*								m_user_data;

	/* 0x0234 */	pcstr								m_initiator;
	/* 0x0238 */	pcstr								m_file;
	/* 0x023c */	pcstr								m_function_signature;
	/* 0x0240 */	u32									m_line;
	/* 0x0244 */	verbosity							m_verbosity;
}; // class logger

STATIC_SIZE_ASSERT(logger, 0x248);

} // namespace logging
} // namespace vostok

#endif // #ifndef VOSTOK_LOGGING_HELPER_H_INCLUDED