////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "lobby_menu.h"
#include "lobby_menu_external_handler.h"
#include <vostok/scaleform/sources/flash_function_handler.h>

namespace survarium {

// TU-local (canonical headers/relocate_item_func.h; owner mapping in
// temp/triage_log.md) - lobby_menu holds it through m_relocate_item_func
class relocate_item_func : public flash_function_handler , public boost::noncopyable {
public:
			explicit	relocate_item_func	( game& g );

	virtual	void		call				( flash_function_handler_params& params ) override;

	inline				~relocate_item_func	( ) { /* no source */ }

private:
	/* 0x0000 */	/* flash_function_handler */
	/* 0x0008 */	/* boost::noncopyable */
	/* 0x0008 */	game&		m_game;
}; // class relocate_item_func

STATIC_SIZE_ASSERT(relocate_item_func, 0xC);

// STATE[STUB]
 relocate_item_func::relocate_item_func( game& g )
{
	// FUNCTION BODY[0x93370]
	// <0x93370>|0x000|      :'31'	{
	// ******
}

// STATE[STUB]
void relocate_item_func::call( flash_function_handler_params& params )
{
	// LOCALS
	// vector< relocate_item_descr > 	descriptions
	// lobby_client& 					lobby
	// u32 								second_item_id
	// flash_value 						descr_member_value
	// flash_value 						descr_value
	// dictionary_item 					current_item
	// relocate_item_descr 				current
	// ******

	// CALL SITE INFO
	// <0x934c9> -> lobby_client& < unknown >()
	// ******

	// FUNCTION BODY[0x933d0]: 84
	// <0x933dc>|0x00c|+0x0d1:'35'
	// <0>
	// <1>
	// <0x934ad>|0x0dd|+0x01e:'38'
	// <0>
	// <1>
	// <2>
	// <0x934cb>|0x0fb|+0x017:'42'
	// <0>
	// <0x934e2>|0x112|+0x4c8:'44'
	// <0x939aa>|0x5da|-0x4b5:'44'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <0x934f5>|0x125|+0x02d:'51'
	// <0>
	// <0x93522>|0x152|+0x02e:'53'
	// <0>
	// <1>
	// <0x93550>|0x180|+0x034:'56'
	// <0>
	// <1>
	// <0x93584>|0x1b4|+0x036:'59'
	// <0>
	// <1>
	// <0x935ba>|0x1ea|+0x034:'62'
	// <0x935ee>|0x21e|+0x004:'63'
	// <0>
	// <0x935f2>|0x222|+0x030:'65'
	// <0x93622>|0x252|+0x004:'66'
	// <0>
	// <0x93626>|0x256|+0x030:'68'
	// <0>
	// <1>
	// <0x93656>|0x286|+0x057:'71'
	// <0>
	// <1>
	// <0x936ad>|0x2dd|+0x0f3:'74'
	// <0>
	// <1>
	// <0x937a0>|0x3d0|+0x029:'77'
	// <0>
	// <1>
	// <2>
	// <0x937c9>|0x3f9|+0x01b:'81'
	// <0>
	// <0x937e4>|0x414|-0x011:'83'
	// <0>
	// <0x937d3>|0x403|+0x00a:'85'
	// <0>
	// <0x937dd>|0x40d|+0x00c:'87'
	// <0>
	// <1>
	// <0x937e9>|0x419|+0x025:'90'
	// <0x9380e>|0x43e|-0x013:'91'
	// <0x937fb>|0x42b|+0x02b:'92'
	// <0x93826>|0x456|-0x015:'92'
	// <0>
	// <0x93811>|0x441|+0x005:'94'
	// <0x93816>|0x446|+0x016:'95'
	// <0>
	// <1>
	// <2>
	// <0x9382c>|0x45c|+0x092:'99'
	// <0x938be>|0x4ee|-0x00c:'99'
	// <0x938b2>|0x4e2|+0x010:'100'
	// <0x938c2>|0x4f2|+0x010:'100'
	// <0>
	// <1>
	// <0x938d2>|0x502|+0x008:'103'
	// <0>
	// <1>
	// <0x938da>|0x50a|+0x03f:'106'
	// <0>
	// <0x93919>|0x549|+0x095:'108'
	// <0>
	// <0x939ae>|0x5de|+0x00f:'110'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <7>
	// ******
}

// STATE[STUB]
void lobby_menu_external_handler::callback(
	flash_movie*			pmovieView,
	pcstr					methodName,
	flash_value const*		args,
	u32						argCount
)
{
	// LOCALS
	// const u32 						items_count
	// u8 								faction_id
	// flash_value 						skills_array
	// flash_value 						perks_array
	// vectora< u8 > 					perks
	// vectora< player_skill > 			skills
	// u8 								i
	// player_skill 					current_skill
	// flash_value 						branch_value
	// flash_value 						branch_member_value
	// u8 								i
	// flash_value 						perk_value
	// u8 								perk
	// flash_value[2] 					ret_args
	// ******

	// CONSTANTS
	// const lobby_menu_external_handler::callback::__l60::< unnamed-tag > c_min_name_len_to_search = 3;
	// ******

	// CALL SITE INFO
	// <0x93afb> -> lobby_client& < unknown >()
	// <0x93b4b> -> lobby_client& < unknown >()
	// <0x93b76> -> lobby_client& < unknown >()
	// <0x93b88> -> lobby_client& < unknown >()
	// <0x93c92> -> lobby_client& < unknown >()
	// <0x93cf6> -> lobby_client& < unknown >()
	// <0x93f4a> -> lobby_client& < unknown >()
	// <0x93fee> -> lobby_client& < unknown >()
	// <0x94040> -> messaging_client& < unknown >()
	// <0x94076> -> messaging_client& < unknown >()
	// <0x940ad> -> messaging_client& < unknown >()
	// <0x940e4> -> messaging_client& < unknown >()
	// <0x9411b> -> messaging_client& < unknown >()
	// ******

	// FUNCTION BODY[0x93aa0]: 134
	// <0x93aa0>|0x000|+0x00e:'148'	{
	// <0>
	// <0x93aae>|0x00e|+0x03f:'150'
	// <0>
	// <0x93aed>|0x04d|+0x020:'152'
	// <0>
	// <0x93b0d>|0x06d|+0x030:'154'
	// <0>
	// <0x93b3d>|0x09d|+0x01d:'156'
	// <0>
	// <0x93b5a>|0x0ba|+0x01e:'158'
	// <0x93b78>|0x0d8|+0x032:'159'
	// <0>
	// <1>
	// <0x93baa>|0x10a|+0x030:'162'
	// <0>
	// <1>
	// <0x93bda>|0x13a|+0x020:'165'
	// <0>
	// <0x93bfa>|0x15a|+0x010:'167'
	// <0>
	// <0x93c0a>|0x16a|+0x018:'169'
	// <0>
	// <0x93c22>|0x182|+0x010:'171'
	// <0>
	// <0x93c32>|0x192|+0x01d:'173'
	// <0>
	// <0x93c4f>|0x1af|+0x014:'175'
	// <0>
	// <0x93c63>|0x1c3|+0x003:'177'
	// <0x93c66>|0x1c6|+0x008:'178'
	// <0x93c6e>|0x1ce|+0x00b:'179'
	// <0>
	// <0x93c79>|0x1d9|+0x004:'181'
	// <0x93c7d>|0x1dd|+0x055:'182'
	// <0x93cd2>|0x232|-0x052:'182'
	// <0x93c80>|0x1e0|+0x017:'183'
	// <0x93c97>|0x1f7|+0x01a:'184'
	// <0>
	// <0x93cb1>|0x211|+0x019:'186'
	// <0x93cca>|0x22a|-0x002:'187'
	// <0>
	// <1>
	// <2>
	// <0x93cc8>|0x228|+0x006:'191'
	// <0x93cce>|0x22e|+0x00b:'191'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <0x93cd9>|0x239|+0x02e:'198'
	// <0>
	// <0x93d07>|0x267|+0x014:'200'
	// <0>
	// <0x93d1b>|0x27b|+0x007:'202'
	// <0>
	// <1>
	// <0x93d22>|0x282|+0x03d:'205'
	// <0>
	// <0x93d5f>|0x2bf|+0x037:'207'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <0x93d96>|0x2f6|+0x025:'214'
	// <0>
	// <0x93dbb>|0x31b|+0x013:'216'
	// <0x93dce>|0x32e|+0x007:'217'
	// <0>
	// <0x93dd5>|0x335|+0x013:'219'
	// <0>
	// <1>
	// <2>
	// <0x93de8>|0x348|+0x00f:'223'
	// <0>
	// <0x93df7>|0x357|+0x02a:'225'
	// <0x93e21>|0x381|+0x039:'226'
	// <0>
	// <1>
	// <0x93e5a>|0x3ba|+0x039:'229'
	// <0>
	// <1>
	// <0x93e93>|0x3f3|+0x021:'232'
	// <0x93eb4>|0x414|+0x00b:'233'
	// <0x93ebf>|0x41f|+0x023:'234'
	// <0x93ee2>|0x442|+0x051:'235'
	// <0>
	// <0x93f33>|0x493|+0x023:'237'
	// <0x93f56>|0x4b6|+0x07a:'238'
	// <0x93fd0>|0x530|+0x010:'239'
	// <0>
	// <0x93fe0>|0x540|+0x01f:'241'
	// <0>
	// <0x93fff>|0x55f|+0x010:'243'
	// <0>
	// <0x9400f>|0x56f|+0x00a:'245'
	// <0>
	// <0x94019>|0x579|+0x019:'247'
	// <0x94032>|0x592|+0x01f:'248'
	// <0>
	// <0x94051>|0x5b1|+0x010:'250'
	// <0>
	// <0x94061>|0x5c1|+0x006:'252'
	// <0x94067>|0x5c7|+0x021:'253'
	// <0>
	// <0x94088>|0x5e8|+0x010:'255'
	// <0>
	// <1>
	// <0x94098>|0x5f8|+0x027:'258'
	// <0>
	// <0x940bf>|0x61f|+0x010:'260'
	// <0>
	// <0x940cf>|0x62f|+0x006:'262'
	// <0x940d5>|0x635|+0x021:'263'
	// <0>
	// <0x940f6>|0x656|+0x010:'265'
	// <0>
	// <1>
	// <0x94106>|0x666|+0x027:'268'
	// <0>
	// <0x9412d>|0x68d|+0x014:'270'
	// <0>
	// <0x94141>|0x6a1|+0x016:'272'
	// <0x94157>|0x6b7|+0x013:'273'
	// <0x9416a>|0x6ca|+0x011:'274'
	// <0x9417b>|0x6db|+0x02b:'275'
	// <0x941a6>|0x706|+0x010:'276'
	// <0x941b6>|0x716|+0x01f:'277'
	// <0x941d5>|0x735|+0x010:'278'
	// <0>
	// <1>
	// <0x941e5>|0x745|-0x6e1:'281'
	// <0>
	// <0x93b04>|0x064|+0x09d:'283'
	// <0x93ba1>|0x101|+0x050:'283'
	// <0x93bf1>|0x151|+0x028:'283'
	// <0x93c19>|0x179|+0x02d:'283'
	// <0x93c46>|0x1a6|+0x0b8:'283'
	// <0x93cfe>|0x25e|+0x2c9:'283'
	// <0x93fc7>|0x527|+0x02f:'283'
	// <0x93ff6>|0x556|+0x052:'283'
	// <0x94048>|0x5a8|+0x037:'283'
	// <0x9407f>|0x5df|+0x037:'283'
	// <0x940b6>|0x616|+0x037:'283'
	// <0x940ed>|0x64d|+0x037:'283'
	// <0x94124>|0x684|+0x0a8:'283'
	// <0x941cc>|0x72c|+0x021:'283'
	// <0x941ed>|0x74d|      :'283'	}
	// ******
}

// STATE[STUB]
void lobby_menu::show_match_making( bool b_show )
{
	// LOCALS
	// flash_value 						labels_array
	// flash_value 						text
	// wchar_t[512] 					label_txt
	// flash_value 						label
	// flash_value 						label_member
	// ******

	// FUNCTION BODY[0x7466e0]: 53
	// <0x7466e0>|0x000|+0x006:'321'	{
	// <0x7466e6>|0x006|+0x01b:'322'
	// <0>
	// <1>
	// <0x746701>|0x021|+0x008:'325'
	// <0>
	// <0x746709>|0x029|+0x003:'327'
	// <0>
	// <0x74670c>|0x02c|+0x02e:'329'
	// <0>
	// <1>
	// <0x74673a>|0x05a|+0x036:'332'
	// <0>
	// <1>
	// <2>
	// <0x746770>|0x090|+0x020:'336'
	// <0>
	// <1>
	// <2>
	// <0x746790>|0x0b0|+0x018:'340'
	// <0x7467a8>|0x0c8|+0x02a:'341'
	// <0>
	// <1>
	// <0x7467d2>|0x0f2|+0x01e:'344'
	// <0x7467f0>|0x110|+0x009:'345'
	// <0>
	// <0x7467f9>|0x119|+0x02c:'347'
	// <0>
	// <0x746825>|0x145|+0x01a:'349'
	// <0x74683f>|0x15f|+0x05c:'350'
	// <0>
	// <0x74689b>|0x1bb|+0x028:'352'
	// <0>
	// <0x7468c3>|0x1e3|+0x008:'354'
	// <0>
	// <0x7468cb>|0x1eb|+0x00e:'356'
	// <0x7468d9>|0x1f9|+0x01c:'357'
	// <0>
	// <1>
	// <0x7468f5>|0x215|+0x005:'360'
	// <0x7468fa>|0x21a|+0x01c:'361'
	// <0>
	// <1>
	// <0x746916>|0x236|+0x00a:'364'
	// <0x746920>|0x240|+0x01c:'365'
	// <0>
	// <0x74693c>|0x25c|+0x00a:'367'
	// <0x746946>|0x266|+0x01d:'368'
	// <0x746963>|0x283|+0x00b:'369'
	// <0x74696e>|0x28e|+0x049:'370'
	// <0x7469b7>|0x2d7|+0x015:'371'
	// <0x7469cc>|0x2ec|-0x012:'372'
	// <0>
	// <0x7469ba>|0x2da|+0x01f:'374'
	// <0x7469d9>|0x2f9|-0x016:'374'
	// <0x7469c3>|0x2e3|+0x019:'375'
	// <0x7469dc>|0x2fc|      :'375'	}
	// ******
}

// STATE[STUB]
void lobby_menu::update_level_loading_progress( )
{
	// LOCALS
	// wchar_t[512] 					queries_count
	// flash_value 						progress
	// wchar_t[512] 					w_text
	// char[64] 						buff
	// wchar_t[512] 					level_name
	// flash_value 						text
	// ******

	// FUNCTION BODY[0x743440]: 32
	// <0>
	// <0x743440>|0x000|+0x034:'380'
	// <0>
	// <0x743474>|0x034|+0x021:'382'
	// <0>
	// <0x743495>|0x055|+0x019:'384'
	// <0x7434ae>|0x06e|+0x03e:'385'
	// <0>
	// <1>
	// <0x7434ec>|0x0ac|+0x018:'388'
	// <0>
	// <0x743504>|0x0c4|+0x066:'390'
	// <0>
	// <1>
	// <2>
	// <0x74356a>|0x12a|+0x030:'394'
	// <0x74359a>|0x15a|+0x025:'395'
	// <0>
	// <0x7435bf>|0x17f|+0x01d:'397'
	// <0x7435dc>|0x19c|+0x01a:'398'
	// <0x7435f6>|0x1b6|+0x01d:'399'
	// <0>
	// <1>
	// <0x743613>|0x1d3|+0x018:'402'
	// <0x74362b>|0x1eb|+0x01f:'403'
	// <0>
	// <1>
	// <2>
	// <0x74364a>|0x20a|+0x02b:'407'
	// <0>
	// <1>
	// <0x743675>|0x235|+0x067:'410'
	// ******
}

// STATE[STUB]
void lobby_menu::on_ui_destroy( )
{
	// CALL SITE INFO
	// <0x743acc> -> void* < unknown >( u32 )
	// ******

	// FUNCTION BODY[0x743aa0]: 2
	// <0x743aa1>|0x001|+0x04a:'415'
	// <0x743aeb>|0x04b|+0x016:'416'
	// ******
}

// STATE[STUB]
void lobby_menu::update_ui( const u32 frame_delta_in_ms, const u32 current_time_in_ms )
{
	// LOCALS
	// flash_value 						queries_count
	// float 							deltaTime
	// ******

	// FUNCTION BODY[0x743740]: 20
	// <0>
	// <1>
	// <0x743740>|0x000|+0x012:'424'
	// <0x743752>|0x012|+0x006:'425'
	// <0>
	// <0x743758>|0x018|+0x009:'427'
	// <0>
	// <1>
	// <0x743761>|0x021|+0x026:'430'
	// <0x743787>|0x047|+0x02e:'431'
	// <0x7437b5>|0x075|+0x01f:'432'
	// <0>
	// <0x7437d4>|0x094|+0x012:'434'
	// <0>
	// <0x7437e6>|0x0a6|+0x028:'436'
	// <0x74380e>|0x0ce|+0x024:'437'
	// <0x743832>|0x0f2|+0x024:'438'
	// <0>
	// <0x743856>|0x116|+0x00b:'440'
	// <0x743861>|0x121|+0x024:'441'
	// ******
}

// STATE[STUB]
void lobby_menu::update_status( )
{
	// LOCALS
	// flash_value 						b_val
	// fixed_string< 128 > 				status_str
	// flash_value 						account_info
	// fixed_string< 128 > 				buff
	// flash_value 						log_message
	// flash_value 						port
	// ******

	// CALL SITE INFO
	// <0x743244> -> network::login_client& < unknown >()
	// <0x743259> -> lobby_client& < unknown >()
	// ******

	// FUNCTION BODY[0x743190]: 35
	// <0x7431a0>|0x010|+0x010:'446'
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x7431b0>|0x020|+0x051:'451'
	// <0x743201>|0x071|+0x004:'452'
	// <0>
	// <1>
	// <2>
	// <0x743205>|0x075|+0x02e:'456'
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x743233>|0x0a3|+0x013:'461'
	// <0x743246>|0x0b6|+0x015:'462'
	// <0>
	// <1>
	// <0x74325b>|0x0cb|+0x023:'465'
	// <0x74327e>|0x0ee|+0x013:'466'
	// <0x743291>|0x101|+0x011:'467'
	// <0>
	// <1>
	// <0x7432a2>|0x112|+0x032:'470'
	// <0x7432d4>|0x144|+0x01d:'471'
	// <0x7432f1>|0x161|+0x018:'472'
	// <0x743309>|0x179|+0x015:'473'
	// <0>
	// <0x74331e>|0x18e|+0x021:'475'
	// <0>
	// <1>
	// <0x74333f>|0x1af|+0x018:'478'
	// <0x743357>|0x1c7|+0x021:'479'
	// <0x743378>|0x1e8|+0x01f:'480'
	// ******
}

// STATE[STUB]
void lobby_menu::on_render_scenes_ready( resources::queries_result& data )
{
	// LOCALS
	// flash_value 						func
	// float3 							pos
	// float3 							dir
	// flash_value 						proxy
	// flash_value 						players_count
	// configs::binary_config_ptr 		player_config
	// ******

	// CALL SITE INFO
	// <0x74729d> -> sound::world_user& < unknown >() const
	// <0x74797f> -> void < unknown >( bool )
	// <0x7479d7> -> void < unknown >()
	// ******

	// FUNCTION BODY[0x747070]: 92
	// <0>
	// <1>
	// <0x74707c>|0x00c|+0x078:'487'
	// <0x7470f4>|0x084|+0x06f:'488'
	// <0x747163>|0x0f3|+0x0a8:'489'
	// <0>
	// <0x74720b>|0x19b|+0x09b:'491'
	// <0>
	// <0x7472a6>|0x236|+0x0a5:'493'
	// <0>
	// <0x74734b>|0x2db|+0x024:'495'
	// <0>
	// <1>
	// <0x74736f>|0x2ff|+0x0a5:'498'
	// <0x747414>|0x3a4|+0x01f:'499'
	// <0x747433>|0x3c3|+0x006:'500'
	// <0>
	// <0x747439>|0x3c9|+0x0af:'502'
	// <0x7474e8>|0x478|+0x01f:'503'
	// <0x747507>|0x497|+0x006:'504'
	// <0>
	// <0x74750d>|0x49d|+0x0af:'506'
	// <0x7475bc>|0x54c|+0x01f:'507'
	// <0x7475db>|0x56b|+0x006:'508'
	// <0>
	// <0x7475e1>|0x571|+0x0af:'510'
	// <0x747690>|0x620|+0x023:'511'
	// <0x7476b3>|0x643|+0x006:'512'
	// <0>
	// <0x7476b9>|0x649|+0x04f:'514'
	// <0>
	// <0x747708>|0x698|+0x03d:'516'
	// <0>
	// <0x747745>|0x6d5|+0x03d:'518'
	// <0>
	// <0x747782>|0x712|+0x026:'520'
	// <0x7477a8>|0x738|+0x026:'521'
	// <0>
	// <0x7477ce>|0x75e|+0x018:'523'
	// <0x7477e6>|0x776|+0x017:'524'
	// <0>
	// <0x7477fd>|0x78d|+0x018:'526'
	// <0x747815>|0x7a5|+0x017:'527'
	// <0>
	// <0x74782c>|0x7bc|+0x018:'529'
	// <0x747844>|0x7d4|+0x017:'530'
	// <0>
	// <0x74785b>|0x7eb|+0x018:'532'
	// <0x747873>|0x803|+0x017:'533'
	// <0x74788a>|0x81a|+0x026:'534'
	// <0>
	// <1>
	// <2>
	// <0x7478b0>|0x840|+0x026:'538'
	// <0x7478d6>|0x866|+0x023:'539'
	// <0>
	// <0x7478f9>|0x889|+0x02e:'541'
	// <0x747927>|0x8b7|+0x02e:'542'
	// <0>
	// <0x747955>|0x8e5|+0x01f:'544'
	// <0>
	// <0x747974>|0x904|+0x00d:'546'
	// <0>
	// <1>
	// <2>
	// <0x747981>|0x911|+0x035:'550'
	// <0>
	// <0x7479b6>|0x946|+0x00c:'552'
	// <0x7479c2>|0x952|+0x006:'553'
	// <0>
	// <0x7479c8>|0x958|+0x008:'555'
	// <0>
	// <0x7479d0>|0x960|+0x002:'557'
	// <0x7479d2>|0x962|+0x00e:'558'
	// <0>
	// <1>
	// <0x7479e0>|0x970|+0x01e:'561'
	// <0x7479fe>|0x98e|+0x036:'562'
	// <0>
	// <0x747a34>|0x9c4|+0x01a:'564'
	// <0x747a4e>|0x9de|+0x01a:'565'
	// <0>
	// <0x747a68>|0x9f8|+0x02d:'567'
	// <0x747a95>|0xa25|+0x03b:'568'
	// <0x747ad0>|0xa60|+0x0ca:'569'
	// <0>
	// <0x747b9a>|0xb2a|+0x008:'571'
	// <0x747ba2>|0xb32|+0x006:'572'
	// <0>
	// <0x747ba8>|0xb38|+0x01f:'574'
	// <0>
	// <0x747bc7>|0xb57|+0x008:'576'
	// ******
}

// STATE[STUB]
void lobby_menu::fill_inventory_labels( )
{
	// LOCALS
	// flash_value 						labels
	// wchar_t[512] 					label_w
	// flash_value 						label_translate
	// ******

	// FUNCTION BODY[0x745140]: 19
	// <0>
	// <0x74514f>|0x00f|+0x035:'676'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <7>
	// <8>
	// <0x745184>|0x044|+0x020:'686'
	// <0>
	// <0x7451a4>|0x064|+0x068:'688'
	// <0>
	// <0x74520c>|0x0cc|+0x02a:'690'
	// <0x745236>|0x0f6|+0x02e:'691'
	// <0>
	// <0x745264>|0x124|+0x021:'693'
	// ******
}

// STATE[STUB]
void lobby_menu::fill_items_dictionary( )
{
	// LOCALS
	// map< u32, dictionary_item, std::less< u32 > > const& itm_dict
	// std::priv::_Rb_tree_iterator< std::pair< u32 const , dictionary_item >, std::priv::_ConstMapTraitsT< std::pair< u32 const , dictionary_item > > > itm_it
	// fixed_string< 32 >[6] 			sellers_names
	// flash_value 						traders_array_item_property
	// flash_value 						items_descr_array
	// flash_value 						inventory_item_property
	// flash_value 						traders_array
	// u32 								in_array_index
	// u32 								icon
	// flash_value 						inventory_item_propertyies_array
	// flash_value 						item_property_member
	// u8 								current_item_category_id
	// wchar_t[512] 					item_name
	// u32 								current_item_dict_id
	// wchar_t[512] 					item_desc
	// float 							item_weight
	// bool 							current_item_is_stack
	// configs::binary_config_value const* it_end
	// flash_value 						inventory_item_descr
	// dictionary_item 					current_item
	// u32 								j
	// wchar_t[512] 					prop_name
	// flash_value 						item_property
	// u32 								prop_icon
	// flash_value 						traders_array_item
	// wchar_t[512] 					faction_name
	// ******

	// FUNCTION BODY[0x745500]: 144
	// <0x74550f>|0x00f|+0x006:'699'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <0x745515>|0x015|+0x015:'705'
	// <0>
	// <1>
	// <2>
	// <0x74552a>|0x02a|+0x056:'709'
	// <0>
	// <0x745580>|0x080|+0x013:'711'
	// <0x745593>|0x093|+0x007:'712'
	// <0>
	// <0x74559a>|0x09a|+0x014:'714'
	// <0x7455ae>|0x0ae|+0x00b:'715'
	// <0>
	// <0x7455b9>|0x0b9|+0x01f:'717'
	// <0x7455d8>|0x0d8|+0x0f9:'718'
	// <0x7456d1>|0x1d1|+0x00e:'719'
	// <0>
	// <1>
	// <0x7456df>|0x1df|+0x01a:'722'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <0x7456f9>|0x1f9|+0x047:'728'
	// <0>
	// <0x745740>|0x240|+0x040:'730'
	// <0>
	// <1>
	// <0x745780>|0x280|+0x026:'733'
	// <0>
	// <1>
	// <2>
	// <0x7457a6>|0x2a6|+0x02b:'737'
	// <0x7457d1>|0x2d1|+0x038:'738'
	// <0>
	// <0x745809>|0x309|+0x010:'740'
	// <0x745819>|0x319|+0x02c:'741'
	// <0>
	// <0x745845>|0x345|+0x00c:'743'
	// <0x745851>|0x351|+0x02a:'744'
	// <0>
	// <0x74587b>|0x37b|+0x02b:'746'
	// <0x7458a6>|0x3a6|+0x03a:'747'
	// <0>
	// <0x7458e0>|0x3e0|+0x028:'749'
	// <0x745908>|0x408|+0x038:'750'
	// <0>
	// <1>
	// <0x745940>|0x440|+0x01f:'753'
	// <0x74595f>|0x45f|+0x03e:'754'
	// <0x74599d>|0x49d|+0x03a:'755'
	// <0x7459d7>|0x4d7|+0x029:'756'
	// <0>
	// <1>
	// <0x745a00>|0x500|+0x00b:'759'
	// <0>
	// <0x745a0b>|0x50b|+0x02e:'761'
	// <0x745a39>|0x539|+0x045:'762'
	// <0>
	// <0x745a7e>|0x57e|+0x029:'764'
	// <0x745aa7>|0x5a7|+0x034:'765'
	// <0>
	// <1>
	// <2>
	// <0x745adb>|0x5db|+0x02d:'769'
	// <0>
	// <0x745b08>|0x608|+0x01a:'771'
	// <0x745b22>|0x622|+0x02b:'772'
	// <0>
	// <0x745b4d>|0x64d|+0x008:'774'
	// <0>
	// <0x745b55>|0x655|+0x2a8:'776'
	// <0x745dfd>|0x8fd|-0x29a:'776'
	// <0>
	// <1>
	// <0x745b63>|0x663|+0x027:'779'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <0x745b8a>|0x68a|+0x028:'785'
	// <0>
	// <0x745bb2>|0x6b2|+0x00e:'787'
	// <0x745bc0>|0x6c0|+0x02e:'788'
	// <0>
	// <0x745bee>|0x6ee|+0x079:'790'
	// <0x745c67>|0x767|+0x02c:'791'
	// <0>
	// <0x745c93>|0x793|+0x083:'793'
	// <0x745d16>|0x816|+0x02e:'794'
	// <0>
	// <0x745d44>|0x844|+0x02c:'796'
	// <0x745d70>|0x870|+0x038:'797'
	// <0>
	// <0x745da8>|0x8a8|+0x020:'799'
	// <0x745dc8>|0x8c8|+0x037:'800'
	// <0>
	// <0x745dff>|0x8ff|+0x031:'802'
	// <0>
	// <0x745e30>|0x930|+0x023:'804'
	// <0>
	// <0x745e53>|0x953|+0x0dc:'806'
	// <0x745f2f>|0xa2f|+0x029:'807'
	// <0>
	// <1>
	// <2>
	// <0x745f58>|0xa58|+0x02a:'811'
	// <0>
	// <0x745f82>|0xa82|+0x028:'813'
	// <0x745faa>|0xaaa|+0x04b:'814'
	// <0x745ff5>|0xaf5|+0x050:'815'
	// <0x746045>|0xb45|+0x050:'816'
	// <0x746095>|0xb95|+0x050:'817'
	// <0x7460e5>|0xbe5|+0x050:'818'
	// <0x746135>|0xc35|+0x050:'819'
	// <0>
	// <0x746185>|0xc85|+0x015:'821'
	// <0>
	// <0x74619a>|0xc9a|+0x006:'823'
	// <0>
	// <1>
	// <0x7461a0>|0xca0|+0x029:'826'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <0x7461c9>|0xcc9|+0x01e:'832'
	// <0>
	// <0x7461e7>|0xce7|+0x098:'834'
	// <0x74627f>|0xd7f|+0x031:'835'
	// <0>
	// <0x7462b0>|0xdb0|+0x034:'837'
	// <0x7462e4>|0xde4|+0x046:'838'
	// <0>
	// <0x74632a>|0xe2a|+0x01c:'840'
	// <0x746346>|0xe46|+0x030:'841'
	// <0x746376>|0xe76|+0x029:'842'
	// ******
}

// STATE[STUB]
void lobby_menu::fill_inventory_contents( )
{
	// LOCALS
	// flash_value 						inventory_array
	// flash_value 						inventory_item_property
	// inventory_item_instance const* 	it_e
	// u32 								i
	// flash_value 						inventory_item
	// ******

	// CALL SITE INFO
	// <0x742ebc> -> lobby_client& < unknown >()
	// ******

	// FUNCTION BODY[0x742ea0]: 39
	// <0x742ea4>|0x004|+0x01a:'847'
	// <0>
	// <1>
	// <2>
	// <0x742ebe>|0x01e|+0x00c:'851'
	// <0>
	// <1>
	// <0x742eca>|0x02a|+0x02c:'854'
	// <0>
	// <0x742ef6>|0x056|+0x008:'856'
	// <0>
	// <0x742efe>|0x05e|+0x012:'858'
	// <0>
	// <1>
	// <0x742f10>|0x070|+0x024:'861'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <7>
	// <0x742f34>|0x094|+0x032:'870'
	// <0x742f66>|0x0c6|+0x03a:'871'
	// <0>
	// <0x742fa0>|0x100|+0x029:'873'
	// <0x742fc9>|0x129|+0x03a:'874'
	// <0>
	// <0x743003>|0x163|+0x02b:'876'
	// <0x74302e>|0x18e|+0x03b:'877'
	// <0>
	// <0x743069>|0x1c9|+0x029:'879'
	// <0x743092>|0x1f2|+0x036:'880'
	// <0>
	// <0x7430c8>|0x228|+0x01a:'882'
	// <0x7430e2>|0x242|+0x037:'883'
	// <0>
	// <0x743119>|0x279|+0x021:'885'
	// ******
}

// STATE[STUB]
void lobby_menu::on_items_compatibility_arrived( )
{
	// LOCALS
	// flash_value 						slot_restrictions_array
	// flash_value 						items_compatibility_item_property
	// u8 								i
	// flash_value 						items_compatibility_item
	// ******

	// CALL SITE INFO
	// <0x742c8c> -> lobby_client& < unknown >()
	// ******

	// FUNCTION BODY[0x742c70]: 26
	// <0x742c76>|0x006|+0x018:'890'
	// <0>
	// <0x742c8e>|0x01e|+0x025:'892'
	// <0>
	// <0x742cb3>|0x043|+0x008:'894'
	// <0>
	// <0x742cbb>|0x04b|+0x163:'896'
	// <0x742e1e>|0x1ae|-0x14e:'896'
	// <0>
	// <0x742cd0>|0x060|+0x006:'898'
	// <0>
	// <1>
	// <0x742cd6>|0x066|+0x02b:'901'
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x742d01>|0x091|+0x02b:'906'
	// <0x742d2c>|0x0bc|+0x03a:'907'
	// <0>
	// <0x742d66>|0x0f6|+0x02d:'909'
	// <0x742d93>|0x123|+0x03a:'910'
	// <0>
	// <0x742dcd>|0x15d|+0x016:'912'
	// <0x742de3>|0x173|+0x03f:'913'
	// <0>
	// <0x742e22>|0x1b2|+0x021:'915'
	// ******
}

// STATE[STUB]
void lobby_menu::on_slot_restrictions_arrived( )
{
	// LOCALS
	// flash_value 						slot_restrictions_array
	// flash_value 						slot_restriction_item_property
	// u8 								i
	// flash_value 						slot_restriction_item
	// ******

	// CALL SITE INFO
	// <0x742a5c> -> lobby_client& < unknown >()
	// ******

	// FUNCTION BODY[0x742a40]: 26
	// <0x742a46>|0x006|+0x018:'920'
	// <0>
	// <0x742a5e>|0x01e|+0x025:'922'
	// <0>
	// <0x742a83>|0x043|+0x008:'924'
	// <0>
	// <0x742a8b>|0x04b|+0x163:'926'
	// <0x742bee>|0x1ae|-0x14e:'926'
	// <0>
	// <0x742aa0>|0x060|+0x006:'928'
	// <0>
	// <1>
	// <0x742aa6>|0x066|+0x02b:'931'
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x742ad1>|0x091|+0x02b:'936'
	// <0x742afc>|0x0bc|+0x03a:'937'
	// <0>
	// <0x742b36>|0x0f6|+0x02d:'939'
	// <0x742b63>|0x123|+0x03a:'940'
	// <0>
	// <0x742b9d>|0x15d|+0x016:'942'
	// <0x742bb3>|0x173|+0x03f:'943'
	// <0>
	// <0x742bf2>|0x1b2|+0x021:'945'
	// ******
}

// STATE[STUB]
void lobby_menu::fill_profiles( )
{
	// LOCALS
	// flash_value 						profile_item_property
	// flash_value 						profiles_array
	// wchar_t[512] 					profile_name_w
	// flash_value 						profile_item
	// ******

	// CALL SITE INFO
	// <0x7427f2> -> lobby_client& < unknown >()
	// ******

	// FUNCTION BODY[0x7427d0]: 28
	// <0>
	// <0x7427df>|0x00f|+0x015:'951'
	// <0>
	// <0x7427f4>|0x024|+0x025:'953'
	// <0>
	// <0x742819>|0x049|+0x006:'955'
	// <0>
	// <0x74281f>|0x04f|+0x008:'957'
	// <0>
	// <0x742827>|0x057|+0x019:'959'
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x742840>|0x070|+0x024:'964'
	// <0>
	// <1>
	// <0x742864>|0x094|+0x01e:'967'
	// <0>
	// <0x742882>|0x0b2|+0x06d:'969'
	// <0x7428ef>|0x11f|+0x02c:'970'
	// <0>
	// <0x74291b>|0x14b|+0x024:'972'
	// <0x74293f>|0x16f|+0x03e:'973'
	// <0>
	// <0x74297d>|0x1ad|+0x016:'975'
	// <0x742993>|0x1c3|+0x032:'976'
	// <0x7429c5>|0x1f5|+0x021:'977'
	// ******
}

// STATE[STUB]
void lobby_menu::on_profile_changed( u8 profile_id )
{
	// LOCALS
	// flash_value 						profile_descriptor
	// variant< 32 > 					user_data
	// flash_value 						profile_items_array
	// player_profile const& 			profile
	// fixed_string< 260 > 				player_parameters_query_path
	// flash_value 						profile_descriptor_property
	// flash_value 						slot_item_property
	// u8 								in_array_index
	// flash_value 						v
	// u32 								condition_or_stack
	// flash_value 						slot_item
	// ******

	// CALL SITE INFO
	// <0x746a1c> -> lobby_client& < unknown >()
	// ******

	// FUNCTION BODY[0x7469f0]: 79
	// <0x746a01>|0x011|+0x003:'982'
	// <0>
	// <0x746a04>|0x014|+0x01a:'984'
	// <0>
	// <1>
	// <0x746a1e>|0x02e|+0x020:'987'
	// <0>
	// <1>
	// <0x746a3e>|0x04e|+0x02c:'990'
	// <0>
	// <1>
	// <2>
	// <0x746a6a>|0x07a|+0x037:'994'
	// <0>
	// <0x746aa1>|0x0b1|+0x1ce:'996'
	// <0>
	// <0x746c6f>|0x27f|-0x1af:'998'
	// <0>
	// <0x746ac0>|0x0d0|+0x003:'1000'
	// <0x746ac3>|0x0d3|+0x008:'1001'
	// <0>
	// <0x746acb>|0x0db|+0x002:'1003'
	// <0>
	// <0x746acd>|0x0dd|+0x016:'1005'
	// <0>
	// <1>
	// <0x746ae3>|0x0f3|+0x02a:'1008'
	// <0>
	// <1>
	// <2>
	// <0x746b0d>|0x11d|+0x025:'1012'
	// <0x746b32>|0x142|+0x03b:'1013'
	// <0>
	// <0x746b6d>|0x17d|+0x029:'1015'
	// <0x746b96>|0x1a6|+0x034:'1016'
	// <0>
	// <0x746bca>|0x1da|+0x028:'1018'
	// <0x746bf2>|0x202|+0x034:'1019'
	// <0>
	// <0x746c26>|0x236|+0x021:'1021'
	// <0>
	// <1>
	// <0x746c47>|0x257|+0x04f:'1024'
	// <0>
	// <1>
	// <0x746c96>|0x2a6|+0x024:'1027'
	// <0>
	// <0x746cba>|0x2ca|+0x031:'1029'
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x746ceb>|0x2fb|+0x004:'1034'
	// <0x746cef>|0x2ff|+0x042:'1035'
	// <0>
	// <1>
	// <2>
	// <0x746d31>|0x341|+0x055:'1039'
	// <0>
	// <0x746d86>|0x396|+0x025:'1041'
	// <0x746dab>|0x3bb|+0x040:'1042'
	// <0>
	// <0x746deb>|0x3fb|+0x021:'1044'
	// <0>
	// <1>
	// <0x746e0c>|0x41c|+0x00d:'1047'
	// <0x746e19>|0x429|+0x00e:'1048'
	// <0x746e27>|0x437|+0x009:'1049'
	// <0>
	// <0x746e30>|0x440|+0x00e:'1051'
	// <0x746e3e>|0x44e|+0x00c:'1052'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <0x746e4a>|0x45a|+0x10e:'1060'
	// ******
}

// STATE[STUB]
void lobby_menu::player_parameters_ready( resources::queries_result& data, player_parameters_cooker_data* cook_data )
{
	// LOCALS
	// flash_value[2] 					args
	// ******

	// FUNCTION BODY[0x7452c0]: 13
	// <0x7452c9>|0x009|+0x023:'1065'
	// <0>
	// <1>
	// <0x7452ec>|0x02c|+0x040:'1068'
	// <0x74532c>|0x06c|+0x006:'1069'
	// <0x745332>|0x072|+0x022:'1070'
	// <0x745354>|0x094|+0x02f:'1071'
	// <0x745383>|0x0c3|+0x040:'1072'
	// <0x7453c3>|0x103|+0x02a:'1073'
	// <0>
	// <0x7453ed>|0x12d|+0x03c:'1075'
	// <0x745429>|0x169|+0x045:'1076'
	// <0x74546e>|0x1ae|+0x032:'1077'
	// ******
}

// STATE[STUB]
void lobby_menu::on_profile_arrived( u8 profile_id )
{
	// LOCALS
	// flash_value 						profile_id_value
	// ******

	// FUNCTION BODY[0x742760]: 6
	// <0x742763>|0x003|+0x008:'1083'
	// <0>
	// <1>
	// <0x74276b>|0x00b|+0x003:'1086'
	// <0x74276e>|0x00e|+0x036:'1087'
	// <0x7427a4>|0x044|+0x020:'1088'
	// ******
}

// STATE[STUB]
void lobby_menu::on_price_items_arrived( u8 trader_id )
{
	// LOCALS
	// configs::binary_config_value const& faction_levels
	// u8 								levels_count
	// lobby_client& 					lobby_client
	// char[32] 						faction_str
	// u8 								current_reputation_level
	// flash_value 						price_item_property
	// flash_value[5] 					current_level
	// wchar_t[512] 					faction_level_name_w
	// flash_value 						prices_array_item
	// ******

	// CALL SITE INFO
	// <0x744c32> -> lobby_client& < unknown >()
	// ******

	// FUNCTION BODY[0x744c10]: 55
	// <0x744c17>|0x007|+0x0c9:'1093'
	// <0x744ce0>|0x0d0|-0x0ac:'1093'
	// <0>
	// <1>
	// <0x744c34>|0x024|+0x020:'1096'
	// <0x744c54>|0x044|+0x042:'1097'
	// <0>
	// <0x744c96>|0x086|+0x026:'1099'
	// <0>
	// <1>
	// <2>
	// <0x744cbc>|0x0ac|+0x028:'1103'
	// <0>
	// <0x744ce4>|0x0d4|+0x015:'1105'
	// <0>
	// <0x744cf9>|0x0e9|+0x01c:'1107'
	// <0>
	// <0x744d15>|0x105|+0x02d:'1109'
	// <0>
	// <0x744d42>|0x132|+0x028:'1111'
	// <0>
	// <0x744d6a>|0x15a|+0x03f:'1113'
	// <0>
	// <1>
	// <0x744da9>|0x199|+0x01f:'1116'
	// <0x744dc8>|0x1b8|+0x083:'1117'
	// <0>
	// <0x744e4b>|0x23b|+0x019:'1119'
	// <0>
	// <0x744e64>|0x254|+0x058:'1121'
	// <0x744ebc>|0x2ac|-0x025:'1121'
	// <0>
	// <1>
	// <2>
	// <0x744e97>|0x287|+0x029:'1125'
	// <0>
	// <0x744ec0>|0x2b0|+0x013:'1127'
	// <0>
	// <0x744ed3>|0x2c3|+0x00f:'1129'
	// <0>
	// <1>
	// <2>
	// <0x744ee2>|0x2d2|+0x02d:'1133'
	// <0>
	// <0x744f0f>|0x2ff|+0x02c:'1135'
	// <0x744f3b>|0x32b|+0x038:'1136'
	// <0>
	// <0x744f73>|0x363|+0x028:'1138'
	// <0x744f9b>|0x38b|+0x03f:'1139'
	// <0>
	// <0x744fda>|0x3ca|+0x02d:'1141'
	// <0x745007>|0x3f7|+0x036:'1142'
	// <0>
	// <0x74503d>|0x42d|+0x015:'1144'
	// <0x745052>|0x442|+0x03a:'1145'
	// <0x74508c>|0x47c|+0x028:'1146'
	// <0x7450b4>|0x4a4|+0x073:'1147'
	// ******
}

// STATE[STUB]
void lobby_menu::reset_account_money( )
{
	// LOCALS
	// flash_value 						account_info_property
	// wchar_t[256] 					an
	// flash_value 						account_info
	// ******

	// FUNCTION BODY[0x742550]: 19
	// <0>
	// <0x74255e>|0x00e|+0x029:'1153'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <0x742587>|0x037|+0x039:'1159'
	// <0>
	// <0x7425c0>|0x070|+0x00d:'1161'
	// <0x7425cd>|0x07d|+0x02c:'1162'
	// <0>
	// <0x7425f9>|0x0a9|+0x03e:'1164'
	// <0x742637>|0x0e7|+0x03b:'1165'
	// <0>
	// <0x742672>|0x122|+0x03e:'1167'
	// <0x7426b0>|0x160|+0x036:'1168'
	// <0>
	// <0x7426e6>|0x196|+0x021:'1170'
	// ******
}

// STATE[STUB]
void lobby_menu::on_shop_ui_ready( )
{
	// CALL SITE INFO
	// <0x743b32> -> lobby_client& < unknown >()
	// ******

	// FUNCTION BODY[0x743b10]: 5
	// <0>
	// <0x743b17>|0x007|+0x009:'1176'
	// <0>
	// <0x743b20>|0x010|+0x020:'1178'
	// <0>
	// ******
}

// STATE[STUB]
void lobby_menu::fill_skills_tree( )
{
	// LOCALS
	// configs::binary_config_value const& tree
	// configs::binary_config_value const& db_dictionaries
	// flash_value 						skills_tree_value_prop
	// flash_value 						skills_tree_value
	// wchar_t[512] 					branch_descr
	// char[32] 						skill_branch_name
	// u32 								levels_count
	// wchar_t[512] 					branch_name
	// u8 								skill_id
	// flash_value 						skills_tree_level_value_prop
	// configs::binary_config_value const& current_skill_value
	// char[32] 						skill_name
	// u32 								i
	// configs::binary_config_value const* booster_it_e
	// flash_value 						skills_tree_level_value
	// flash_value 						item_property_member
	// configs::binary_config_value const& current_level_cfg
	// char[32] 						skill_level_table_name
	// u32 								booster_index
	// char[32] 						prop_value_str
	// char[32] 						booster_table_name
	// wchar_t[512] 					prop_name
	// float 							prop_value
	// flash_value 						item_property
	// flash_value 						perk_property_member
	// configs::binary_config_value const* perk_it_e
	// configs::binary_config_value const* perk_it
	// u32 								perk_index
	// wchar_t[512] 					perk_name
	// flash_value 						perk_property
	// char[32] 						perk_table_name
	// wchar_t[512] 					perk_descr
	// ******

	// FUNCTION BODY[0x743cd0]: 156
	// <0x743cda>|0x00a|+0x022:'1184'
	// <0>
	// <0x743cfc>|0x02c|+0x01f:'1186'
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x743d1b>|0x04b|+0x024:'1191'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <0x743d3f>|0x06f|+0x019:'1198'
	// <0>
	// <0x743d58>|0x088|+0x019:'1200'
	// <0>
	// <0x743d71>|0x0a1|+0x015:'1202'
	// <0>
	// <1>
	// <0x743d86>|0x0b6|+0x031:'1205'
	// <0>
	// <1>
	// <0x743db7>|0x0e7|+0x01e:'1208'
	// <0>
	// <1>
	// <2>
	// <0x743dd5>|0x105|+0x045:'1212'
	// <0>
	// <0x743e1a>|0x14a|+0x06e:'1214'
	// <0x743e88>|0x1b8|+0x037:'1215'
	// <0>
	// <1>
	// <0x743ebf>|0x1ef|+0x045:'1218'
	// <0>
	// <0x743f04>|0x234|+0x06e:'1220'
	// <0x743f72>|0x2a2|+0x037:'1221'
	// <0>
	// <0x743fa9>|0x2d9|+0x050:'1223'
	// <0x743ff9>|0x329|+0x041:'1224'
	// <0>
	// <0x74403a>|0x36a|+0x028:'1226'
	// <0x744062>|0x392|+0x042:'1227'
	// <0>
	// <0x7440a4>|0x3d4|+0x024:'1229'
	// <0x7440c8>|0x3f8|+0x03f:'1230'
	// <0>
	// <0x744107>|0x437|+0x019:'1232'
	// <0>
	// <1>
	// <2>
	// <0x744120>|0x450|+0x038:'1236'
	// <0x744158>|0x488|+0x9b4:'1237'
	// <0x744b0c>|0xe3c|-0x993:'1237'
	// <0>
	// <1>
	// <0x744179>|0x4a9|+0x019:'1240'
	// <0x744192>|0x4c2|+0x01b:'1241'
	// <0>
	// <1>
	// <0x7441ad>|0x4dd|+0x036:'1244'
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x7441e3>|0x513|+0x083:'1249'
	// <0x744266>|0x596|+0x03a:'1250'
	// <0>
	// <0x7442a0>|0x5d0|+0x00c:'1252'
	// <0>
	// <0x7442ac>|0x5dc|+0x039:'1254'
	// <0x7442e5>|0x615|+0x04d:'1255'
	// <0>
	// <0x744332>|0x662|+0x01c:'1257'
	// <0>
	// <0x74434e>|0x67e|+0x00e:'1259'
	// <0x74435c>|0x68c|+0x01c:'1260'
	// <0>
	// <0x744378>|0x6a8|+0x008:'1262'
	// <0>
	// <0x744380>|0x6b0|+0x2fe:'1264'
	// <0x74467e>|0x9ae|-0x2ee:'1264'
	// <0>
	// <1>
	// <0x744390>|0x6c0|+0x024:'1267'
	// <0>
	// <0x7443b4>|0x6e4|+0x01d:'1269'
	// <0x7443d1>|0x701|+0x015:'1270'
	// <0>
	// <1>
	// <0x7443e6>|0x716|+0x017:'1273'
	// <0>
	// <1>
	// <0x7443fd>|0x72d|+0x045:'1276'
	// <0>
	// <1>
	// <0x744442>|0x772|+0x022:'1279'
	// <0>
	// <0x744464>|0x794|+0x074:'1281'
	// <0x7444d8>|0x808|+0x02e:'1282'
	// <0>
	// <0x744506>|0x836|+0x06e:'1284'
	// <0x744574>|0x8a4|+0x02c:'1285'
	// <0>
	// <0x7445a0>|0x8d0|+0x050:'1287'
	// <0x7445f0>|0x920|+0x03d:'1288'
	// <0>
	// <0x74462d>|0x95d|+0x020:'1290'
	// <0x74464d>|0x97d|+0x038:'1291'
	// <0>
	// <0x744685>|0x9b5|+0x03a:'1293'
	// <0x7446bf>|0x9ef|+0x014:'1294'
	// <0x7446d3>|0xa03|+0x01c:'1295'
	// <0>
	// <0x7446ef>|0xa1f|+0x00e:'1297'
	// <0x7446fd>|0xa2d|+0x023:'1298'
	// <0>
	// <0x744720>|0xa50|+0x008:'1300'
	// <0>
	// <0x744728>|0xa58|+0x01c:'1302'
	// <0>
	// <1>
	// <0x744744>|0xa74|+0x024:'1305'
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x744768>|0xa98|+0x00e:'1310'
	// <0>
	// <1>
	// <0x744776>|0xaa6|+0x012:'1313'
	// <0>
	// <1>
	// <0x744788>|0xab8|+0x048:'1316'
	// <0>
	// <1>
	// <0x7447d0>|0xb00|+0x045:'1319'
	// <0>
	// <0x744815>|0xb45|+0x083:'1321'
	// <0x744898>|0xbc8|+0x02e:'1322'
	// <0>
	// <0x7448c6>|0xbf6|+0x07e:'1324'
	// <0x744944>|0xc74|+0x02c:'1325'
	// <0>
	// <0x744970>|0xca0|+0x024:'1327'
	// <0x744994>|0xcc4|+0x03a:'1328'
	// <0>
	// <0x7449ce>|0xcfe|+0x020:'1330'
	// <0x7449ee>|0xd1e|+0x03c:'1331'
	// <0x744a2a>|0xd5a|+0x03a:'1332'
	// <0x744a64>|0xd94|+0x021:'1333'
	// <0x744a85>|0xdb5|+0x020:'1334'
	// <0x744aa5>|0xdd5|+0x06c:'1335'
	// <0x744b11>|0xe41|+0x037:'1336'
	// <0>
	// <0x744b48>|0xe78|+0x023:'1338'
	// <0x744b6b>|0xe9b|+0x07b:'1339'
	// ******
}

// STATE[STUB]
void lobby_menu::fill_character_data( )
{
	// LOCALS
	// flash_value 						player_skills_value_prop
	// flash_value 						player_skills_value
	// u8 								total_points_in_tree
	// flash_value 						skill_value_prop
	// u8 								i
	// flash_value 						skill_value
	// flash_value 						perk_value
	// ******

	// FUNCTION BODY[0x741ee0]: 60
	// <0>
	// <0x741ee3>|0x003|+0x029:'1345'
	// <0>
	// <1>
	// <0x741f0c>|0x02c|+0x021:'1348'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <0x741f2d>|0x04d|+0x1ab:'1354'
	// <0x7420d8>|0x1f8|-0x176:'1354'
	// <0>
	// <0x741f62>|0x082|+0x01e:'1356'
	// <0>
	// <0x741f80>|0x0a0|+0x027:'1358'
	// <0>
	// <1>
	// <2>
	// <0x741fa7>|0x0c7|+0x02c:'1362'
	// <0x741fd3>|0x0f3|+0x038:'1363'
	// <0>
	// <0x74200b>|0x12b|+0x02c:'1365'
	// <0x742037>|0x157|+0x03a:'1366'
	// <0>
	// <0x742071>|0x191|+0x016:'1368'
	// <0>
	// <1>
	// <0x742087>|0x1a7|+0x053:'1371'
	// <0x7420da>|0x1fa|+0x02c:'1372'
	// <0>
	// <0x742106>|0x226|+0x046:'1374'
	// <0x74214c>|0x26c|+0x039:'1375'
	// <0>
	// <0x742185>|0x2a5|+0x03f:'1377'
	// <0x7421c4>|0x2e4|+0x036:'1378'
	// <0>
	// <0x7421fa>|0x31a|+0x059:'1380'
	// <0x742253>|0x373|+0x036:'1381'
	// <0>
	// <0x742289>|0x3a9|+0x013:'1383'
	// <0x74229c>|0x3bc|+0x01f:'1384'
	// <0>
	// <0x7422bb>|0x3db|+0x02f:'1386'
	// <0x7422ea>|0x40a|+0x036:'1387'
	// <0>
	// <0x742320>|0x440|+0x02b:'1389'
	// <0x74234b>|0x46b|+0x036:'1390'
	// <0>
	// <0x742381>|0x4a1|+0x019:'1392'
	// <0>
	// <0x74239a>|0x4ba|+0x0df:'1394'
	// <0x742479>|0x599|-0x0b9:'1394'
	// <0>
	// <1>
	// <2>
	// <0x7423c0>|0x4e0|+0x056:'1398'
	// <0x742416>|0x536|+0x022:'1399'
	// <0x742438>|0x558|+0x045:'1400'
	// <0x74247d>|0x59d|+0x02c:'1401'
	// <0>
	// <0x7424a9>|0x5c9|+0x021:'1403'
	// ******
}

// STATE[STUB]
void lobby_menu::fill_service_prices( )
{
	// LOCALS
	// flash_value 						reroll_cost_value
	// ******

	// FUNCTION BODY[0x741e60]: 5
	// <0x741e63>|0x003|+0x013:'1408'
	// <0>
	// <0x741e76>|0x016|+0x00e:'1410'
	// <0x741e84>|0x024|+0x002:'1411'
	// <0x741e86>|0x026|+0x02d:'1412'
	// ******
}

// STATE[STUB]
void lobby_menu::fill_friend_list( )
{
	// LOCALS
	// vectora< account_list_item > const& players_list
	// flash_value 						array_value
	// flash_value 						value
	// u32 								i
	// wchar_t[512] 					player_name_w
	// flash_value 						list_item
	// ******

	// FUNCTION BODY[0x741ae0]: 32
	// <0x741ae8>|0x008|+0x075:'1417'
	// <0x741b5d>|0x07d|-0x059:'1417'
	// <0>
	// <0x741b04>|0x024|+0x02f:'1419'
	// <0>
	// <0x741b33>|0x053|+0x008:'1421'
	// <0x741b3b>|0x05b|+0x025:'1422'
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x741b60>|0x080|+0x024:'1427'
	// <0>
	// <0x741b84>|0x0a4|+0x028:'1429'
	// <0x741bac>|0x0cc|+0x038:'1430'
	// <0>
	// <1>
	// <2>
	// <0x741be4>|0x104|+0x022:'1434'
	// <0>
	// <0x741c06>|0x126|+0x071:'1436'
	// <0x741c77>|0x197|+0x02c:'1437'
	// <0>
	// <0x741ca3>|0x1c3|+0x024:'1439'
	// <0x741cc7>|0x1e7|+0x03f:'1440'
	// <0>
	// <0x741d06>|0x226|+0x034:'1442'
	// <0x741d3a>|0x25a|+0x034:'1443'
	// <0>
	// <0x741d6e>|0x28e|+0x01a:'1445'
	// <0>
	// <0x741d88>|0x2a8|+0x054:'1447'
	// <0x741ddc>|0x2fc|+0x021:'1448'
	// ******
}

// STATE[STUB]
void lobby_menu::fill_ignore_list( )
{
	// LOCALS
	// flash_value 						array_value
	// flash_value 						value
	// flash_value 						list_item
	// ******

	// FUNCTION BODY[0x741800]: 22
	// <0x741804>|0x004|+0x06b:'1453'
	// <0x74186f>|0x06f|-0x051:'1453'
	// <0>
	// <0x74181e>|0x01e|+0x02b:'1455'
	// <0>
	// <0x741849>|0x049|+0x008:'1457'
	// <0x741851>|0x051|+0x20a:'1458'
	// <0x741a5b>|0x25b|-0x1ea:'1458'
	// <0>
	// <1>
	// <0x741871>|0x071|+0x028:'1461'
	// <0>
	// <0x741899>|0x099|+0x02e:'1463'
	// <0x7418c7>|0x0c7|+0x03a:'1464'
	// <0>
	// <0x741901>|0x101|+0x06e:'1466'
	// <0x74196f>|0x16f|+0x02c:'1467'
	// <0>
	// <0x74199b>|0x19b|+0x028:'1469'
	// <0x7419c3>|0x1c3|+0x03e:'1470'
	// <0>
	// <0x741a01>|0x201|+0x016:'1472'
	// <0x741a17>|0x217|+0x048:'1473'
	// <0x741a5f>|0x25f|+0x021:'1474'
	// ******
}

// STATE[STUB]
void lobby_menu::fill_found_players( )
{
	// LOCALS
	// flash_value 						array_value
	// flash_value 						value
	// const u32 						count
	// u32 								i
	// flash_value 						list_item
	// ******

	// FUNCTION BODY[0x7415a0]: 16
	// <0x7415a6>|0x006|+0x06f:'1479'
	// <0x741615>|0x075|-0x057:'1479'
	// <0>
	// <0x7415be>|0x01e|+0x02b:'1481'
	// <0x7415e9>|0x049|+0x008:'1482'
	// <0x7415f1>|0x051|+0x01a:'1483'
	// <0x74160b>|0x06b|+0x00c:'1484'
	// <0>
	// <0x741617>|0x077|+0x024:'1486'
	// <0x74163b>|0x09b|+0x02a:'1487'
	// <0x741665>|0x0c5|+0x038:'1488'
	// <0>
	// <0x74169d>|0x0fd|+0x06a:'1490'
	// <0x741707>|0x167|+0x02c:'1491'
	// <0x741733>|0x193|+0x01a:'1492'
	// <0x74174d>|0x1ad|+0x037:'1493'
	// <0x741784>|0x1e4|+0x021:'1494'
	// ******
}

// STATE[STUB]
void lobby_menu::set_cursor( u8 id )
{
	// LOCALS
	// flash_value 						c_id
	// ******

	// FUNCTION BODY[0x741540]: 3
	// <0>
	// <0x741543>|0x003|+0x003:'1500'
	// <0x741546>|0x006|+0x036:'1501'
	// ******
}

// STATE[STUB]
void lobby_menu::set_fps_stats( float fps )
{
	// LOCALS
	// flash_value 						f_val
	// ******

	// FUNCTION BODY[0x7414d0]: 3
	// <0>
	// <0x7414d3>|0x003|+0x009:'1507'
	// <0x7414dc>|0x00c|+0x03a:'1508'
	// ******
}

// STATE[STUB]
bool lobby_menu::is_mouse_over_ui( )
{
	// LOCALS
	// flash_value 						is_mouse_over_val
	// ******

	return false;

	// FUNCTION BODY[0x741420]: 10
	// <0>
	// <0x74142b>|0x00b|+0x034:'1513'
	// <0>
	// <1>
	// <2>
	// <0x74145f>|0x03f|+0x02f:'1517'
	// <0>
	// <1>
	// <2>
	// <0x74148e>|0x06e|+0x02f:'1521'
	// ******
}

// STATE[STUB]
void lobby_menu::on_match_message_arrived( wchar_t const* w_text )
{
	// LOCALS
	// game_team_id 					team
	// wchar_t const* 					player_left_message
	// wchar_t const* 					queue_state_message
	// wchar_t[8] 						w_player_team
	// wchar_t[16] 						w_player_in_queue
	// wchar_t[32] 						w_player_name
	// flash_value[2] 					add_player_args
	// flash_value 						player_member_value
	// flash_value 						player_name_val
	// flash_value 						players_in_queue_val
	// ******

	// FUNCTION BODY[0x7410e0]: 63
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <0x7410f3>|0x013|+0x00b:'1537'
	// <0x7410fe>|0x01e|+0x00d:'1538'
	// <0x74110b>|0x02b|+0x00f:'1539'
	// <0>
	// <0x74111a>|0x03a|+0x011:'1541'
	// <0>
	// <0x74112b>|0x04b|+0x00b:'1543'
	// <0>
	// <0x741136>|0x056|+0x01b:'1545'
	// <0>
	// <0x741151>|0x071|+0x00d:'1547'
	// <0x74115e>|0x07e|+0x00b:'1548'
	// <0>
	// <1>
	// <0x741169>|0x089|+0x01b:'1551'
	// <0x741184>|0x0a4|+0x010:'1552'
	// <0>
	// <0x741194>|0x0b4|+0x017:'1554'
	// <0>
	// <0x7411ab>|0x0cb|+0x01c:'1556'
	// <0>
	// <1>
	// <2>
	// <0x7411c7>|0x0e7|+0x018:'1560'
	// <0x7411df>|0x0ff|+0x02a:'1561'
	// <0>
	// <0x741209>|0x129|+0x022:'1563'
	// <0x74122b>|0x14b|+0x03b:'1564'
	// <0>
	// <0x741266>|0x186|+0x029:'1566'
	// <0>
	// <0x74128f>|0x1af|+0x029:'1568'
	// <0>
	// <0x7412b8>|0x1d8|+0x03c:'1570'
	// <0x7412f4>|0x214|+0x008:'1571'
	// <0>
	// <0x7412fc>|0x21c|+0x00b:'1573'
	// <0>
	// <0x741307>|0x227|+0x01e:'1575'
	// <0>
	// <1>
	// <0x741325>|0x245|+0x018:'1578'
	// <0x74133d>|0x25d|+0x01f:'1579'
	// <0>
	// <0x74135c>|0x27c|+0x01f:'1581'
	// <0x74137b>|0x29b|+0x00c:'1582'
	// <0>
	// <0x741387>|0x2a7|+0x00b:'1584'
	// <0>
	// <1>
	// <0x741392>|0x2b2|+0x022:'1587'
	// <0>
	// <1>
	// <0x7413b4>|0x2d4|+0x015:'1590'
	// <0x7413c9>|0x2e9|+0x01f:'1591'
	// <0x7413e8>|0x308|+0x01f:'1592'
	// <0>
	// ******
}

// STATE[STUB]
void lobby_menu::on_stats_message_arrived(
	wchar_t const*						w_text,
	wchar_t const*						w_sender_name,
	messaging::message_channel_enum		message_channel
)
{
	// LOCALS
	// wchar_t[32] 						w_player_id
	// wchar_t[8] 						w_player_count
	// wchar_t[32] 						w_player_exp
	// wchar_t const* 					player_exp
	// char[32] 						player_name
	// flash_value 						player_count_val
	// ******

	// FUNCTION BODY[0x746480]: 68
	// <0x746480>|0x000|+0x018:'1609'	{
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <0x746498>|0x018|+0x00b:'1615'
	// <0x7464a3>|0x023|+0x00d:'1616'
	// <0>
	// <0x7464b0>|0x030|+0x00f:'1618'
	// <0x7464bf>|0x03f|+0x010:'1619'
	// <0>
	// <0x7464cf>|0x04f|+0x008:'1621'
	// <0>
	// <0x7464d7>|0x057|+0x00b:'1623'
	// <0>
	// <0x7464e2>|0x062|+0x018:'1625'
	// <0>
	// <1>
	// <0x7464fa>|0x07a|+0x020:'1628'
	// <0>
	// <0x74651a>|0x09a|+0x047:'1630'
	// <0>
	// <1>
	// <0x746561>|0x0e1|+0x023:'1633'
	// <0>
	// <0x746584>|0x104|+0x00c:'1635'
	// <0x746590>|0x110|+0x00b:'1636'
	// <0>
	// <0x74659b>|0x11b|+0x01b:'1638'
	// <0x7465b6>|0x136|+0x00d:'1639'
	// <0>
	// <0x7465c3>|0x143|+0x025:'1641'
	// <0>
	// <0x7465e8>|0x168|+0x01b:'1643'
	// <0x746603>|0x183|+0x04b:'1644'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <7>
	// <8>
	// <9>
	// <10>
	// <11>
	// <12>
	// <13>
	// <14>
	// <15>
	// <16>
	// <17>
	// <18>
	// <0x74664e>|0x1ce|+0x004:'1664'
	// <0>
	// <0x746652>|0x1d2|+0x00b:'1666'
	// <0>
	// <1>
	// <0x74665d>|0x1dd|+0x018:'1669'
	// <0>
	// <0x746675>|0x1f5|+0x005:'1671'
	// <0x74667a>|0x1fa|+0x015:'1672'
	// <0x74668f>|0x20f|+0x01f:'1673'
	// <0x7466ae>|0x22e|-0x090:'1674'
	// <0>
	// <1>
	// <0x74661e>|0x19e|+0x023:'1677'
	// <0x746641>|0x1c1|+0x08c:'1678'
	// <0x7466cd>|0x24d|      :'1678'	}
	// ******
}

// STATE[STUB]
void lobby_menu::show_disconnected_message( bool b_show )
{
	// LOCALS
	// wchar_t[512] 					message_txt
	// flash_value[4] 					v
	// flash_value 						window_id
	// ******

	// FUNCTION BODY[0x743b50]: 18
	// <0x743b50>|0x000|+0x00c:'1681'	{
	// <0x743b5c>|0x00c|+0x010:'1682'
	// <0>
	// <0x743b6c>|0x01c|+0x013:'1684'
	// <0x743b7f>|0x02f|+0x029:'1685'
	// <0x743ba8>|0x058|+0x01e:'1686'
	// <0>
	// <0x743bc6>|0x076|+0x01d:'1688'
	// <0x743be3>|0x093|+0x010:'1689'
	// <0x743bf3>|0x0a3|+0x029:'1690'
	// <0>
	// <0x743c1c>|0x0cc|+0x032:'1692'
	// <0x743c4e>|0x0fe|+0x01c:'1693'
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x743c6a>|0x11a|+0x03a:'1698'
	// <0x743ca4>|0x154|-0x043:'1699'
	// <0x743c61>|0x111|+0x064:'1700'
	// <0x743cc5>|0x175|      :'1700'	}
	// ******
}

// STATE[STUB]
void lobby_menu::on_player_reputations_arrived( )
{
	// LOCALS
	// u8 								reputation_id
	// u8 								player_reputation_level
	// flash_value[3] 					player_progress_args
	// char[32] 						faction_str
	// u8 								current_reputation_level
	// ******

	// FUNCTION BODY[0x743890]: 28
	// <0>
	// <0x743894>|0x004|+0x02c:'1705'
	// <0>
	// <0x7438c0>|0x030|+0x01e:'1707'
	// <0x7438de>|0x04e|+0x008:'1708'
	// <0>
	// <1>
	// <0x7438e6>|0x056|+0x012:'1711'
	// <0x7438f8>|0x068|+0x03f:'1712'
	// <0>
	// <0x743937>|0x0a7|+0x01e:'1714'
	// <0>
	// <0x743955>|0x0c5|+0x005:'1716'
	// <0>
	// <0x74395a>|0x0ca|+0x037:'1718'
	// <0x743991>|0x101|-0x02e:'1718'
	// <0>
	// <0x743963>|0x0d3|+0x015:'1720'
	// <0x743978>|0x0e8|+0x020:'1721'
	// <0>
	// <1>
	// <2>
	// <0x743998>|0x108|+0x01c:'1725'
	// <0x7439b4>|0x124|+0x025:'1726'
	// <0>
	// <0x7439d9>|0x149|+0x037:'1728'
	// <0>
	// <0x743a10>|0x180|+0x029:'1730'
	// <0x743a39>|0x1a9|+0x05b:'1731'
	// ******
}

// STATE[STUB]
void lobby_menu::set_ping( u32 ping_val )
{
	// LOCALS
	// flash_value 						args
	// ******

	// FUNCTION BODY[0x741080]: 3
	// <0>
	// <1>
	// <0x741083>|0x003|+0x036:'1738'
	// ******
}

} // namespace survarium
