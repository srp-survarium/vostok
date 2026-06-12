////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#ifndef KEY_BIND_DESCR_H_INCLUDED
#define KEY_BIND_DESCR_H_INCLUDED

/* INCLUDES */
class vostok::fixed_string<32>;

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
enum survarium::keybind_group_type
{
	lobby_type		= 0x0,
	chat_type		= 0x1,
	game_type		= 0x2,
};
enum survarium::action_type
{
	locked_action		= 0x0,
	unlocked_action		= 0x1,
};

struct key_bind_descr {
	inline		key_bind_descr	( ) { /* no source */ }

public:
	/* 0x0000 */	game_action_id			action_id;
	/* 0x0004 */	pcstr					str_description;
	/* 0x0008 */	keybind_group_type		group_id;
	/* 0x000c */	action_type				type;
	/* 0x0010 */	fixed_string< 32 >		new_binded_key;
	/* 0x003c */	fixed_string< 32 >		old_binded_key;
}; // struct key_bind_descr

STATIC_SIZE_ASSERT(key_bind_descr, 0x68);

} // namespace survarium

#endif // #ifndef KEY_BIND_DESCR_H_INCLUDED
