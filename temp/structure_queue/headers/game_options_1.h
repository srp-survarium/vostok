////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#ifndef GAME_OPTIONS_H_INCLUDED
#define GAME_OPTIONS_H_INCLUDED

/* INCLUDES */
class vostok::math::int2;
class survarium::base_game_scene;
class survarium::game;
class survarium::options_tab;
class survarium::vector<enum survarium::game_action_id>;
struct vostok::input::handler;
struct survarium::flash_external_handler;
class vostok::input::enum_gamepad_action;
class vostok::input::enum_keyboard;
class vostok::input::enum_keyboard_action;
class vostok::input::enum_mouse_key_action;
class vostok::input::gamepad_button;
class vostok::input::mouse_button;
class survarium::game_action_id;
typedef vostok::resources::resource_ptr<survarium::flash_movie_resource,vostok::resources::unmanaged_intrusive_base>
	survarium::flash_movie_resource_ptr;
class survarium::flash_movie_resource;

/* FORWARD REFS */
class vostok::input::world;
class vostok::resources::queries_result;
class survarium::flash_movie;
class survarium::flash_value;
class survarium::options_item_base;

namespace survarium {

enum survarium::game_action_id
{
	kLEFT							= 0x00,
	kRIGHT							= 0x01,
	kUP								= 0x02,
	kDOWN							= 0x03,
	kJUMP							= 0x04,
	kCROUCH							= 0x05,
	kACCEL							= 0x06,
	kSPRINT_TOGGLE					= 0x07,
	kSET_NEXT_FIRE_QUEUE_TYPE		= 0x08,
	kSET_NEXT_AMMO_TYPE				= 0x09,
	kFWD							= 0x0a,
	kBACK							= 0x0b,
	kL_STRAFE						= 0x0c,
	kR_STRAFE						= 0x0d,
	kL_LOOKOUT						= 0x0e,
	kR_LOOKOUT						= 0x0f,
	kCAM_1							= 0x10,
	kCAM_2							= 0x11,
	kCAM_3							= 0x12,
	kCAM_4							= 0x13,
	kCAM_ZOOM_IN					= 0x14,
	kCAM_ZOOM_OUT					= 0x15,
	kTORCH							= 0x16,
	kNIGHT_VISION					= 0x17,
	kDETECTOR						= 0x18,
	kWPN_1							= 0x19,
	kWPN_2							= 0x1a,
	kMISSILE_WPN					= 0x1b,
	kARTEFACT						= 0x1c,
	kWPN_NEXT						= 0x1d,
	kWPN_FIRE						= 0x1e,
	kWPN_RELOAD						= 0x1f,
	kWPN_AIM						= 0x20,
	kDELAY_BREATH					= 0x21,
	cam_x_plus						= 0x22,
	cam_x_minus						= 0x23,
	cam_y_plus						= 0x24,
	cam_y_minus						= 0x25,
	cam_z_plus						= 0x26,
	cam_z_minus						= 0x27,
	cam_fov_plus					= 0x28,
	cam_fov_minus					= 0x29,
	cam_dump						= 0x2a,
	kPAUSE							= 0x2b,
	kUSE							= 0x2c,
	kDROP							= 0x2d,
	kSERIALIZE_PLAYER_STATE			= 0x2e,
	kDESERIALIZE_PLAYER_STATE		= 0x2f,
	kQUICK_USE_1					= 0x30,
	kQUICK_USE_2					= 0x31,
	kQUICK_USE_3					= 0x32,
	kQUICK_USE_4					= 0x33,
	kQUICK_USE_5					= 0x34,
	kQUICK_USE_6					= 0x35,
	kBACK_SLOT_USE					= 0x36,
	kCHAT							= 0x37,
	kCHARACTER						= 0x38,
	kINVENTORY						= 0x39,
	kSHOP							= 0x3a,
	kOPTIONS						= 0x3b,
	kFRIENDS						= 0x3c,
	kSEND_MESSAGE					= 0x3d,
	kSELECT_SEND_TO					= 0x3e,
	kPTT							= 0x3f,
	kLASTACTION						= 0x40,
	kNOTBINDED						= 0x41,
	kFORCEDWORD						= -0x01,
};

class game_options : public input::handler , public flash_external_handler , public boost::noncopyable {
public:
			explicit				game_options				( game& g );
	virtual							~game_options				( );

	virtual	input::handler&			input_handler				( ) { /* no source */ }

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

	virtual	s32						input_priority				( ) override { /* no source */ }

	virtual	void					callback					(
										flash_movie*			pmovieView,
										pcstr					methodName,
										flash_value const*		args,
										u32						argCount
									) override;

			void					tick						( const u32 frame_delta, const u32 __formal, const bool __formal );

			void					show_options				( bool b_val );

			void					initialize					( );

			void					initialize_bindings			( );

			void					fill_labels					( );
			void					fill_settings_data			( );
			void					fill_menu_buttons			( bool in_game_world );

			void					activate					( base_game_scene* parent_scene );

			void					deactivate					( );

	inline	bool					is_active					( ) { /* no source */ }

			void					refill_item_data			( u8 options_tab_id, u8 options_item_id );

	inline	options_item_base*		get_options_item			( u8 arg_0, u8 arg_1 ) { /* no source */ }

			void					on_resources_ready			( resources::queries_result& data );

			bool					process_key_input			( s32 dik );

			void					assign_binding				( game_action_id action_id, pcstr key );
			void					finish_binding				( );

			void					apply_default_graphic		( );
			void					apply_key_bindings			( );
			void					reset_bindings				( bool is_default );
			void					reset_bindings_to_defaults	( );

	inline	bool					is_waiting_for_bind_key		( ) { /* no source */ }

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
