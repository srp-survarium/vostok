// SPDX-License-Identifier: GPL-3.0-or-later
////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////
#ifndef GAME_OPTIONS_H_INCLUDED
#define GAME_OPTIONS_H_INCLUDED

#include <vostok/input/handler.h>
#include <vostok/math_int2.h>

#include <vostok/scaleform/sources/flash_external_handler.h>
#include <vostok/scaleform/sources/flash_movie_resource.h>
#include "game_action_id.h"	// the canonical dump inlines the enum here; it lives in its own header

namespace vostok {
namespace resources {
	class queries_result;
} // namespace resources
} // namespace vostok

namespace survarium {

class base_game_scene;
class game;
class options_item_base;
class options_tab;

// the canonical game_options_1.h variant is byte-identical - no union needed
class game_options : public input::handler , public flash_external_handler , private boost::noncopyable {
public:
			explicit				game_options				( game& g );
	virtual							~game_options				( );

	// game_options' own virtual (not in the input::handler interface)
	virtual	input::handler&			input_handler				( )
	{
		return *this;
	}

	virtual	bool					on_keyboard_action			(
										input::world*					input_world,
										input::enum_keyboard			key,
										input::enum_keyboard_action		action
									) override;
	virtual	bool					on_gamepad_action			(
										input::world*					input_world,
										input::gamepad_button			button,
										input::enum_gamepad_action		action
									) override;
	virtual	bool					on_mouse_key_action			(
										input::world*					input_world,
										input::mouse_button				button,
										input::enum_mouse_key_action	action
									) override;
	virtual	bool					on_mouse_move				(
										input::world*		input_world,
										s32					x,
										s32					y,
										s32					z
									) override;

	virtual	s32						input_priority				( ) override { /* no source */ return 0; }

	virtual	void					callback					(
										flash_movie*			pmovieView,
										pcstr					methodName,
										flash_value const*		args,
										u32						argCount
									) override;

			void					tick						( const u32 frame_delta, const u32, const bool );	// PDB: last two params __formal, genuinely unused

			void					show_options				( bool b_val );

			void					initialize					( );

			void					initialize_bindings			( );

			void					fill_labels					( );
			void					fill_settings_data			( );
			void					fill_menu_buttons			( bool in_game_world );

			void					activate					( base_game_scene* parent_scene );

			void					deactivate					( );

	inline	bool					is_active					( ) { /* no source */ return m_is_active; }

			void					refill_item_data			( u8 options_tab_id, u8 options_item_id );

	inline	options_item_base*		get_options_item			( u8 arg_0, u8 arg_1 ) { /* no source */ return NULL; }

private:
			void					on_resources_ready			( resources::queries_result& data );

			bool					process_key_input			( s32 dik );

			void					assign_binding				( game_action_id action_id, pcstr key );
			void					finish_binding				( );

			void					apply_default_graphic		( );
			void					apply_key_bindings			( );
			void					reset_bindings				( bool is_default );
			void					reset_bindings_to_defaults	( );

	inline	bool					is_waiting_for_bind_key		( ) { /* no source */ return false; }

private:
	/* 0x0000 */	/* input::handler */
	/* 0x0004 */	/* flash_external_handler */
	/* 0x000c */	/* boost::noncopyable */
	/* 0x000c */	base_game_scene*					m_parent_scene;
	/* 0x0010 */	flash_movie_resource_ptr			m_options_ui;
	/* 0x0014 */	flash_movie_resource_ptr			m_cursor_ui;
	/* 0x0018 */	options_tab*						m_options[4];
	/* 0x0028 */	game&								m_game;
	/* 0x002c */	math::int2							m_mouse_pos;
	/* 0x0034 */	bool								m_is_active;
	/* 0x0038 */	game_action_id						m_waiting_for_bind_action;
	/* 0x003c */	pcstr								m_conflicted_key_name;
	/* 0x0040 */	game_action_id						m_conflicted_action_to_bind;
	/* 0x0044 */	vector< enum game_action_id >		m_conflicted_action_ids;
}; // class game_options

STATIC_SIZE_ASSERT(game_options, 0x50);

} // namespace survarium

#endif // #ifndef GAME_OPTIONS_H_INCLUDED
