////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "vostok\render\core\dx11\sources\effect_cook.h"

namespace vostok {
namespace render {

// STATE[STUB]
 effect_cook::effect_cook( )
{
	// FUNCTION BODY[0x758e30]: 0
	// ******
}

// STATE[STUB]
void effect_cook::on_binary_shaders(
	resources::query_result_for_cook*		in_out_query,
	res_effect*								effect_resource,
	effect_compile_data*					compile_data,
	resources::queries_result&				data
)
{
	// LOCALS
	// effect_compiler 					compiler
	// associative_vector< binary_shader_key_type, binary_shader_source_ptr, vector, std::less< binary_shader_key_type > > sources
	// u32 								i
	// binary_shader_key_type 			key
	// ******

	// CALL SITE INFO
	// <0x7590e7> -> void* < unknown >( u32 )
	// <0x759130> -> void < unknown >( effect_compiler&, custom_config_value const& )
	// ******

	// FUNCTION BODY[0x758f10]: 43
	// <0>
	// <1>
	// <2>
	// <0x758f20>|0x010|+0x12a:'51'
	// <0x75904a>|0x13a|-0x0ea:'51'
	// <0>
	// <0x758f60>|0x050|+0x015:'53'
	// <0>
	// <0x758f75>|0x065|+0x03f:'55'
	// <0x758fb4>|0x0a4|+0x03a:'56'
	// <0x758fee>|0x0de|+0x045:'57'
	// <0x759033>|0x123|+0x002:'58'
	// <0x759035>|0x125|-0x110:'59'
	// <0>
	// <1>
	// <0x758f25>|0x015|+0x144:'62'
	// <0x759069>|0x159|+0x00a:'62'
	// <0>
	// <0x759073>|0x163|+0x00e:'64'
	// <0x759081>|0x171|+0x05b:'65'
	// <0x7590dc>|0x1cc|+0x00d:'66'
	// <0x7590e9>|0x1d9|+0x015:'67'
	// <0x7590fe>|0x1ee|+0x005:'68'
	// <0>
	// <1>
	// <0x759103>|0x1f3|+0x019:'71'
	// <0x75911c>|0x20c|+0x016:'72'
	// <0>
	// <1>
	// <0x759132>|0x222|+0x010:'75'
	// <0>
	// <0x759142>|0x232|+0x004:'77'
	// <0>
	// <0x759146>|0x236|+0x018:'79'
	// <0>
	// <1>
	// <0x75915e>|0x24e|+0x032:'82'
	// <0x759190>|0x280|+0x00b:'83'
	// <0>
	// <0x75919b>|0x28b|+0x009:'85'
	// <0>
	// <0x7591a4>|0x294|+0x013:'87'
	// <0>
	// <1>
	// <0x7591b7>|0x2a7|+0x054:'90'
	// ******
}

// STATE[STUB]
void effect_cook::create_resource(
	resources::query_result_for_cook&		in_out_query,
	const_buffer							raw_file_data,
	mutable_buffer							in_out_unmanaged_resource_buffer
)
{
	// LOCALS
	// effect_compile_data* 			compile_data
	// fs_new::virtual_path_string* 	pathes
	// effect_compiler 					compiler
	// const u32 						num_requests
	// res_effect* 						effect_resource
	// resources::creation_request* 	requests
	// ******

	// CALL SITE INFO
	// <0x7594bf> -> void < unknown >( effect_compiler&, custom_config_value const& )
	// ******

	// FUNCTION BODY[0x759260]: 157
	// <0>
	// <1>
	// <2>
	// <0x759273>|0x013|+0x01b:'100'
	// <0x75928e>|0x02e|+0x00a:'101'
	// <0>
	// <0x759298>|0x038|+0x008:'103'
	// <0>
	// <0x7592a0>|0x040|+0x00b:'105'
	// <0>
	// <0x7592ab>|0x04b|+0x01f:'107'
	// <0x7592ca>|0x06a|+0x017:'108'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <0x7592e1>|0x081|+0x009:'115'
	// <0>
	// <0x7592ea>|0x08a|+0x0a5:'117'
	// <0x75938f>|0x12f|+0x00b:'118'
	// <0>
	// <0x75939a>|0x13a|+0x01f:'120'
	// <0x7593b9>|0x159|+0x017:'121'
	// <0>
	// <1>
	// <2>
	// <0x7593d0>|0x170|+0x00a:'125'
	// <0>
	// <0x7593da>|0x17a|+0x097:'127'
	// <0x759471>|0x211|-0x005:'127'
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x75946c>|0x20c|+0x00c:'132'
	// <0x759478>|0x218|+0x005:'132'
	// <0>
	// <1>
	// <0x75947d>|0x21d|+0x01a:'135'
	// <0>
	// <0x759497>|0x237|+0x086:'137'
	// <0x75951d>|0x2bd|-0x06f:'137'
	// <0>
	// <0x7594ae>|0x24e|+0x013:'139'
	// <0>
	// <0x7594c1>|0x261|+0x009:'141'
	// <0>
	// <1>
	// <0x7594ca>|0x26a|+0x00f:'144'
	// <0>
	// <1>
	// <0x7594d9>|0x279|+0x022:'147'
	// <0x7594fb>|0x29b|+0x01c:'148'
	// <0x759517>|0x2b7|+0x002:'149'
	// <0>
	// <0x759519>|0x2b9|+0x032:'151'
	// <0x75954b>|0x2eb|-0x02a:'151'
	// <0x759521>|0x2c1|+0x031:'152'
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x759552>|0x2f2|+0x2e4:'157'
	// <0x759836>|0x5d6|-0x2bf:'157'
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
	// <0x759577>|0x317|+0x00e:'170'
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
	// <0x759585>|0x325|+0x0c3:'184'
	// <0x759648>|0x3e8|+0x005:'185'
	// <0>
	// <1>
	// <0x75964d>|0x3ed|+0x01b:'188'
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
	// <0x759668>|0x408|+0x0c3:'202'
	// <0x75972b>|0x4cb|+0x005:'203'
	// <0>
	// <1>
	// <0x759730>|0x4d0|+0x01b:'206'
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
	// <0x75974b>|0x4eb|+0x0c3:'220'
	// <0x75980e>|0x5ae|+0x02c:'221'
	// <0>
	// <1>
	// <0x75983a>|0x5da|+0x00e:'224'
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
	// <0x759848>|0x5e8|+0x0d0:'241'
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
	// <0x759918>|0x6b8|+0x01e:'252'
	// <0x759936>|0x6d6|-0x65e:'253'
	// <0x7592d8>|0x078|+0x0ef:'254'
	// <0x7593c7>|0x167|+0x583:'254'
	// ******
}

// STATE[STUB]
void effect_cook::destroy_resource( resources::unmanaged_resource* resource_to_destroy )
{
	// CALL SITE INFO
	// <0x758eca> -> void* < unknown >( u32 )
	// ******

	// FUNCTION BODY[0x758eb0]: 2
	// <0x758eb1>|0x001|+0x011:'258'
	// <0x758ec2>|0x012|+0x00c:'259'
	// ******
}

// STATE[STUB]
mutable_buffer effect_cook::allocate_resource(
	resources::query_result_for_cook&		in_query,
	const_buffer							raw_file_data,
	bool									file_exist
)
{
	// FUNCTION BODY[0x758ee0]: 2
	// <0>
	// <0x758ee0>|0x000|+0x025:'267'
	// ******
}

// STATE[STUB]
void effect_cook::deallocate_resource( void* buffer )
{
	// FUNCTION BODY[0x758e90]: 1
	// <0x758e90>|0x000|+0x01c:'272'
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
	// 	std::pair< vostok::render::binary_shader_key_type, vostok::render::binary_shader_source_ptr >*
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
