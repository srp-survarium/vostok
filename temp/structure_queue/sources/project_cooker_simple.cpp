////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "vostok\game\sources\project_cooker_simple.h"

namespace survarium {

// STATE[STUB]
explicit project_cooker_simple::project_cooker_simple( bool editor_present )
{
	// FUNCTION BODY[0x5d74d0]: 1
	// <0x5d74ee>|0x01e|+0x04b:'48'
	// ******
}

// STATE[STUB]
void project_cooker_simple::translate_query( resources::query_result_for_cook& parent )
{
	// LOCALS
	// fs_new::virtual_path_string 		game_proj_path
	// fs_new::virtual_path_string 		project_name
	// ******

	// FUNCTION BODY[0x5d98a0]: 49
	// <0x5d98ad>|0x00d|+0x02b:'53'
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
	// <19>
	// <20>
	// <21>
	// <22>
	// <23>
	// <24>
	// <25>
	// <26>
	// <27>
	// <28>
	// <29>
	// <30>
	// <31>
	// <32>
	// <33>
	// <34>
	// <35>
	// <0x5d98d8>|0x038|+0x00a:'90'
	// <0x5d98e2>|0x042|+0x038:'91'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <7>
	// <0x5d991a>|0x07a|+0x094:'100'
	// <0>
	// ******
}

// STATE[STUB]
void project_cooker_simple::on_game_project_loaded( resources::queries_result& data, resources::query_result_for_cook* parent )
{
	// LOCALS
	// configs::binary_config_ptr 		game_proj_ptr
	// ******

	// FUNCTION BODY[0x5d9800]: 6
	// <0>
	// <1>
	// <2>
	// <0x5d9801>|0x001|+0x050:'150'
	// <0>
	// <0x5d9851>|0x051|+0x01e:'152'
	// ******
}

// STATE[STUB]
void project_cooker_simple::on_object_loaded(
	game_object_&							__formal,
	simple_game_project*					project,
	resources::query_result_for_cook*		parent_query
)
{
	// CALL SITE INFO
	// <0x5d7e8a> -> void < unknown >()
	// ******

	// FUNCTION BODY[0x5d7e30]: 8
	// <0x5d7e34>|0x004|+0x00a:'383'
	// <0x5d7e3e>|0x00e|+0x03d:'384'
	// <0>
	// <0x5d7e7b>|0x04b|+0x011:'386'
	// <0>
	// <0x5d7e8c>|0x05c|+0x024:'388'
	// <0x5d7eb0>|0x080|+0x00b:'389'
	// <0>
	// ******
}

// STATE[STUB]
void project_cooker_simple::create_game_objects(
	configs::binary_config_ptr				project_cfg,
	resources::query_result_for_cook*		parent_query
)
{
	// LOCALS
	// vector< variant< 32 > const* > 	ladders_user_data
	// configs::binary_config_value 	render_visuals
	// boost::function< void( game_object_& ) > callback
	// vector< resources::request > 	damage_zones_requests
	// simple_game_project* 			project
	// vector< resources::creation_request > ext_objects_requests
	// fs_new::virtual_path_string 		project_source_path
	// base_game_scene* 				base_game_scene
	// vector< variant< 32 > const* > 	damage_zones_user_data
	// configs::binary_config_value const* it_e
	// u32 								request_idx
	// vector< resources::request > 	ladders_requests
	// vector< resources::request > 	collision_and_render
	// configs::binary_config_value 	collision_objects
	// configs::binary_config_value const* it
	// collision_geometry* 				geometry
	// victory_items_container* 		container
	// resources::request 				r
	// variant< 32 >* 					ud
	// variant< 32 >* 					ud
	// resources::request 				request
	// ******

	// TYPEDEFS
	// typedef
	// 	vector< variant< 32 > const* >
	// 	user_data_type;

	// typedef
	// 	vector< resources::request >
	// 	requests_type;

	// ******

	// CALL SITE INFO
	// <0x5d8599> -> void < unknown >( configs::binary_config_value const&, pcstr, boost::function< void( game_object_& ) >& )
	// <0x5d8c60> -> void < unknown >( configs::binary_config_value const& )
	// <0x5d8ece> -> void < unknown >( configs::binary_config_value const& )
	// <0x5d9044> -> void < unknown >( configs::binary_config_value const& )
	// <0x5d93b0> -> void < unknown >( configs::binary_config_value const& )
	// <0x5d96b5> -> void < unknown >()
	// <0x5d979e> -> < unknown >
	// ******

	// OTHER SYMBOLS
	// Label(LabelSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x5c87f3 }, flags: ProcedureFlags { nofpo: false, int: false, far: false, never: false, notreached: true, cust_call: false, noinline: false, optdbginfo: false }, name: RawString("$LN2132") })
	// Label(LabelSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x5c83a7 }, flags: ProcedureFlags { nofpo: false, int: false, far: false, never: false, notreached: true, cust_call: false, noinline: false, optdbginfo: false }, name: RawString("$LN2138") })
	// ******

	// FUNCTION BODY[0x5d7ed0]: 299
	// <0>
	// <1>
	// <0x5d7ee2>|0x012|+0x05c:'397'
	// <0>
	// <1>
	// <2>
	// <0x5d7f3e>|0x06e|+0x022:'401'
	// <0x5d7f60>|0x090|+0x031:'402'
	// <0x5d7f91>|0x0c1|+0x00f:'403'
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x5d7fa0>|0x0d0|+0x051:'408'
	// <0x5d7ff1>|0x121|+0x02a:'409'
	// <0>
	// <0x5d801b>|0x14b|+0x026:'411'
	// <0>
	// <0x5d8041>|0x171|+0x03c:'413'
	// <0x5d807d>|0x1ad|-0x195:'414'
	// <0>
	// <0x5d7ee8>|0x018|+0x1bf:'416'
	// <0x5d80a7>|0x1d7|+0x078:'416'
	// <0x5d811f>|0x24f|-0x058:'416'
	// <0>
	// <0x5d80c7>|0x1f7|+0x033:'418'
	// <0>
	// <1>
	// <2>
	// <0x5d80fa>|0x22a|+0x019:'422'
	// <0>
	// <0x5d8113>|0x243|+0x004:'424'
	// <0>
	// <0x5d8117>|0x247|+0x00c:'426'
	// <0>
	// <1>
	// <0x5d8123>|0x253|+0x01c:'429'
	// <0>
	// <1>
	// <2>
	// <0x5d813f>|0x26f|+0x002:'433'
	// <0>
	// <1>
	// <0x5d8141>|0x271|+0x015:'436'
	// <0x5d8156>|0x286|+0x00e:'437'
	// <0x5d8164>|0x294|+0x019:'438'
	// <0x5d817d>|0x2ad|+0x02b:'439'
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x5d81a8>|0x2d8|+0x019:'444'
	// <0>
	// <1>
	// <0x5d81c1>|0x2f1|+0x012:'447'
	// <0>
	// <0x5d81d3>|0x303|+0x00e:'449'
	// <0>
	// <0x5d81e1>|0x311|+0x009:'451'
	// <0x5d81ea>|0x31a|+0x002:'452'
	// <0>
	// <1>
	// <0x5d81ec>|0x31c|+0x015:'455'
	// <0x5d8201>|0x331|+0x01b:'456'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <7>
	// <0x5d821c>|0x34c|+0x103:'465'
	// <0>
	// <1>
	// <0x5d831f>|0x44f|+0x013:'468'
	// <0x5d8332>|0x462|+0x025:'469'
	// <0>
	// <0x5d8357>|0x487|+0x0a9:'471'
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
	// <0x5d8400>|0x530|+0x070:'482'
	// <0>
	// <0x5d8470>|0x5a0|+0x010:'484'
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
	// <0x5d8480>|0x5b0|+0x034:'500'
	// <0>
	// <0x5d84b4>|0x5e4|+0x117:'502'
	// <0>
	// <0x5d85cb>|0x6fb|-0x05c:'504'
	// <0>
	// <0x5d856f>|0x69f|+0x090:'506'
	// <0x5d85ff>|0x72f|+0x0c0:'506'
	// <0>
	// <0x5d86bf>|0x7ef|+0x034:'508'
	// <0>
	// <0x5d86f3>|0x823|+0x0d7:'510'
	// <0>
	// <0x5d87ca>|0x8fa|+0x030:'512'
	// <0>
	// <0x5d87fa>|0x92a|+0x02b:'514'
	// <0>
	// <0x5d8825>|0x955|+0x034:'516'
	// <0>
	// <0x5d8859>|0x989|+0x02b:'518'
	// <0>
	// <0x5d8884>|0x9b4|+0x035:'520'
	// <0>
	// <0x5d88b9>|0x9e9|+0x050:'522'
	// <0>
	// <0x5d8909>|0xa39|+0x030:'524'
	// <0>
	// <0x5d8939>|0xa69|+0x049:'526'
	// <0>
	// <0x5d8982>|0xab2|+0x037:'528'
	// <0>
	// <0x5d89b9>|0xae9|+0x02b:'530'
	// <0>
	// <0x5d89e4>|0xb14|+0x035:'532'
	// <0>
	// <0x5d8a19>|0xb49|+0x02b:'534'
	// <0>
	// <0x5d8a44>|0xb74|+0x035:'536'
	// <0>
	// <0x5d8a79>|0xba9|+0x02b:'538'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <0x5d8aa4>|0xbd4|+0x014:'545'
	// <0>
	// <0x5d8ab8>|0xbe8|+0x021:'547'
	// <0x5d8ad9>|0xc09|+0x010:'548'
	// <0x5d8ae9>|0xc19|+0x03b:'549'
	// <0x5d8b24>|0xc54|+0x03c:'550'
	// <0>
	// <0x5d8b60>|0xc90|+0x005:'552'
	// <0x5d8b65>|0xc95|+0x010:'553'
	// <0>
	// <0x5d8b75>|0xca5|+0x02b:'555'
	// <0>
	// <0x5d8ba0>|0xcd0|+0x010:'557'
	// <0>
	// <0x5d8bb0>|0xce0|+0x02b:'559'
	// <0>
	// <0x5d8bdb>|0xd0b|+0x010:'561'
	// <0>
	// <0x5d8beb>|0xd1b|+0x02b:'563'
	// <0>
	// <0x5d8c16>|0xd46|+0x014:'565'
	// <0>
	// <0x5d8c2a>|0xd5a|+0x023:'567'
	// <0x5d8c4d>|0xd7d|+0x015:'568'
	// <0x5d8c62>|0xd92|+0x026:'569'
	// <0x5d8c88>|0xdb8|+0x036:'570'
	// <0x5d8cbe>|0xdee|+0x03f:'571'
	// <0x5d8cfd>|0xe2d|+0x005:'572'
	// <0x5d8d02>|0xe32|+0x014:'573'
	// <0>
	// <0x5d8d16>|0xe46|+0x037:'575'
	// <0x5d8d4d>|0xe7d|+0x00e:'576'
	// <0x5d8d5b>|0xe8b|+0x066:'577'
	// <0x5d8dc1>|0xef1|+0x005:'578'
	// <0x5d8dc6>|0xef6|+0x010:'579'
	// <0>
	// <1>
	// <0x5d8dd6>|0xf06|+0x017:'582'
	// <0x5d8ded>|0xf1d|+0x01e:'583'
	// <0>
	// <0x5d8e0b>|0xf3b|+0x01b:'585'
	// <0x5d8e26>|0xf56|+0x012:'586'
	// <0x5d8e38>|0xf68|+0x013:'587'
	// <0x5d8e4b>|0xf7b|+0x005:'588'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <0x5d8e50>|0xf80|+0x010:'596'
	// <0>
	// <0x5d8e60>|0xf90|+0x02b:'598'
	// <0>
	// <0x5d8e8b>|0xfbb|+0x014:'600'
	// <0>
	// <0x5d8e9f>|0xfcf|+0x023:'602'
	// <0x5d8ec2>|0xff2|+0x00e:'603'
	// <0x5d8ed0>|0x1000|+0x026:'604'
	// <0x5d8ef6>|0x1026|+0x036:'605'
	// <0x5d8f2c>|0x105c|+0x046:'606'
	// <0x5d8f72>|0x10a2|+0x005:'607'
	// <0x5d8f77>|0x10a7|+0x010:'608'
	// <0>
	// <1>
	// <0x5d8f87>|0x10b7|+0x017:'611'
	// <0x5d8f9e>|0x10ce|+0x01b:'612'
	// <0>
	// <0x5d8fb9>|0x10e9|+0x01b:'614'
	// <0x5d8fd4>|0x1104|+0x012:'615'
	// <0x5d8fe6>|0x1116|+0x013:'616'
	// <0x5d8ff9>|0x1129|+0x005:'617'
	// <0x5d8ffe>|0x112e|+0x014:'618'
	// <0>
	// <0x5d9012>|0x1142|+0x026:'620'
	// <0x5d9038>|0x1168|+0x00e:'621'
	// <0x5d9046>|0x1176|+0x032:'622'
	// <0x5d9078>|0x11a8|+0x043:'623'
	// <0x5d90bb>|0x11eb|+0x005:'624'
	// <0>
	// <0x5d90c0>|0x11f0|-0xb4b:'626'
	// <0>
	// <0x5d8575>|0x6a5|+0xc14:'628'
	// <0x5d9189>|0x12b9|-0xc0c:'628'
	// <0>
	// <0x5d857d>|0x6ad|+0x01e:'630'
	// <0x5d859b>|0x6cb|+0xbf3:'631'
	// <0x5d918e>|0x12be|+0x20f:'631'
	// <0x5d939d>|0x14cd|-0x073:'631'
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x5d932a>|0x145a|+0x01b:'636'
	// <0>
	// <0x5d9345>|0x1475|+0x013:'638'
	// <0x5d9358>|0x1488|+0x025:'639'
	// <0>
	// <0x5d937d>|0x14ad|+0x051:'641'
	// <0x5d93ce>|0x14fe|-0x04d:'641'
	// <0>
	// <0x5d9381>|0x14b1|+0x026:'643'
	// <0x5d93a7>|0x14d7|+0x002:'643'
	// <0x5d93a9>|0x14d9|+0x009:'644'
	// <0x5d93b2>|0x14e2|+0x020:'645'
	// <0>
	// <1>
	// <2>
	// <0x5d93d2>|0x1502|+0x00e:'649'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <0x5d93e0>|0x1510|+0x0d6:'656'
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x5d94b6>|0x15e6|+0x01f:'661'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <7>
	// <0x5d94d5>|0x1605|+0x0df:'670'
	// <0x5d95b4>|0x16e4|+0x00a:'671'
	// <0>
	// <1>
	// <0x5d95be>|0x16ee|+0x00a:'674'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <7>
	// <0x5d95c8>|0x16f8|+0x0a1:'683'
	// <0x5d9669>|0x1799|+0x00a:'684'
	// <0>
	// <1>
	// <0x5d9673>|0x17a3|+0x02f:'687'
	// <0>
	// <0x5d96a2>|0x17d2|+0x015:'689'
	// <0>
	// <0x5d96b7>|0x17e7|+0x01f:'691'
	// <0x5d96d6>|0x1806|+0x00a:'692'
	// <0>
	// ******
}

// STATE[STUB]
void project_cooker_simple::delete_resource( resources::resource_base* resource )
{
	// LOCALS
	// collision_geometry** 			geom_end
	// victory_items_container** 		vc_it_e
	// game_object_** 					it_e
	// ******

	// CALL SITE INFO
	// <0x5d757f> -> void* < unknown >( u32 )
	// <0x5d7643> -> void* < unknown >( u32 )
	// <0x5d76a0> -> void* < unknown >( u32 )
	// ******

	// FUNCTION BODY[0x5d7540]: 40
	// <0>
	// <0x5d7542>|0x002|+0x004:'699'
	// <0x5d7546>|0x006|+0x013:'700'
	// <0>
	// <0x5d7559>|0x019|+0x03e:'702'
	// <0x5d7597>|0x057|-0x037:'702'
	// <0>
	// <0x5d7560>|0x020|+0x002:'704'
	// <0x5d7562>|0x022|+0x03e:'705'
	// <0>
	// <1>
	// <0x5d75a0>|0x060|+0x006:'708'
	// <0x5d75a6>|0x066|+0x006:'709'
	// <0>
	// <0x5d75ac>|0x06c|+0x009:'711'
	// <0>
	// <0x5d75b5>|0x075|+0x003:'713'
	// <0x5d75b8>|0x078|+0x037:'714'
	// <0>
	// <0x5d75ef>|0x0af|+0x01f:'716'
	// <0>
	// <1>
	// <0x5d760e>|0x0ce|+0x010:'719'
	// <0>
	// <0x5d761e>|0x0de|+0x03d:'721'
	// <0x5d765b>|0x11b|-0x039:'721'
	// <0>
	// <0x5d7622>|0x0e2|+0x003:'723'
	// <0x5d7625>|0x0e5|+0x03f:'724'
	// <0>
	// <1>
	// <2>
	// <0x5d7664>|0x124|+0x010:'728'
	// <0>
	// <0x5d7674>|0x134|+0x044:'730'
	// <0x5d76b8>|0x178|-0x038:'730'
	// <0>
	// <0x5d7680>|0x140|+0x003:'732'
	// <0x5d7683>|0x143|+0x03e:'733'
	// <0>
	// <0x5d76c1>|0x181|+0x03a:'735'
	// <0>
	// <0x5d76fb>|0x1bb|+0x031:'737'
	// ******
}

// STATE[STUB]
void project_cooker_simple::on_damage_zones_loaded( simple_game_project* project, resources::queries_result& data )
{
	// LOCALS
	// u32 								i
	// damage_zone_ptr 					zone
	// configs::binary_config_value 	cfg
	// ******

	// CALL SITE INFO
	// <0x5d7dea> -> void < unknown >()
	// ******

	// FUNCTION BODY[0x5d7c00]: 24
	// <0>
	// <0x5d7c0a>|0x00a|+0x026:'743'
	// <0x5d7c30>|0x030|+0x176:'743'
	// <0x5d7da6>|0x1a6|-0x172:'743'
	// <0>
	// <0x5d7c34>|0x034|+0x049:'745'
	// <0x5d7c7d>|0x07d|-0x06d:'746'
	// <0>
	// <0x5d7c10>|0x010|+0x0b1:'748'
	// <0x5d7cc1>|0x0c1|+0x003:'748'
	// <0>
	// <1>
	// <0x5d7cc4>|0x0c4|+0x020:'751'
	// <0x5d7ce4>|0x0e4|+0x013:'752'
	// <0>
	// <0x5d7cf7>|0x0f7|+0x041:'754'
	// <0x5d7d38>|0x138|+0x036:'755'
	// <0x5d7d6e>|0x16e|+0x03a:'756'
	// <0>
	// <0x5d7da8>|0x1a8|+0x003:'758'
	// <0x5d7dab>|0x1ab|+0x030:'759'
	// <0>
	// <0x5d7ddb>|0x1db|+0x011:'761'
	// <0>
	// <0x5d7dec>|0x1ec|+0x02c:'763'
	// <0x5d7e18>|0x218|+0x00e:'764'
	// <0>
	// ******
}

// STATE[STUB]
void project_cooker_simple::on_ladders_loaded( simple_game_project* project, resources::queries_result& data )
{
	// LOCALS
	// u32 								i
	// configs::binary_config_value 	cfg
	// ladder_ptr 						new_ladder
	// ******

	// CALL SITE INFO
	// <0x5d7bba> -> void < unknown >()
	// ******

	// FUNCTION BODY[0x5d79d0]: 25
	// <0>
	// <0x5d79da>|0x00a|+0x026:'771'
	// <0x5d7a00>|0x030|+0x176:'771'
	// <0x5d7b76>|0x1a6|-0x172:'771'
	// <0>
	// <0x5d7a04>|0x034|+0x049:'773'
	// <0x5d7a4d>|0x07d|-0x06d:'774'
	// <0>
	// <0x5d79e0>|0x010|+0x0b1:'776'
	// <0x5d7a91>|0x0c1|+0x003:'776'
	// <0>
	// <1>
	// <0x5d7a94>|0x0c4|+0x020:'779'
	// <0x5d7ab4>|0x0e4|+0x013:'780'
	// <0>
	// <0x5d7ac7>|0x0f7|+0x041:'782'
	// <0x5d7b08>|0x138|+0x036:'783'
	// <0x5d7b3e>|0x16e|+0x03a:'784'
	// <0>
	// <0x5d7b78>|0x1a8|+0x003:'786'
	// <0>
	// <0x5d7b7b>|0x1ab|+0x030:'788'
	// <0>
	// <0x5d7bab>|0x1db|+0x011:'790'
	// <0>
	// <0x5d7bbc>|0x1ec|+0x02c:'792'
	// <0x5d7be8>|0x218|+0x00e:'793'
	// <0>
	// ******
}

// STATE[STUB]
void project_cooker_simple::on_collision_and_visuals_loaded( resources::queries_result& data, simple_game_project* project )
{
	// LOCALS
	// u32 								request_idx
	// u32 								i
	// u32 								i
	// ******

	// CALL SITE INFO
	// <0x5d7987> -> void < unknown >()
	// ******

	// FUNCTION BODY[0x5d7730]: 24
	// <0>
	// <1>
	// <2>
	// <0x5d7735>|0x005|+0x02d:'802'
	// <0x5d7762>|0x032|+0x0df:'802'
	// <0x5d7841>|0x111|-0x0db:'802'
	// <0>
	// <0x5d7766>|0x036|-0x00d:'804'
	// <0x5d7759>|0x029|+0x017:'805'
	// <0x5d7770>|0x040|+0x0d5:'805'
	// <0>
	// <1>
	// <0x5d7845>|0x115|+0x101:'808'
	// <0x5d7946>|0x216|-0x0d2:'808'
	// <0>
	// <0x5d7874>|0x144|+0x00a:'810'
	// <0x5d787e>|0x14e|+0x0cc:'811'
	// <0>
	// <0x5d794a>|0x21a|+0x007:'813'
	// <0x5d7951>|0x221|+0x007:'814'
	// <0>
	// <0x5d7958>|0x228|+0x020:'816'
	// <0>
	// <0x5d7978>|0x248|+0x011:'818'
	// <0>
	// <0x5d7989>|0x259|+0x02d:'820'
	// <0x5d79b6>|0x286|+0x00e:'821'
	// <0>
	// ******
}

	// TYPEDEFS
	// typedef
	// 	pcvoid*
	// 	iterator_type;

	// typedef
	// 	survarium::base_project::resolve_link_object*
	// 	iterator_type;

	// typedef
	// 	survarium::damage_zone_ptr*
	// 	iterator_type;

	// typedef
	// 	survarium::ladder_ptr*
	// 	iterator_type;

	// typedef
	// 	survarium::scheduler::record*
	// 	iterator_type;

	// typedef
	// 	survarium::zone_group::zone_wrapper*
	// 	iterator_type;

	// typedef
	// 	void**
	// 	iterator_type;

	// typedef
	// 	vostok::collision::bone_collision_data const*
	// 	iterator_type;

	// typedef
	// 	vostok::collision::bone_collision_data*
	// 	iterator_type;

	// typedef
	// 	vostok::resources::creation_request*
	// 	iterator_type;

	// typedef
	// 	vostok::resources::request*
	// 	iterator_type;

	// ******

} // namespace survarium
