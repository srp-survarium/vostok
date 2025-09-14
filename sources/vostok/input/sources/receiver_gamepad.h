////////////////////////////////////////////////////////////////////////////
//	Created 	: 21.11.2008
//	Author		: Dmitriy Iassenev
//	Copyright (C) GSC Game World - 2009
////////////////////////////////////////////////////////////////////////////

#ifndef RECEIVER_CONTROLLER_H_INCLUDED
#define RECEIVER_CONTROLLER_H_INCLUDED

#include <vostok/input/world.h>
#include <vostok/input/gamepad.h>
#include <vostok/os_include.h>

#if VOSTOK_PLATFORM_WINDOWS
#	include <xinput.h>
#endif // #if VOSTOK_PLATFORM_WINDOWS

namespace vostok {
namespace input {

struct handler;

namespace receiver {

class gamepad :
	public input::gamepad,
	private boost::noncopyable
{
	typedef vector< handler* >				handlers_type;

public:
							gamepad			( u32 user_index, vostok::input::world& input_world );
	virtual					~gamepad		( );
	virtual	state const&	get_state		( ) const;
	virtual	float			get_vibration	( gamepad_vibrators vibrator ) const;
	virtual	void			set_vibration	( gamepad_vibrators vibrator, float value );
			void			on_activate		( );
			void			on_deactivate	( );
			void			execute			( );
			void			process			( handlers_type& handlers );

private:
	vostok::input::world&		m_world;
#ifdef _MSC_VER
	XINPUT_CAPABILITIES		m_device_capabilities;
	XINPUT_VIBRATION		m_current_vibration;
#endif // #ifdef _MSC_VER
	state					m_current_state;
	state					m_previous_state;
	u32						m_user_index;
	bool					m_connected;
	bool					m_inserted;
	bool					m_removed;
}; // class world

} // namespace receiver
} // namespace input
} // namespace vostok

#endif // #ifndef RECEIVER_CONTROLLER_H_INCLUDED