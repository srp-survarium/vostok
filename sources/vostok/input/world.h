////////////////////////////////////////////////////////////////////////////
//	Created 	: 20.11.2008
//	Author		: Dmitriy Iassenev
//	Copyright (C) GSC Game World - 2009
////////////////////////////////////////////////////////////////////////////

#ifndef VOSTOK_INPUT_WORLD_H_INCLUDED
#define VOSTOK_INPUT_WORLD_H_INCLUDED

namespace vostok {
namespace input {

struct handler;
struct keyboard;
struct mouse;
struct gamepad;

struct VOSTOK_NOVTABLE world {
public:
	virtual	void		tick			( ) = 0;
	virtual	void		clear_resources	( ) = 0;
	virtual	void		on_activate		( ) = 0;
	virtual	void		on_deactivate	( ) = 0;
	virtual	void		add_handler		( handler& handler ) = 0;
	virtual	void		remove_handler	( handler& handler ) = 0;
	virtual	void		acquire			( ) = 0;
	virtual	void		unacquire		( ) = 0;

public:
	typedef raw<gamepad>::ptr			gamepad_ptr_type;
	typedef raw<keyboard const>::ptr	keyboard_ptr_type;
	typedef raw<mouse const>::ptr		mouse_ptr_type;

public:
	virtual gamepad_ptr_type	get_gamepad	( ) = 0;
	virtual keyboard_ptr_type	get_keyboard( ) = 0;
	virtual mouse_ptr_type		get_mouse	( ) = 0;

protected:
	VOSTOK_DECLARE_PURE_VIRTUAL_DESTRUCTOR( world )
}; // class world

} // namespace input
} // namespace vostok

#endif // #ifndef VOSTOK_INPUT_WORLD_H_INCLUDED