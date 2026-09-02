// SPDX-License-Identifier: GPL-3.0-or-later
////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////
#ifndef KEY_BINDER_H_INCLUDED
#define KEY_BINDER_H_INCLUDED

#include "game_action_id.h"
#include "key_binding.h"
#include "toggle_action_enum.h"

namespace survarium {

class game;
class console_command_bind;

class key_binder : private boost::noncopyable {
	friend class console_command_bind;
public:
			explicit				key_binder			( game& g );

			void					bind_key			( pcstr args, s32 bind_number );
			void					unbind_key			( pcstr args, s32 bind_number );

			void					set_default_controls( );

			keyboard_key_descr*		dik_to_ptr			( s32 _dik, bool bSafe );

			game_action_id			get_binded_action	( s32 _dik, toggle_action_enum& actions_mask_type, s32 key_group_mask ) const;

			pcstr					id_to_action_name	( game_action_id _id ) const;

			pcstr					dik_to_keyname		( s32 _dik );

			s32						get_binding_group	( game_action_id _id );

	inline	void					GetActionAllBinding	( pcstr arg_0, char* arg_1, s32 arg_2 ) { /* no source */ }

	/* 0x0000 */	key_binding		m_key_bindings[64];

	inline	float					mouse_sensitivity	( ) const { /* no source */ return 0.0f; }

	inline	bool					mouse_invertion		( ) const { /* no source */ return false; }

			s32						get_action_dik		( game_action_id _action_id, s32 idx );

private:
	inline	s32						keyname_to_dik		( pcstr arg_0 ) { /* no source */ return 0; }

private:
			keyboard_key_descr*		keyname_to_ptr		( pcstr _name );

			game_action_id			action_name_to_id	( pcstr _name );
			game_action_descr*		action_name_to_ptr	( pcstr _name );

			void					remap_keys			( );

	inline	bool					is_binded			( game_action_id arg_0, s32 arg_1 ) { /* no source */ return false; }

private:
	/* 0x0000 */	/* boost::noncopyable */
	/* 0x0300 */	game&			m_game;
}; // class key_binder

STATIC_SIZE_ASSERT(key_binder, 0x304);

} // namespace survarium

#endif // #ifndef KEY_BINDER_H_INCLUDED
