////////////////////////////////////////////////////////////////////////////
//	Created 	: 21.11.2008
//	Author		: Dmitriy Iassenev
//	Copyright (C) GSC Game World - 2009
////////////////////////////////////////////////////////////////////////////

#ifndef RECEIVER_KEYBOARD_H_INCLUDED
#define RECEIVER_KEYBOARD_H_INCLUDED

#include "direct_input_include.h"
#include <vostok/input/world.h>
#include <vostok/input/keyboard.h>

struct IDirectInput8A;
struct IDirectInputDevice8A;

namespace vostok {
namespace input {

struct handler;

namespace receiver {

class keyboard :
	public input::keyboard,
	private boost::noncopyable
{
typedef vector< handler* >		handlers_type;
public:
					keyboard			( IDirectInput8A& direct_input, HWND window_handle, vostok::input::world& input_world );
	virtual			~keyboard			( );
	virtual	bool	is_key_down			( enum_keyboard key ) const;
private:
			bool	is_key_down			( char value ) const;
public:
	virtual	bool	get_dik_name		( int dik, pstr dest_str, int dest_sz ) const;
	virtual	bool	get_dik_unicode		( int dik, wchar_t* buff, u32 buff_size ) const;
			void	on_activate			( );
			void	on_deactivate		( );
			void	execute				( );
			void	process				( handlers_type& handlers );
private:
			void	reset_current_state	( );

private:
	s32						m_current_key_state	[256];
	DWORD					m_current_events_count;
	DIDEVICEOBJECTDATA		m_current_events	[64];
	HWND					m_window_handle;
	IDirectInputDevice8A*	m_device;
	vostok::input::world&	m_world;
}; // class keyboard

STATIC_SIZE_ASSERT(keyboard, 0x914);

} // namespace receiver
} // namespace input
} // namespace vostok

#endif // #ifndef RECEIVER_KEYBOARD_H_INCLUDED
