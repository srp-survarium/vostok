////////////////////////////////////////////////////////////////////////////
//	Created 	: 31.10.2008
//	Author		: Dmitriy Iassenev
//	Copyright (C) GSC Game World - 2009
////////////////////////////////////////////////////////////////////////////

#ifndef VOSTOK_LOGGING_NONCOPYABLE_H_INCLUDED
#define VOSTOK_LOGGING_NONCOPYABLE_H_INCLUDED

namespace vostok {
namespace logging {

class VOSTOK_LOGGING_API noncopyable {
	protected:
		inline						noncopyable	( ) { }
		inline						~noncopyable( ) { }

	private:  // emphasize the following members are private
									noncopyable	( const noncopyable& );
		inline	noncopyable const&	operator=	( const noncopyable& );
};

} // namespace logging
} // namespace vostok

#endif // #ifndef VOSTOK_LOGGING_NONCOPYABLE_H_INCLUDED