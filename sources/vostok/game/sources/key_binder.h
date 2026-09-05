// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef KEY_BINDER_H_INCLUDED
#define KEY_BINDER_H_INCLUDED

#include "game_action_id.h"
#include "key_binding.h"
#include "keyboard_key_descr.h"
#include "toggle_action_enum.h"

namespace survarium {

class game;

extern float g_mouse_sensitivity;
extern bool g_mouse_invert;

class key_binder : private boost::noncopyable {
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

	// sushi@TODO: Legacy formatting sketch lacks a live output contract; recover consumer, localization and buffer policy.
	inline	void					GetActionAllBinding	( pcstr arg_0, char* arg_1, s32 arg_2 ) { /* no source */ }

	/* 0x0000 */	key_binding		m_key_bindings[64];

	// sushi@TODO: Retail input reads this global; verify the original getter boundaries at the three consumers.
	inline	float					mouse_sensitivity	( ) const { return g_mouse_sensitivity; }

	// sushi@TODO: Retail player input reads this flag; verify the original getter boundaries.
	inline	bool					mouse_invertion		( ) const { return g_mouse_invert; }

	// sushi@TODO: Legacy indexed/fallback model; both retail consumers use fallback, but the original argument spelling is unverified.
			s32						get_action_dik		( game_action_id _action_id, s32 idx );

private:
	// sushi@TODO: Legacy unchecked lookup model; recover original private invocation and valid-name precondition.
	inline	s32						keyname_to_dik		( pcstr name )
	{
		keyboard_key_descr* key = keyname_to_ptr( name );
		return key->dik;
	}

private:
			keyboard_key_descr*		keyname_to_ptr		( pcstr _name );

			game_action_id			action_name_to_id	( pcstr _name );
			game_action_descr*		action_name_to_ptr	( pcstr _name );

			void					remap_keys			( );

	// sushi@TODO: Legacy two-slot model; bind its nonconst private caller, not the const get_binded_action method.
	inline	bool					is_binded			( game_action_id action_id, s32 dik )
	{
		key_binding* binding = &m_key_bindings[action_id];
		if ( binding->m_keyboard[0] && binding->m_keyboard[0]->dik == dik )
			return true;
		if ( binding->m_keyboard[1] && binding->m_keyboard[1]->dik == dik )
			return true;
		return false;
	}

private:
	/* 0x0000 */	/* boost::noncopyable */
	/* 0x0300 */	game&			m_game;
}; // class key_binder

STATIC_SIZE_ASSERT(key_binder, 0x304);

} // namespace survarium

#endif // #ifndef KEY_BINDER_H_INCLUDED
