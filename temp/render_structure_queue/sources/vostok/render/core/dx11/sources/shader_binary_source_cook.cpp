////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "vostok\render\core\dx11\sources\shader_binary_source_cook.h"

namespace vostok {
namespace render {

// STATE[STUB]
void `dynamic initializer for 's_no_cache_shaders_key''( )
{
	// FUNCTION BODY[0x7df560]
	// ******
}

// STATE[STUB]
 shader_binary_source_cook::shader_binary_source_cook( )
{
	// FUNCTION BODY[0x5588a0]: 1
	// <0>
	// ******
}

// STATE[STUB]
 shader_binary_source_cook::~shader_binary_source_cook( )
{
	// FUNCTION BODY[0x558890]: 1
	// <0>
	// ******
}

// STATE[STUB]
void shader_binary_source_cook::converted_shader_loaded( conveted_shader_loaded_data* data, resources::queries_result& result )
{
	// LOCALS
	// resources::pinned_ptr_const< u8 > ptr_managed
	// ******

	// FUNCTION BODY[0x5589e0]: 47
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
	// <0x5589e6>|0x006|+0x060:'422'
	// <0x558a46>|0x066|+0x00b:'423'
	// <0x558a51>|0x071|+0x00d:'424'
	// <0>
	// <0x558a5e>|0x07e|+0x02f:'426'
	// <0>
	// <0x558a8d>|0x0ad|+0x006:'428'
	// <0>
	// <1>
	// <0x558a93>|0x0b3|+0x00f:'431'
	// <0x558aa2>|0x0c2|+0x002:'432'
	// <0>
	// <0x558aa4>|0x0c4|+0x009:'434'
	// <0x558aad>|0x0cd|+0x009:'435'
	// <0>
	// <0x558ab6>|0x0d6|+0x022:'437'
	// <0x558ad8>|0x0f8|+0x015:'438'
	// <0x558aed>|0x10d|+0x00c:'439'
	// <0>
	// <0x558af9>|0x119|+0x026:'441'
	// <0x558b1f>|0x13f|+0x00d:'442'
	// <0>
	// <0x558b2c>|0x14c|+0x021:'444'
	// <0x558b4d>|0x16d|+0x00d:'445'
	// <0x558b5a>|0x17a|+0x009:'446'
	// ******
}

// STATE[STUB]
pcstr shader_type_to_ext( enum_shader_type st )
{
	return NULL;

	// FUNCTION BODY[0x558870]: 7
	// <0x558870>|0x000|+0x016:'456'
	// <0>
	// <0x558886>|0x016|-0x006:'458'
	// <0x558880>|0x010|-0x006:'459'
	// <0x55887a>|0x00a|+0x005:'460'
	// <0>
	// <1>
	// <0x55887f>|0x00f|+0x006:'463'
	// <0x558885>|0x015|+0x006:'463'
	// ******
}

// STATE[STUB]
void shader_binary_source_cook::create_resource(
	resources::query_result_for_cook&		in_out_query,
	const_buffer							raw_file_data,
	mutable_buffer							in_out_unmanaged_resource_buffer
)
{
	// LOCALS
	// fixed_vector< shader_macro, 128 > working_defines_list
	// binary_shader_source* 			new_resource
	// resources::request 				request
	// binary_shader_cook_data* 		user_data
	// fs_new::virtual_path_string 		converted_shader_path
	// resources::query_resource_params params
	// configs::binary_config_value const& mask_values
	// shader_macro const* 				define_end
	// ******

	// FUNCTION BODY[0x558b70]: 117
	// <0x558b87>|0x017|+0x012:'469'
	// <0>
	// <0x558b99>|0x029|+0x00c:'471'
	// <0>
	// <0x558ba5>|0x035|+0x00d:'473'
	// <0>
	// <0x558bb2>|0x042|+0x01f:'475'
	// <0x558bd1>|0x061|+0x01f:'476'
	// <0>
	// <1>
	// <2>
	// <0x558bf0>|0x080|+0x035:'480'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <0x558c25>|0x0b5|+0x03f:'486'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <0x558c64>|0x0f4|+0x01d:'494'
	// <0>
	// <1>
	// <0x558c81>|0x111|+0x010:'497'
	// <0x558c91>|0x121|+0x05e:'498'
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
	// <0x558cef>|0x17f|+0x00e:'510'
	// <0>
	// <1>
	// <0x558cfd>|0x18d|+0x051:'513'
	// <0>
	// <0x558d4e>|0x1de|+0x056:'515'
	// <0x558da4>|0x234|-0x020:'515'
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x558d84>|0x214|+0x030:'520'
	// <0>
	// <1>
	// <0x558db4>|0x244|+0x006:'523'
	// <0>
	// <0x558dba>|0x24a|+0x00d:'525'
	// <0>
	// <1>
	// <2>
	// <0x558dc7>|0x257|+0x009:'529'
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x558dd0>|0x260|+0x071:'534'
	// <0>
	// <1>
	// <0x558e41>|0x2d1|+0x017:'537'
	// <0>
	// <1>
	// <2>
	// <0x558e58>|0x2e8|-0x050:'541'
	// <0>
	// <0x558e08>|0x298|+0x037:'543'
	// <0>
	// <1>
	// <2>
	// <0x558e3f>|0x2cf|+0x01b:'547'
	// <0>
	// <0x558e5a>|0x2ea|+0x007:'549'
	// <0x558e61>|0x2f1|+0x007:'550'
	// <0>
	// <0x558e68>|0x2f8|+0x008:'552'
	// <0>
	// <0x558e70>|0x300|+0x02a:'554'
	// <0>
	// <1>
	// <2>
	// <0x558e9a>|0x32a|+0x010:'558'
	// <0>
	// <0x558eaa>|0x33a|+0x007:'560'
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
	// <0x558eb1>|0x341|+0x21e:'581'
	// <0>
	// <1>
	// <2>
	// <0x5590cf>|0x55f|-0x4e8:'585'
	// <0x558be7>|0x077|+0x500:'586'
	// ******
}

// STATE[STUB]
void shader_binary_source_cook::destroy_resource( resources::unmanaged_resource* resource_to_destroy )
{
	// CALL SITE INFO
	// <0x558991> -> unsigned long < unknown >()
	// <0x55899b> -> void* < unknown >( u32 )
	// ******

	// FUNCTION BODY[0x558950]: 26
	// <0>
	// <1>
	// <0x558951>|0x001|+0x00d:'592'
	// <0>
	// <1>
	// <0x55895e>|0x00e|+0x019:'595'
	// <0>
	// <1>
	// <2>
	// <0x558977>|0x027|+0x00a:'599'
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
	// <0x558981>|0x031|+0x00a:'612'
	// <0x55898b>|0x03b|+0x008:'613'
	// <0>
	// <0x558993>|0x043|+0x00b:'615'
	// ******
}

// STATE[STUB]
mutable_buffer shader_binary_source_cook::allocate_resource(
	resources::query_result_for_cook&		in_query,
	const_buffer							raw_file_data,
	bool									file_exist
)
{
	// FUNCTION BODY[0x5589b0]: 5
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x5589b0>|0x000|+0x025:'626'
	// ******
}

// STATE[STUB]
void shader_binary_source_cook::deallocate_resource( void* buffer )
{
	// FUNCTION BODY[0x558930]: 1
	// <0x558930>|0x000|+0x01c:'631'
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
	// 	void**
	// 	iterator_type;

	// typedef
	// 	vostok::fixed_string< 32 >*
	// 	iterator_type;

	// typedef
	// 	vostok::render::effect_compiler::shader_cache_info*
	// 	iterator_type;

	// typedef
	// 	vostok::render::res_pass_ptr*
	// 	iterator_type;

	// typedef
	// 	vostok::render::res_shader_technique_ptr*
	// 	iterator_type;

	// typedef
	// 	vostok::render::shader_constant*
	// 	iterator_type;

	// typedef
	// 	vostok::render::shader_constant_binding*
	// 	iterator_type;

	// typedef
	// 	vostok::render::shader_constant_buffer_ptr*
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
