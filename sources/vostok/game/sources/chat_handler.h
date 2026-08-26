////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#ifndef CHAT_HANDLER_H_INCLUDED
#define CHAT_HANDLER_H_INCLUDED

#include <vostok/input/handler.h>
#include <vostok/resources_unmanaged_resource.h>

#include <vostok/scaleform/sources/flash_external_handler.h>
#include <vostok/scaleform/sources/flash_function_handler.h>
#include <vostok/scaleform/sources/flash_movie_resource.h>
#include "messaging_enums.h"

namespace survarium {

class base_game_scene;
class game;
struct flash_function_handler_params;
struct flash_movie;
struct flash_value;

class chat_handler : public input::handler , public flash_function_handler , public flash_external_handler , private boost::noncopyable {
public:
			explicit						chat_handler			( game& game );
	virtual									~chat_handler			( );

	virtual	bool							on_keyboard_action		(
												input::world*					input_world,
												input::enum_keyboard			key,
												input::enum_keyboard_action		action
											) override;
	virtual	bool							on_gamepad_action		(
												input::world*					input_world,
												input::gamepad_button			button,
												input::enum_gamepad_action		action
											) override;
	virtual	bool							on_mouse_key_action		(
												input::world*					input_world,
												input::mouse_button				button,
												input::enum_mouse_key_action	action
											) override;
	virtual	bool							on_mouse_move			(
												input::world*		input_world,
												s32					x,
												s32					y,
												s32					z
											) override;

	virtual	s32								input_priority			( ) override
	{
		return 5;
	}

	virtual	void							callback				(
												flash_movie*			__formal,
												pcstr					methodName,
												flash_value const*		args,
												u32						__formal2 /* PDB: __formal too (two would collide) */
											) override;

	virtual	void							call					( flash_function_handler_params& params ) override;

			void							focus					( bool b_focused );

			void							show					( base_game_scene* scene );

			void							hide					( base_game_scene* scene );

			void							tick					( u32 delta );

	inline	bool							is_active				( ) { /* no source */ return m_active; }

			void							set_mode				( bool is_game_mode );

			void							on_message_typed		( wchar_t const* text, messaging::message_channel_enum message_chanel );

			void							add_message				(
												const messaging::message_channel_enum	arg_0 /* messaging::message_channel_enum channel */,
												wchar_t const*		w_text,
												wchar_t const*		w_sender_name
											);
			void							add_to_recent_list		( wchar_t const* name );

	inline	bool							in_match				( ) const { /* no source */ return false; }

			void							initialize				( resources::unmanaged_resource_ptr const& ui );

	inline	flash_movie_resource_ptr&		get_movie				( ) { /* no source */ return m_chat_ui; }

			void							set_local_player_name	( pcstr account_name );

private:
	/* 0x0000 */	/* input::handler */
	/* 0x0004 */	/* flash_function_handler */
	/* 0x000c */	/* flash_external_handler */
	/* 0x0014 */	/* boost::noncopyable */
	/* 0x0014 */	bool						m_focused;
	/* 0x0015 */	bool						m_active;
	/* 0x0016 */	bool						m_game_ui_mode;
	/* 0x0018 */	game&						m_game;
	/* 0x001c */	flash_movie_resource_ptr	m_chat_ui;
}; // class chat_handler

STATIC_SIZE_ASSERT(chat_handler, 0x20);

} // namespace survarium

#endif // #ifndef CHAT_HANDLER_H_INCLUDED
