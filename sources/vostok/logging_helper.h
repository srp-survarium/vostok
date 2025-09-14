////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.08.2008
//	Author		: Sergey Chechin
//	Copyright (C) GSC Game World - 2009
////////////////////////////////////////////////////////////////////////////

#ifndef VOSTOK_LOGGING_HELPER_H_INCLUDED
#define VOSTOK_LOGGING_HELPER_H_INCLUDED

#include <vostok/logging_extensions.h>

namespace vostok {
namespace logging {

class settings;

struct VOSTOK_CORE_API helper_data : private detail::noncopyable {
	pcstr			m_initiator;
	pcstr			m_file;
	pcstr			m_function_signature;
	int				m_line;
	verbosity	m_verbosity;

					helper_data	(
						pcstr 		file,
						pcstr 		function_signature,
						int   		line,
						pcstr 		major_initiator,
						verbosity	verbosity
					);
}; // struct helper_data

class VOSTOK_CORE_API helper : private helper_data {
public:
					helper		(
						pcstr 		file,
						pcstr 		function_signature,
						int   		line,
						pcstr 		major_initiator,
						verbosity	verbosity
					);
	void __cdecl	operator( )	( settings const& log_settings, pcstr format, ... );
	void __cdecl	operator( )	( pcstr format, ... );
}; // class helper

} // namespace logging
} // namespace vostok

#endif // #ifndef VOSTOK_LOGGING_HELPER_H_INCLUDED