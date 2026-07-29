////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "vostok\render\engine\sources\render_model_cooker.h"

namespace vostok {
namespace render {

// STATE[STUB]
 user_mesh_cook::user_mesh_cook( )
{
	// FUNCTION BODY[0x650df0]: 1
	// <0x650e26>|0x036|+0x037:'34'
	// ******
}

// STATE[STUB]
mutable_buffer user_mesh_cook::allocate_resource(
	resources::query_result_for_cook&		in_query,
	const_buffer							raw_file_data,
	bool									file_exist
)
{
	// FUNCTION BODY[0x650c10]: 2
	// <0>
	// <0x650c11>|0x001|+0x012:'42'
	// ******
}

// STATE[STUB]
void user_mesh_cook::deallocate_resource( void* buffer )
{
	// FUNCTION BODY[0x650c00]: 1
	// <0>
	// ******
}

// STATE[STUB]
void user_mesh_cook::create_resource(
	resources::query_result_for_cook&		in_out_query,
	const_buffer							raw_file_data,
	mutable_buffer							in_out_unmanaged_resource_buffer
)
{
	// LOCALS
	// memory::chunk_reader 			chunk
	// ******

	// CALL SITE INFO
	// <0x65227f> -> void < unknown >( memory::chunk_reader& )
	// ******

	// FUNCTION BODY[0x652200]: 15
	// <0>
	// <0x65220e>|0x00e|+0x01e:'55'
	// <0>
	// <0x65222c>|0x02c|+0x042:'57'
	// <0>
	// <0x65226e>|0x06e|+0x007:'59'
	// <0>
	// <0x652275>|0x075|+0x00c:'61'
	// <0>
	// <0x652281>|0x081|+0x01f:'63'
	// <0>
	// <1>
	// <2>
	// <0x6522a0>|0x0a0|+0x047:'67'
	// <0x6522e7>|0x0e7|+0x00a:'68'
	// ******
}

// STATE[STUB]
void user_mesh_cook::destroy_resource( resources::unmanaged_resource* resource )
{
	// FUNCTION BODY[0x650f30]: 6
	// <0>
	// <1>
	// <2>
	// <0x650f31>|0x001|+0x03f:'76'
	// <0>
	// <0x650f70>|0x040|+0x030:'78'
	// ******
}

// STATE[STUB]
 static_render_model_instance_cook::static_render_model_instance_cook( )
{
	// FUNCTION BODY[0x650d80]: 1
	// <0x650db8>|0x038|+0x029:'84'
	// ******
}

// STATE[STUB]
void static_render_model_instance_cook::translate_query( resources::query_result_for_cook& parent )
{
	// LOCALS
	// fs_new::virtual_path_string 		render_path
	// ******

	// FUNCTION BODY[0x652e20]: 7
	// <0x652e2e>|0x00e|+0x027:'90'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <0x652e55>|0x035|+0x074:'96'
	// ******
}

// STATE[STUB]
void static_render_model_instance_cook::on_sub_resources_loaded( resources::queries_result& data )
{
	// LOCALS
	// static_render_model_instance* 	created_resource
	// resources::query_result_for_cook* parent
	// configs::binary_config_value 	cfg
	// ******

	// FUNCTION BODY[0x651b20]: 23
	// <0x651b29>|0x009|+0x003:'101'
	// <0>
	// <0x651b2c>|0x00c|+0x02e:'103'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <0x651b5a>|0x03a|-0x026:'110'
	// <0>
	// <0x651b34>|0x014|+0x063:'112'
	// <0x651b97>|0x077|+0x02d:'112'
	// <0x651bc4>|0x0a4|+0x009:'113'
	// <0>
	// <0x651bcd>|0x0ad|+0x009:'115'
	// <0x651bd6>|0x0b6|+0x013:'116'
	// <0>
	// <0x651be9>|0x0c9|+0x02e:'118'
	// <0>
	// <0x651c17>|0x0f7|+0x027:'120'
	// <0>
	// <0x651c3e>|0x11e|+0x024:'122'
	// <0x651c62>|0x142|-0x111:'123'
	// <0x651b51>|0x031|+0x11e:'124'
	// ******
}

// STATE[STUB]
void static_render_model_instance_cook::delete_resource( resources::resource_base* resource )
{
	// CALL SITE INFO
	// <0x650f14> -> void* < unknown >( u32 )
	// ******

	// FUNCTION BODY[0x650ef0]: 1
	// <0x650ef1>|0x001|+0x03a:'128'
	// ******
}

// STATE[STUB]
 skeleton_render_model_instance_cook::skeleton_render_model_instance_cook( )
{
	// FUNCTION BODY[0x650d10]: 1
	// <0x650d48>|0x038|+0x029:'134'
	// ******
}

// STATE[STUB]
void skeleton_render_model_instance_cook::translate_query( resources::query_result_for_cook& parent )
{
	// LOCALS
	// fs_new::virtual_path_string 		render_path
	// ******

	// FUNCTION BODY[0x652d60]: 7
	// <0x652d6e>|0x00e|+0x027:'139'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <0x652d95>|0x035|+0x074:'145'
	// ******
}

// STATE[STUB]
void skeleton_render_model_instance_cook::on_sub_resources_loaded( resources::queries_result& data )
{
	// FUNCTION BODY[0x651a10]: 14
	// <0x651a13>|0x003|+0x004:'150'
	// <0>
	// <0x651a17>|0x007|+0x02c:'152'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <0x651a43>|0x033|+0x03f:'158'
	// <0x651a82>|0x072|-0x061:'159'
	// <0x651a21>|0x011|+0x07e:'160'
	// <0x651a9f>|0x08f|+0x023:'160'
	// <0>
	// <0x651ac2>|0x0b2|+0x020:'162'
	// <0x651ae2>|0x0d2|-0x0a9:'163'
	// <0x651a39>|0x029|+0x0b6:'164'
	// ******
}

// STATE[STUB]
void skeleton_render_model_instance_cook::delete_resource( resources::resource_base* resource )
{
	// CALL SITE INFO
	// <0x650ed4> -> void* < unknown >( u32 )
	// ******

	// FUNCTION BODY[0x650eb0]: 1
	// <0x650eb1>|0x001|+0x03a:'168'
	// ******
}

// STATE[STUB]
 cook_intermediate_data::cook_intermediate_data(
	fs_new::virtual_path_string const&		in_resource_path,
	resources::query_result_for_cook*		in_query_result
)
{
	// FUNCTION BODY[0x651980]: 0
	// ******
}

// STATE[STUB]
s32 cook_intermediate_data::find_surface_index( pcstr surface_name )
{
	return 0;

	// FUNCTION BODY[0x650ca0]: 6
	// <0x650ca8>|0x008|+0x018:'189'
	// <0>
	// <0x650cc0>|0x020|+0x038:'191'
	// <0>
	// <1>
	// <0x650cf8>|0x058|+0x007:'194'
	// ******
}

// STATE[STUB]
s32 cook_intermediate_data::find_material_index( pcstr surface_name )
{
	return 0;

	// FUNCTION BODY[0x650fb0]: 6
	// <0x650fb7>|0x007|+0x01e:'199'
	// <0x650fd5>|0x025|+0x0af:'200'
	// <0x651084>|0x0d4|-0x05d:'201'
	// <0>
	// <0x651027>|0x077|+0x068:'203'
	// <0x65108f>|0x0df|-0x009:'203'
	// <0>
	// <0x651086>|0x0d6|+0x07d:'205'
	// ******
}

// STATE[STUB]
void cook_intermediate_data::register_models( vfs::vfs_iterator const& fs_it )
{
	// LOCALS
	// vfs::vfs_iterator 				end
	// vfs::vfs_iterator 				it
	// ******

	// FUNCTION BODY[0x651830]: 25
	// <0>
	// <1>
	// <2>
	// <0x65183a>|0x00a|+0x007:'212'
	// <0x651841>|0x011|+0x014:'213'
	// <0>
	// <1>
	// <0x651855>|0x025|+0x00b:'216'
	// <0x651860>|0x030|+0x00b:'217'
	// <0>
	// <0x65186b>|0x03b|+0x00e:'219'
	// <0>
	// <0x651879>|0x049|+0x017:'221'
	// <0>
	// <0x651890>|0x060|+0x00c:'223'
	// <0>
	// <0x65189c>|0x06c|+0x012:'225'
	// <0x6518ae>|0x07e|+0x022:'226'
	// <0>
	// <1>
	// <2>
	// <0x6518d0>|0x0a0|+0x044:'230'
	// <0x651914>|0x0e4|-0x00a:'230'
	// <0>
	// <0x65190a>|0x0da|+0x00d:'232'
	// <0x651917>|0x0e7|+0x057:'233'
	// ******
}

// STATE[STUB]
 grass_render_model_cook::grass_render_model_cook( )
{
	// FUNCTION BODY[0x650c80]: 0
	// ******
}

// STATE[STUB]
void grass_render_model_cook::translate_query( resources::query_result_for_cook& parent )
{
	// LOCALS
	// fs_new::virtual_path_string 		model_path
	// fs_new::virtual_path_string 		render_path
	// ******

	// FUNCTION BODY[0x653880]: 22
	// <0x65388d>|0x00d|+0x017:'243'
	// <0>
	// <0x6538a4>|0x024|+0x006:'245'
	// <0x6538aa>|0x02a|+0x030:'246'
	// <0>
	// <0x6538da>|0x05a|+0x029:'248'
	// <0>
	// <0x653903>|0x083|+0x010:'250'
	// <0>
	// <1>
	// <2>
	// <0x653913>|0x093|+0x03c:'254'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <7>
	// <8>
	// <0x65394f>|0x0cf|+0x0a1:'264'
	// ******
}

// STATE[STUB]
explicit render_model_cook::render_model_cook( resources::class_id_enum model_type )
{
	// FUNCTION BODY[0x650c30]: 1
	// <0x650c66>|0x036|+0x014:'271'
	// ******
}

// STATE[STUB]
void render_model_cook::translate_query( resources::query_result_for_cook& parent )
{
	// LOCALS
	// fs_new::virtual_path_string 		model_path
	// fs_new::virtual_path_string 		render_path
	// ******

	// FUNCTION BODY[0x6536f0]: 21
	// <0x6536fe>|0x00e|+0x02c:'276'
	// <0x65372a>|0x03a|+0x027:'277'
	// <0>
	// <0x653751>|0x061|+0x00c:'279'
	// <0>
	// <0x65375d>|0x06d|+0x029:'281'
	// <0>
	// <0x653786>|0x096|+0x00a:'283'
	// <0>
	// <1>
	// <2>
	// <0x653790>|0x0a0|+0x033:'287'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <7>
	// <0x6537c3>|0x0d3|+0x0a8:'296'
	// ******
}

// STATE[STUB]
void render_model_cook::on_fs_iterator_ready_submeshes( cook_intermediate_data* cook_data, vfs::vfs_locked_iterator const& fs_it )
{
	// LOCALS
	// u32 								model_index
	// u32 								num_requests
	// vfs::vfs_iterator 				it_e
	// fs_new::virtual_path_string 		render_dir
	// vfs::vfs_iterator 				it
	// pcstr 							sname
	// ******

	// FUNCTION BODY[0x653320]: 79
	// <0x65332e>|0x00e|+0x053:'301'
	// <0x653381>|0x061|+0x04c:'302'
	// <0>
	// <0x6533cd>|0x0ad|+0x00b:'304'
	// <0>
	// <0x6533d8>|0x0b8|+0x00a:'306'
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x6533e2>|0x0c2|+0x01e:'311'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <0x653400>|0x0e0|+0x00b:'318'
	// <0x65340b>|0x0eb|+0x005:'319'
	// <0>
	// <1>
	// <2>
	// <0x653410>|0x0f0|+0x006:'323'
	// <0>
	// <0x653416>|0x0f6|+0x02f:'325'
	// <0x653445>|0x125|+0x034:'326'
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x653479>|0x159|+0x01f:'331'
	// <0x653498>|0x178|+0x007:'332'
	// <0x65349f>|0x17f|+0x005:'333'
	// <0>
	// <1>
	// <0x6534a4>|0x184|+0x014:'336'
	// <0x6534b8>|0x198|+0x00d:'337'
	// <0>
	// <0x6534c5>|0x1a5|+0x0f7:'339'
	// <0x6535bc>|0x29c|-0x0cc:'339'
	// <0>
	// <0x6534f0>|0x1d0|+0x011:'341'
	// <0>
	// <1>
	// <0x653501>|0x1e1|+0x009:'344'
	// <0x65350a>|0x1ea|+0x02b:'345'
	// <0>
	// <0x653535>|0x215|+0x014:'347'
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x653549>|0x229|+0x02c:'352'
	// <0>
	// <1>
	// <2>
	// <0x653575>|0x255|+0x04b:'356'
	// <0>
	// <1>
	// <0x6535c0>|0x2a0|+0x00a:'359'
	// <0>
	// <0x6535ca>|0x2aa|+0x02b:'361'
	// <0>
	// <0x6535f5>|0x2d5|+0x014:'363'
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
	// <0x653609>|0x2e9|+0x0a5:'376'
	// <0>
	// <0x6536ae>|0x38e|+0x019:'378'
	// <0x6536c7>|0x3a7|-0x2d0:'379'
	// <0x6533f7>|0x0d7|+0x2e5:'380'
	// ******
}

// STATE[STUB]
void render_model_cook::on_model_settings_loaded( resources::queries_result& data, cook_intermediate_data* cook_data )
{
	// LOCALS
	// configs::binary_config_value 	msettings
	// u32 								num_render_models
	// configs::binary_config_value 	root
	// pcstr 							material_name
	// pcstr 							sname
	// ******

	// FUNCTION BODY[0x6528b0]: 74
	// <0x6528bc>|0x00c|+0x018:'386'
	// <0>
	// <0x6528d4>|0x024|+0x003:'388'
	// <0x6528d7>|0x027|+0x014:'389'
	// <0x6528eb>|0x03b|+0x010:'390'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <0x6528fb>|0x04b|+0x072:'396'
	// <0x65296d>|0x0bd|+0x022:'397'
	// <0>
	// <1>
	// <0x65298f>|0x0df|+0x014:'400'
	// <0x6529a3>|0x0f3|+0x02c:'401'
	// <0x6529cf>|0x11f|+0x008:'402'
	// <0>
	// <0x6529d7>|0x127|+0x03d:'404'
	// <0>
	// <0x652a14>|0x164|+0x018:'406'
	// <0x652a2c>|0x17c|+0x01c:'407'
	// <0>
	// <1>
	// <0x652a48>|0x198|+0x006:'410'
	// <0>
	// <0x652a4e>|0x19e|+0x0bf:'412'
	// <0x652b0d>|0x25d|-0x0b5:'412'
	// <0>
	// <0x652a58>|0x1a8|+0x00e:'414'
	// <0x652a66>|0x1b6|+0x0a9:'415'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <7>
	// <8>
	// <0x652b0f>|0x25f|+0x0c7:'425'
	// <0>
	// <1>
	// <2>
	// <0x652bd6>|0x326|+0x01b:'429'
	// <0x652bf1>|0x341|-0x11a:'430'
	// <0>
	// <1>
	// <2>
	// <0x652ad7>|0x227|-0x04b:'434'
	// <0>
	// <0x652a8c>|0x1dc|+0x03a:'436'
	// <0>
	// <0x652ac6>|0x216|+0x19b:'438'
	// <0x652c61>|0x3b1|-0x186:'438'
	// <0>
	// <0x652adb>|0x22b|+0x00e:'440'
	// <0x652ae9>|0x239|+0x11c:'441'
	// <0x652c05>|0x355|+0x014:'441'
	// <0x652c19>|0x369|+0x01c:'442'
	// <0x652c35>|0x385|+0x011:'443'
	// <0x652c46>|0x396|+0x00a:'444'
	// <0x652c50>|0x3a0|+0x019:'445'
	// <0x652c69>|0x3b9|+0x013:'445'
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
	// <0x652c7c>|0x3cc|+0x0b9:'457'
	// <0>
	// <0x652d35>|0x485|-0x443:'459'
	// <0x6528f2>|0x042|+0x30a:'460'
	// <0x652bfc>|0x34c|+0x14e:'460'
	// ******
}

// STATE[STUB]
void render_model_cook::on_subresources_loaded( resources::queries_result& data, cook_intermediate_data* cook_data )
{
	// LOCALS
	// configs::binary_config_ptr 		config_ptr
	// render_model* 					model
	// u32 								request_index
	// fs_new::virtual_path_string 		material_settings_path
	// resources::pinned_ptr_const< u8 > bind_pose_ptr
	// memory::reader 					bones_reader
	// ******

	// CALL SITE INFO
	// <0x652fa5> -> void < unknown >( configs::binary_config_value const& )
	// ******

	// FUNCTION BODY[0x652ee0]: 66
	// <0x652eec>|0x00c|+0x012:'466'
	// <0>
	// <0x652efe>|0x01e|+0x003:'468'
	// <0x652f01>|0x021|+0x017:'469'
	// <0>
	// <1>
	// <2>
	// <0x652f18>|0x038|+0x00a:'473'
	// <0>
	// <1>
	// <2>
	// <0x652f22>|0x042|+0x042:'477'
	// <0x652f64>|0x084|+0x00a:'478'
	// <0>
	// <1>
	// <0x652f6e>|0x08e|+0x014:'481'
	// <0>
	// <0x652f82>|0x0a2|+0x00d:'483'
	// <0x652f8f>|0x0af|+0x005:'484'
	// <0>
	// <0x652f94>|0x0b4|+0x04c:'486'
	// <0x652fe0>|0x100|-0x047:'486'
	// <0>
	// <1>
	// <0x652f99>|0x0b9|+0x00e:'489'
	// <0>
	// <0x652fa7>|0x0c7|+0x00f:'491'
	// <0>
	// <0x652fb6>|0x0d6|+0x02e:'493'
	// <0>
	// <1>
	// <2>
	// <0x652fe4>|0x104|+0x05e:'497'
	// <0x653042>|0x162|+0x008:'498'
	// <0>
	// <0x65304a>|0x16a|+0x0cc:'500'
	// <0x653116>|0x236|+0x020:'501'
	// <0>
	// <1>
	// <0x653136>|0x256|+0x00e:'504'
	// <0>
	// <0x653144>|0x264|+0x064:'506'
	// <0x6531a8>|0x2c8|+0x008:'507'
	// <0>
	// <1>
	// <0x6531b0>|0x2d0|+0x019:'510'
	// <0x6531c9>|0x2e9|+0x009:'511'
	// <0>
	// <1>
	// <2>
	// <0x6531d2>|0x2f2|+0x010:'515'
	// <0>
	// <0x6531e2>|0x302|+0x007:'517'
	// <0>
	// <1>
	// <0x6531e9>|0x309|+0x010:'520'
	// <0x6531f9>|0x319|+0x03a:'521'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <7>
	// <0x653233>|0x353|-0x324:'530'
	// <0>
	// <0x652f0f>|0x02f|+0x3e8:'532'
	// ******
}

// STATE[STUB]
void render_model_cook::on_materials_loaded( resources::queries_result& data, cook_intermediate_data* cook_data )
{
	// LOCALS
	// u32 								i
	// ******

	// FUNCTION BODY[0x652790]: 16
	// <0x65279c>|0x00c|+0x010:'536'
	// <0>
	// <1>
	// <0x6527ac>|0x01c|+0x006:'539'
	// <0>
	// <1>
	// <0x6527b2>|0x022|+0x0b4:'542'
	// <0x652866>|0x0d6|-0x091:'542'
	// <0>
	// <0x6527d5>|0x045|+0x014:'544'
	// <0x6527e9>|0x059|+0x0a4:'545'
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x65288d>|0x0fd|+0x010:'550'
	// <0x65289d>|0x10d|+0x007:'551'
	// ******
}

// STATE[STUB]
enum_vertex_input_type mesh_type_to_vertex_input_type( mesh_type_enum type )
{
	// STATICS
	// static < NoType > 				 = <0x650b34>;
	// static < NoType > 				 = <0x650b10>;
	// ******

	// OTHER SYMBOLS
	// Label(LabelSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x63fade }, flags: ProcedureFlags { nofpo: false, int: false, far: false, never: false, notreached: false, cust_call: false, noinline: false, optdbginfo: false }, name: RawString("$LN9") })
	// Label(LabelSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x63fae4 }, flags: ProcedureFlags { nofpo: false, int: false, far: false, never: false, notreached: false, cust_call: false, noinline: false, optdbginfo: false }, name: RawString("$LN8") })
	// Label(LabelSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x63faea }, flags: ProcedureFlags { nofpo: false, int: false, far: false, never: false, notreached: false, cust_call: false, noinline: false, optdbginfo: false }, name: RawString("$LN7") })
	// Label(LabelSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x63faf0 }, flags: ProcedureFlags { nofpo: false, int: false, far: false, never: false, notreached: false, cust_call: false, noinline: false, optdbginfo: false }, name: RawString("$LN6") })
	// Label(LabelSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x63faf6 }, flags: ProcedureFlags { nofpo: false, int: false, far: false, never: false, notreached: false, cust_call: false, noinline: false, optdbginfo: false }, name: RawString("$LN5") })
	// Label(LabelSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x63fafc }, flags: ProcedureFlags { nofpo: false, int: false, far: false, never: false, notreached: false, cust_call: false, noinline: false, optdbginfo: false }, name: RawString("$LN4") })
	// Label(LabelSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x63fb02 }, flags: ProcedureFlags { nofpo: false, int: false, far: false, never: false, notreached: false, cust_call: false, noinline: false, optdbginfo: false }, name: RawString("$LN3") })
	// Label(LabelSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x63fb08 }, flags: ProcedureFlags { nofpo: false, int: false, far: false, never: false, notreached: false, cust_call: false, noinline: false, optdbginfo: false }, name: RawString("$LN2") })
	// ******

	// FUNCTION BODY[0x650ad0]: 22
	// <0x650ad0>|0x000|+0x00e:'557'
	// <0>
	// <1>
	// <2>
	// <0x650ade>|0x00e|+0x006:'561'
	// <0>
	// <0x650ae4>|0x014|+0x006:'563'
	// <0>
	// <1>
	// <0x650aea>|0x01a|+0x006:'566'
	// <0>
	// <0x650af0>|0x020|+0x006:'568'
	// <0>
	// <0x650af6>|0x026|+0x006:'570'
	// <0>
	// <0x650afc>|0x02c|+0x006:'572'
	// <0>
	// <0x650b02>|0x032|+0x006:'574'
	// <0>
	// <0x650b08>|0x038|-0x025:'576'
	// <0>
	// <1>
	// <0x650ae3>|0x013|+0x006:'579'
	// <0x650ae9>|0x019|+0x006:'579'
	// <0x650aef>|0x01f|+0x006:'579'
	// <0x650af5>|0x025|+0x006:'579'
	// <0x650afb>|0x02b|+0x006:'579'
	// <0x650b01>|0x031|+0x006:'579'
	// <0x650b07>|0x037|+0x006:'579'
	// ******
}

// STATE[STUB]
fs_new::virtual_path_string get_material_effects_instance_request_path( material_ptr mtl, enum_vertex_input_type vertex_input_type )
{
	// FUNCTION BODY[0x6517b0]: 7
	// <0>
	// <0x6517b0>|0x000|+0x017:'585'
	// <0>
	// <1>
	// <2>
	// <0x6517c7>|0x017|+0x031:'589'
	// <0x6517f8>|0x048|+0x02b:'590'
	// ******
}

// STATE[STUB]
void render_model_cook::query_materail_effects( cook_intermediate_data* cook_data )
{
	// LOCALS
	// fs_new::virtual_path_string* 	pathes
	// variant< 32 >** 					user_data_variants_ptrs
	// variant< 32 >* 					user_data_variants
	// const u32 						num_requests
	// resources::request* 				requests
	// u32 								model_index
	// mesh_type_enum 					model_type
	// ******

	// FUNCTION BODY[0x652300]: 63
	// <0>
	// <1>
	// <0x652308>|0x008|+0x012:'597'
	// <0>
	// <0x65231a>|0x01a|+0x011:'599'
	// <0>
	// <0x65232b>|0x02b|+0x00f:'601'
	// <0x65233a>|0x03a|+0x00f:'602'
	// <0>
	// <0x652349>|0x049|+0x028:'604'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <0x652371>|0x071|+0x09f:'611'
	// <0x652410>|0x110|-0x098:'611'
	// <0x652378>|0x078|+0x018:'612'
	// <0x652390>|0x090|+0x017:'613'
	// <0x6523a7>|0x0a7|+0x018:'614'
	// <0x6523bf>|0x0bf|+0x017:'615'
	// <0>
	// <0x6523d6>|0x0d6|+0x03e:'617'
	// <0>
	// <0x652414>|0x114|+0x010:'619'
	// <0x652424>|0x124|+0x00d:'620'
	// <0>
	// <0x652431>|0x131|+0x011:'622'
	// <0x652442>|0x142|+0x008:'623'
	// <0>
	// <0x65244a>|0x14a|+0x00a:'625'
	// <0x652454>|0x154|+0x008:'626'
	// <0>
	// <0x65245c>|0x15c|+0x002:'628'
	// <0x65245e>|0x15e|+0x005:'629'
	// <0x652463>|0x163|+0x062:'630'
	// <0>
	// <0x6524c5>|0x1c5|+0x05d:'632'
	// <0>
	// <1>
	// <0x652522>|0x222|+0x01d:'635'
	// <0>
	// <0x65253f>|0x23f|+0x023:'637'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <0x652562>|0x262|+0x0b4:'643'
	// <0x652616>|0x316|+0x05b:'644'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <7>
	// <0x652671>|0x371|+0x0a4:'653'
	// <0x652715>|0x415|+0x01e:'654'
	// <0x652733>|0x433|+0x01d:'655'
	// <0x652750>|0x450|-0x3f1:'656'
	// <0x65235f>|0x05f|+0x40a:'657'
	// <0x652769>|0x469|-0x405:'657'
	// <0x652364>|0x064|+0x41d:'658'
	// ******
}

// STATE[STUB]
void arrange_surfaces_by_lod( cook_intermediate_data* cook_data, model_lods_descriptor*& lods_descriptor )
{
	// LOCALS
	// configs::binary_config_value 	t_lods
	// u8[3] 							result_lod_surfaces_count
	// pcstr[3] 						lods
	// pbyte[3] 						result_lod_surfaces
	// configs::binary_config_value 	t_root
	// configs::binary_config_value const* it_e
	// configs::binary_config_value 	t_surfaces
	// u8 								i
	// configs::binary_config_value const* it
	// ******

	// FUNCTION BODY[0x651110]: 112
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <0x651118>|0x008|+0x004:'669'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <0x65111c>|0x00c|+0x04c:'675'
	// <0>
	// <0x651168>|0x058|+0x018:'677'
	// <0>
	// <1>
	// <2>
	// <0x651180>|0x070|+0x002:'681'
	// <0x651182>|0x072|+0x00b:'682'
	// <0>
	// <0x65118d>|0x07d|+0x0f9:'684'
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x651286>|0x176|+0x020:'689'
	// <0x6512a6>|0x196|+0x037:'690'
	// <0>
	// <1>
	// <0x6512dd>|0x1cd|+0x03f:'693'
	// <0>
	// <0x65131c>|0x20c|+0x014:'695'
	// <0>
	// <1>
	// <0x651330>|0x220|+0x008:'698'
	// <0x651338>|0x228|+0x015:'699'
	// <0>
	// <1>
	// <0x65134d>|0x23d|+0x021:'702'
	// <0>
	// <1>
	// <0x65136e>|0x25e|+0x028:'705'
	// <0x651396>|0x286|+0x015:'706'
	// <0x6513ab>|0x29b|+0x017:'707'
	// <0>
	// <1>
	// <0x6513c2>|0x2b2|+0x007:'710'
	// <0>
	// <0x6513c9>|0x2b9|+0x050:'712'
	// <0>
	// <0x651419>|0x309|+0x00a:'714'
	// <0>
	// <0x651423>|0x313|+0x01c:'716'
	// <0>
	// <0x65143f>|0x32f|+0x00f:'718'
	// <0x65144e>|0x33e|+0x009:'719'
	// <0>
	// <1>
	// <0x651457>|0x347|+0x103:'722'
	// <0x65155a>|0x44a|+0x002:'723'
	// <0>
	// <0x65155c>|0x44c|+0x007:'725'
	// <0x651563>|0x453|+0x01e:'726'
	// <0>
	// <1>
	// <0x651581>|0x471|+0x004:'729'
	// <0>
	// <1>
	// <0x651585>|0x475|+0x01e:'732'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <0x6515a3>|0x493|+0x036:'740'
	// <0x6515d9>|0x4c9|+0x00c:'741'
	// <0>
	// <0x6515e5>|0x4d5|+0x016:'743'
	// <0>
	// <0x6515fb>|0x4eb|+0x00e:'745'
	// <0x651609>|0x4f9|+0x01f:'746'
	// <0x651628>|0x518|+0x029:'747'
	// <0x651651>|0x541|+0x008:'748'
	// <0>
	// <0x651659>|0x549|+0x032:'750'
	// <0x65168b>|0x57b|+0x032:'751'
	// <0x6516bd>|0x5ad|+0x023:'752'
	// <0x6516e0>|0x5d0|+0x02e:'753'
	// <0x65170e>|0x5fe|-0x002:'753'
	// <0x65170c>|0x5fc|+0x00e:'754'
	// <0x65171a>|0x60a|+0x002:'754'
	// <0x65171c>|0x60c|+0x020:'755'
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x65173c>|0x62c|-0x619:'760'
	// <0x651123>|0x013|+0x62d:'761'
	// <0>
	// <0x651750>|0x640|+0x007:'763'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <0x651757>|0x647|+0x021:'769'
	// <0>
	// <0x651778>|0x668|+0x004:'771'
	// <0x65177c>|0x66c|-0x501:'772'
	// <0>
	// <0x65127b>|0x16b|+0x520:'774'
	// ******
}

// STATE[STUB]
void render_model_cook::finish_model_creation(
	resources::queries_result&		data_material_effects,
	cook_intermediate_data*			cook_data
)
{
	// LOCALS
	// model_lods_descriptor* 			lods_descriptor
	// render_surface** 				surfaces
	// resources::query_result_for_cook* parent_query
	// u32 								model_index
	// resources::pinned_ptr_const< u8 > converted_model_ptr
	// configs::binary_config_value const& properties
	// memory::chunk_reader 			model_reader
	// bool 							material_result
	// configs::binary_config_ptr 		prop_config_ptr
	// pcstr 							sg_name
	// resources::unmanaged_resource_ptr m
	// ******

	// CALL SITE INFO
	// <0x6520b6> -> void < unknown >( configs::binary_config_value const&, memory::chunk_reader& )
	// <0x652183> -> void < unknown >( render_surface**, u8, model_lods_descriptor* )
	// ******

	// FUNCTION BODY[0x651ca0]: 81
	// <0x651cad>|0x00d|+0x00d:'778'
	// <0>
	// <0x651cba>|0x01a|+0x015:'780'
	// <0>
	// <0x651ccf>|0x02f|+0x00b:'782'
	// <0>
	// <0x651cda>|0x03a|+0x012:'784'
	// <0x651cec>|0x04c|+0x007:'785'
	// <0>
	// <1>
	// <2>
	// <0x651cf3>|0x053|+0x005:'789'
	// <0>
	// <1>
	// <0x651cf8>|0x058|+0x048:'792'
	// <0x651d40>|0x0a0|-0x031:'792'
	// <0>
	// <0x651d0f>|0x06f|+0x035:'794'
	// <0>
	// <0x651d44>|0x0a4|+0x028:'796'
	// <0x651d6c>|0x0cc|+0x00a:'797'
	// <0>
	// <0x651d76>|0x0d6|+0x017:'799'
	// <0>
	// <0x651d8d>|0x0ed|+0x00a:'801'
	// <0x651d97>|0x0f7|+0x005:'802'
	// <0>
	// <0x651d9c>|0x0fc|+0x058:'804'
	// <0>
	// <0x651df4>|0x154|+0x007:'806'
	// <0>
	// <0x651dfb>|0x15b|+0x00c:'808'
	// <0x651e07>|0x167|+0x037:'809'
	// <0>
	// <1>
	// <2>
	// <0x651e3e>|0x19e|+0x012:'813'
	// <0>
	// <0x651e50>|0x1b0|+0x008:'815'
	// <0x651e58>|0x1b8|+0x009:'816'
	// <0>
	// <0x651e61>|0x1c1|+0x022:'818'
	// <0>
	// <0x651e83>|0x1e3|+0x00c:'820'
	// <0>
	// <0x651e8f>|0x1ef|+0x00f:'822'
	// <0x651e9e>|0x1fe|+0x00f:'823'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <0x651ead>|0x20d|+0x067:'831'
	// <0>
	// <0x651f14>|0x274|+0x017:'833'
	// <0x651f2b>|0x28b|+0x013:'834'
	// <0>
	// <1>
	// <0x651f3e>|0x29e|+0x00e:'837'
	// <0>
	// <0x651f4c>|0x2ac|+0x02b:'839'
	// <0x651f77>|0x2d7|+0x0f6:'840'
	// <0>
	// <0x65206d>|0x3cd|+0x008:'842'
	// <0x652075>|0x3d5|+0x043:'843'
	// <0>
	// <1>
	// <2>
	// <0x6520b8>|0x418|+0x09c:'847'
	// <0>
	// <1>
	// <2>
	// <0x652154>|0x4b4|+0x00f:'851'
	// <0x652163>|0x4c3|+0x022:'852'
	// <0>
	// <0x652185>|0x4e5|+0x030:'854'
	// <0x6521b5>|0x515|+0x00a:'855'
	// <0>
	// <0x6521bf>|0x51f|+0x015:'857'
	// <0x6521d4>|0x534|+0x01b:'858'
	// ******
}

// STATE[STUB]
void render_model_cook::delete_resource( resources::resource_base* resource )
{
	// FUNCTION BODY[0x650e70]: 3
	// <0>
	// <1>
	// <0x650e71>|0x001|+0x03a:'866'
	// ******
}

	// TYPEDEFS
	// typedef
	// 	pbyte
	// 	iterator_type;

	// typedef
	// 	pcvoid*
	// 	iterator_type;

	// typedef
	// 	u16*
	// 	iterator_type;

	// typedef
	// 	void**
	// 	iterator_type;

	// typedef
	// 	vostok::fixed_string< 32 >*
	// 	iterator_type;

	// typedef
	// 	vostok::render::geometry_batch*
	// 	iterator_type;

	// typedef
	// 	vostok::render::res_pass_ptr*
	// 	iterator_type;

	// typedef
	// 	vostok::render::res_shader_technique_ptr*
	// 	iterator_type;

	// typedef
	// 	vostok::render::shader_constant_binding*
	// 	iterator_type;

	// typedef
	// 	vostok::render::signature_layout_pair*
	// 	iterator_type;

	// typedef
	// 	vostok::render::texture_named_instance*
	// 	iterator_type;

	// ******

} // namespace render
} // namespace vostok
