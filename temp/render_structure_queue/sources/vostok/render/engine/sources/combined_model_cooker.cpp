////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "vostok\render\engine\sources\combined_model_cooker.h"

namespace vostok {
namespace render {

// STATE[STUB]
 skeleton_combined_model_cook::skeleton_combined_model_cook( )
{
	// FUNCTION BODY[0x64f2a0]: 1
	// <0x64f2e0>|0x040|+0x02d:'24'
	// ******
}

// STATE[STUB]
void skeleton_combined_model_cook::translate_query( resources::query_result_for_cook& parent )
{
	// LOCALS
	// skeleton_combined_cook_data* 	cook_data
	// fs_new::virtual_path_string 		config_path
	// ******

	// FUNCTION BODY[0x650810]: 27
	// <0x65081f>|0x00f|+0x00f:'29'
	// <0>
	// <1>
	// <0x65082e>|0x01e|+0x012:'32'
	// <0>
	// <1>
	// <0x650840>|0x030|+0x008:'35'
	// <0>
	// <0x650848>|0x038|+0x011:'37'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <0x650859>|0x049|+0x00f:'44'
	// <0x650868>|0x058|+0x037:'45'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <7>
	// <0x65089f>|0x08f|-0x04f:'54'
	// <0>
	// <0x650850>|0x040|+0x0ec:'56'
	// ******
}

// STATE[STUB]
void build_from_config( configs::binary_config_ptr& cfg, skeleton_combined_cook_data* cook_data )
{
	// LOCALS
	// configs::binary_config_value 	root
	// u32 								part_idx
	// configs::binary_config_value 	current
	// ******

	// FUNCTION BODY[0x64f3e0]: 12
	// <0x64f3e9>|0x009|+0x0ae:'60'
	// <0x64f497>|0x0b7|-0x08a:'60'
	// <0x64f40d>|0x02d|+0x025:'61'
	// <0x64f432>|0x052|+0x025:'62'
	// <0x64f457>|0x077|+0x02c:'63'
	// <0>
	// <0x64f483>|0x0a3|+0x01d:'65'
	// <0>
	// <1>
	// <0x64f4a0>|0x0c0|+0x02d:'68'
	// <0x64f4cd>|0x0ed|+0x046:'69'
	// <0x64f513>|0x133|+0x075:'70'
	// <0>
	// ******
}

// STATE[STUB]
void skeleton_combined_model_cook::on_config_loaded( resources::queries_result& result, resources::query_result_for_cook* parent )
{
	// LOCALS
	// configs::binary_config_ptr 		cfg
	// ******

	// FUNCTION BODY[0x650730]: 11
	// <0x650734>|0x004|+0x012:'77'
	// <0>
	// <0x650746>|0x016|+0x018:'79'
	// <0>
	// <1>
	// <2>
	// <0x65075e>|0x02e|+0x023:'83'
	// <0>
	// <0x650781>|0x051|+0x049:'85'
	// <0x6507ca>|0x09a|+0x009:'86'
	// <0x6507d3>|0x0a3|-0x07e:'87'
	// <0x650755>|0x025|+0x08e:'88'
	// ******
}

// STATE[STUB]
void skeleton_combined_model_cook::query_resources_by_data( resources::query_result_for_cook* parent, skeleton_combined_cook_data* cook_data )
{
	// LOCALS
	// u32 								request_count
	// resources::request* 				requests
	// u32 								part_idx
	// strings::detail::tuples 			STR_JOINA_tuples_unique_identifier
	// strings::detail::tuples 			STR_JOINA_tuples_unique_identifier
	// strings::detail::tuples 			STR_JOINA_tuples_unique_identifier
	// ******

	// FUNCTION BODY[0x6504d0]: 43
	// <0x650550>|0x080|-0x07a:'93'
	// <0x6504d6>|0x006|+0x029:'94'
	// <0>
	// <1>
	// <0x6504ff>|0x02f|+0x00c:'97'
	// <0x65050b>|0x03b|+0x011:'98'
	// <0>
	// <0x65051c>|0x04c|+0x037:'100'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <7>
	// <0x650553>|0x083|-0x017:'109'
	// <0x65053c>|0x06c|+0x05a:'110'
	// <0x650596>|0x0c6|+0x008:'110'
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x65059e>|0x0ce|+0x039:'115'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <0x6505d7>|0x107|-0x098:'123'
	// <0x65053f>|0x06f|+0x0eb:'124'
	// <0x65062a>|0x15a|+0x035:'124'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <7>
	// <0x65065f>|0x18f|+0x0ab:'133'
	// <0>
	// <0x65070a>|0x23a|+0x015:'135'
	// ******
}

// STATE[STUB]
enum_vertex_input_type mesh_type_to_vertex_input_type( mesh_type_enum type )
{
	// STATICS
	// static < NoType > 				 = <0x64f0f4>;
	// static < NoType > 				 = <0x64f0d0>;
	// ******

	// OTHER SYMBOLS
	// Label(LabelSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x63e09e }, flags: ProcedureFlags { nofpo: false, int: false, far: false, never: false, notreached: false, cust_call: false, noinline: false, optdbginfo: false }, name: RawString("$LN9") })
	// Label(LabelSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x63e0a4 }, flags: ProcedureFlags { nofpo: false, int: false, far: false, never: false, notreached: false, cust_call: false, noinline: false, optdbginfo: false }, name: RawString("$LN8") })
	// Label(LabelSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x63e0aa }, flags: ProcedureFlags { nofpo: false, int: false, far: false, never: false, notreached: false, cust_call: false, noinline: false, optdbginfo: false }, name: RawString("$LN7") })
	// Label(LabelSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x63e0b0 }, flags: ProcedureFlags { nofpo: false, int: false, far: false, never: false, notreached: false, cust_call: false, noinline: false, optdbginfo: false }, name: RawString("$LN6") })
	// Label(LabelSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x63e0b6 }, flags: ProcedureFlags { nofpo: false, int: false, far: false, never: false, notreached: false, cust_call: false, noinline: false, optdbginfo: false }, name: RawString("$LN5") })
	// Label(LabelSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x63e0bc }, flags: ProcedureFlags { nofpo: false, int: false, far: false, never: false, notreached: false, cust_call: false, noinline: false, optdbginfo: false }, name: RawString("$LN4") })
	// Label(LabelSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x63e0c2 }, flags: ProcedureFlags { nofpo: false, int: false, far: false, never: false, notreached: false, cust_call: false, noinline: false, optdbginfo: false }, name: RawString("$LN3") })
	// Label(LabelSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x63e0c8 }, flags: ProcedureFlags { nofpo: false, int: false, far: false, never: false, notreached: false, cust_call: false, noinline: false, optdbginfo: false }, name: RawString("$LN2") })
	// ******

	// FUNCTION BODY[0x64f090]: 22
	// <0x64f090>|0x000|+0x00e:'140'
	// <0>
	// <1>
	// <2>
	// <0x64f09e>|0x00e|+0x006:'144'
	// <0>
	// <0x64f0a4>|0x014|+0x006:'146'
	// <0>
	// <1>
	// <0x64f0aa>|0x01a|+0x006:'149'
	// <0>
	// <0x64f0b0>|0x020|+0x006:'151'
	// <0>
	// <0x64f0b6>|0x026|+0x006:'153'
	// <0>
	// <0x64f0bc>|0x02c|+0x006:'155'
	// <0>
	// <0x64f0c2>|0x032|+0x006:'157'
	// <0>
	// <0x64f0c8>|0x038|-0x025:'159'
	// <0>
	// <1>
	// <0x64f0a3>|0x013|+0x006:'162'
	// <0x64f0a9>|0x019|+0x006:'162'
	// <0x64f0af>|0x01f|+0x006:'162'
	// <0x64f0b5>|0x025|+0x006:'162'
	// <0x64f0bb>|0x02b|+0x006:'162'
	// <0x64f0c1>|0x031|+0x006:'162'
	// <0x64f0c7>|0x037|+0x006:'162'
	// ******
}

// STATE[STUB]
void skeleton_combined_model_cook::on_resources_loaded(
	resources::queries_result&				data,
	resources::query_result_for_cook*		parent,
	skeleton_combined_cook_data*			cook_data
)
{
	// LOCALS
	// u32 								parts_count
	// variant< 32 >** 					user_data_variants_ptrs
	// variant< 32 >* 					user_data_variants
	// resources::request* 				requests
	// u32 								i
	// configs::binary_config_value 	root
	// ******

	// FUNCTION BODY[0x64fe70]: 51
	// <0x64fe7c>|0x00c|+0x012:'168'
	// <0>
	// <0x64fe8e>|0x01e|+0x017:'170'
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x64fea5>|0x035|+0x0af:'175'
	// <0x64ff54>|0x0e4|+0x0dc:'176'
	// <0x650030>|0x1c0|-0x085:'176'
	// <0>
	// <0x64ffab>|0x13b|+0x007:'178'
	// <0x64ffb2>|0x142|+0x017:'179'
	// <0x64ffc9>|0x159|+0x018:'180'
	// <0x64ffe1>|0x171|+0x017:'181'
	// <0>
	// <0x64fff8>|0x188|+0x03c:'183'
	// <0>
	// <1>
	// <0x650034>|0x1c4|+0x067:'186'
	// <0x65009b>|0x22b|+0x0cc:'187'
	// <0x650167>|0x2f7|+0x0b8:'188'
	// <0>
	// <0x65021f>|0x3af|+0x026:'190'
	// <0>
	// <0x650245>|0x3d5|+0x014:'192'
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x650259>|0x3e9|+0x053:'197'
	// <0>
	// <1>
	// <0x6502ac>|0x43c|+0x019:'200'
	// <0>
	// <0x6502c5>|0x455|+0x011:'202'
	// <0x6502d6>|0x466|+0x01f:'203'
	// <0x6502f5>|0x485|+0x0ab:'204'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <7>
	// <8>
	// <0x6503a0>|0x530|+0x0cb:'214'
	// <0>
	// <0x65046b>|0x5fb|+0x01e:'216'
	// <0x650489>|0x619|+0x01d:'217'
	// <0x6504a6>|0x636|-0x60a:'218'
	// <0x64fe9c>|0x02c|+0x61e:'219'
	// ******
}

// STATE[STUB]
void skeleton_combined_model_cook::on_material_effects_loaded(
	resources::queries_result&				data,
	resources::query_result_for_cook*		parent,
	skeleton_combined_cook_data*			cook_data
)
{
	// LOCALS
	// u8 								parts_count
	// resources::pinned_ptr_const< u8 > bind_pose_ptr
	// render_surface** 				surfaces
	// memory::reader 					bones_reader
	// render_model* 					result_model
	// u32 								i
	// resources::pinned_ptr_const< u8 > converted_model_ptr
	// memory::chunk_reader 			model_reader
	// ******

	// CALL SITE INFO
	// <0x64fb8c> -> void < unknown >( configs::binary_config_value const&, memory::chunk_reader& )
	// <0x64fcb4> -> void < unknown >( render_surface**, u8, model_lods_descriptor* )
	// ******

	// FUNCTION BODY[0x64f930]: 38
	// <0x64f93c>|0x00c|+0x037:'263'
	// <0>
	// <0x64f973>|0x043|+0x09d:'265'
	// <0>
	// <1>
	// <2>
	// <0x64fa10>|0x0e0|+0x00a:'269'
	// <0>
	// <0x64fa1a>|0x0ea|+0x03e:'271'
	// <0x64fa58>|0x128|+0x010:'272'
	// <0x64fa68>|0x138|+0x00f:'273'
	// <0>
	// <1>
	// <0x64fa77>|0x147|+0x006:'276'
	// <0x64fa7d>|0x14d|+0x022:'277'
	// <0>
	// <0x64fa9f>|0x16f|+0x023:'279'
	// <0>
	// <1>
	// <0x64fac2>|0x192|+0x01e:'282'
	// <0x64fae0>|0x1b0|+0x005:'283'
	// <0x64fae5>|0x1b5|+0x00d:'284'
	// <0>
	// <0x64faf2>|0x1c2|+0x04f:'286'
	// <0x64fb41>|0x211|+0x013:'287'
	// <0x64fb54>|0x224|+0x03a:'288'
	// <0x64fb8e>|0x25e|+0x072:'289'
	// <0x64fc00>|0x2d0|+0x02e:'290'
	// <0x64fc2e>|0x2fe|+0x073:'291'
	// <0>
	// <0x64fca1>|0x371|+0x015:'293'
	// <0>
	// <0x64fcb6>|0x386|+0x02e:'295'
	// <0x64fce4>|0x3b4|+0x00b:'296'
	// <0>
	// <0x64fcef>|0x3bf|-0x3a8:'298'
	// <0x64f947>|0x017|+0x3b1:'299'
	// <0x64fcf8>|0x3c8|+0x01a:'299'
	// <0>
	// ******
}

// STATE[STUB]
void skeleton_combined_model_cook::delete_resource( resources::resource_base* resource )
{
	// FUNCTION BODY[0x64f3a0]: 1
	// <0x64f3a1>|0x001|+0x03a:'305'
	// ******
}

// STATE[STUB]
 skeleton_combined_render_model_instance_cook::skeleton_combined_render_model_instance_cook( )
{
	// FUNCTION BODY[0x64f230]: 1
	// <0x64f268>|0x038|+0x029:'312'
	// ******
}

// STATE[STUB]
void skeleton_combined_render_model_instance_cook::translate_query( resources::query_result_for_cook& parent )
{
	// LOCALS
	// fs_new::virtual_path_string 		render_path
	// skeleton_combined_cook_data* 	cook_data
	// ******

	// FUNCTION BODY[0x64fd70]: 14
	// <0x64fd7f>|0x00f|+0x00f:'317'
	// <0>
	// <1>
	// <0x64fd8e>|0x01e|+0x012:'320'
	// <0>
	// <1>
	// <2>
	// <0x64fda0>|0x030|+0x022:'324'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <0x64fdc2>|0x052|+0x0a5:'330'
	// ******
}

// STATE[STUB]
void skeleton_combined_render_model_instance_cook::on_resources_loaded( resources::queries_result& data, resources::query_result_for_cook* parent )
{
	// FUNCTION BODY[0x64f820]: 12
	// <0x64f823>|0x003|+0x017:'335'
	// <0>
	// <0x64f83a>|0x01a|+0x018:'337'
	// <0>
	// <1>
	// <2>
	// <0x64f852>|0x032|+0x043:'341'
	// <0x64f895>|0x075|+0x021:'342'
	// <0x64f8b6>|0x096|+0x023:'343'
	// <0>
	// <0x64f8d9>|0x0b9|+0x024:'345'
	// <0x64f8fd>|0x0dd|-0x0b4:'346'
	// <0x64f849>|0x029|+0x0bf:'347'
	// ******
}

// STATE[STUB]
void skeleton_combined_render_model_instance_cook::delete_resource( resources::resource_base* resource )
{
	// FUNCTION BODY[0x64f360]: 1
	// <0x64f361>|0x001|+0x03a:'351'
	// ******
}

// STATE[STUB]
 skeleton_combined_model_instance_cook::skeleton_combined_model_instance_cook( )
{
	// FUNCTION BODY[0x64f1c0]: 1
	// <0x64f1f8>|0x038|+0x029:'361'
	// ******
}

// STATE[STUB]
void skeleton_combined_model_instance_cook::translate_query( resources::query_result_for_cook& parent )
{
	// LOCALS
	// skeleton_combined_cook_data* 	cook_data
	// resources::request[2] 			r
	// variant< 32 > 					ud
	// variant< 32 > const*[2] 			params
	// ******

	// FUNCTION BODY[0x650950]: 25
	// <0>
	// <1>
	// <2>
	// <0x65095c>|0x00c|+0x025:'369'
	// <0>
	// <0x650981>|0x031|+0x00a:'371'
	// <0>
	// <1>
	// <2>
	// <0x65098b>|0x03b|+0x032:'375'
	// <0>
	// <0x6509bd>|0x06d|+0x009:'377'
	// <0>
	// <1>
	// <0x6509c6>|0x076|+0x032:'380'
	// <0x6509f8>|0x0a8|+0x014:'381'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <7>
	// <0x650a0c>|0x0bc|+0x09a:'390'
	// ******
}

// STATE[STUB]
void skeleton_combined_model_instance_cook::delete_resource( resources::resource_base* resource )
{
	// FUNCTION BODY[0x64f320]: 1
	// <0x64f321>|0x001|+0x03a:'395'
	// ******
}

// STATE[STUB]
void skeleton_combined_model_instance_cook::on_resources_loaded( resources::queries_result& data, resources::query_result_for_cook* parent_query )
{
	// LOCALS
	// skeleton_model_instance* 		created_resource
	// u32 								i
	// ******

	// FUNCTION BODY[0x64f590]: 21
	// <0x64f59c>|0x00c|+0x02b:'400'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <0x64f5c7>|0x037|-0x024:'406'
	// <0>
	// <0x64f5a3>|0x013|+0x063:'408'
	// <0x64f606>|0x076|+0x0ac:'408'
	// <0x64f6b2>|0x122|+0x0af:'409'
	// <0>
	// <1>
	// <0x64f761>|0x1d1|+0x03f:'412'
	// <0x64f7a0>|0x210|-0x039:'412'
	// <0>
	// <0x64f767>|0x1d7|+0x027:'414'
	// <0>
	// <0x64f78e>|0x1fe|+0x04a:'416'
	// <0x64f7d8>|0x248|-0x034:'416'
	// <0x64f7a4>|0x214|+0x038:'417'
	// <0>
	// <0x64f7dc>|0x24c|-0x223:'419'
	// <0x64f5b9>|0x029|+0x251:'420'
	// <0x64f80a>|0x27a|-0x24c:'420'
	// <0x64f5be>|0x02e|+0x259:'421'
	// ******
}

	// TYPEDEFS
	// typedef
	// 	float4x4*
	// 	iterator_type;

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
