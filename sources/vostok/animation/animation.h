////////////////////////////////////////////////////////////////////////////
//	Created		: 27.10.2011
//	Author		: Evgeniy Obertyukh
//	Copyright (C) GSC Game World - 2011
////////////////////////////////////////////////////////////////////////////

#ifndef VOSTOK_ANIMATION_ANIMATION_H_INCLUDED
#define VOSTOK_ANIMATION_ANIMATION_H_INCLUDED

namespace vostok {
namespace animation {

class VOSTOK_ANIMATION_API noncopyable 
{
protected:
	inline						noncopyable	( ) { }
	inline						~noncopyable( ) { }

private:  // emphasize the following members are private
								noncopyable	( const noncopyable& );
	inline	noncopyable const&	operator=	( const noncopyable& );
}; // class noncopyable 

} // namespace animation
} // namespace vostok

#endif // #ifndef VOSTOK_ANIMATION_ANIMATION_H_INCLUDED