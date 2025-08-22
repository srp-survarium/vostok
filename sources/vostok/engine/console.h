////////////////////////////////////////////////////////////////////////////
//	Created		: 09.11.2010
//	Author		: Andrew Kolomiets
//	Copyright (C) GSC Game World - 2010
////////////////////////////////////////////////////////////////////////////

#ifndef VOSTOK_ENGINE_CONSOLE_H_INCLUDED
#define VOSTOK_ENGINE_CONSOLE_H_INCLUDED

#include <vostok/input/handler.h>

namespace vostok {
namespace engine {

class console : public vostok::input::handler
{
public:
	virtual					~console			( )			{};
	virtual bool			get_active			( ) const	= 0;
	virtual void			on_activate			( )			= 0;
	virtual void			on_deactivate		( )			= 0;
	virtual void			tick				( )			= 0;
}; // class console

} // namespace engine
} // namespace vostok

#endif // #ifndef VOSTOK_ENGINE_CONSOLE_H_INCLUDED