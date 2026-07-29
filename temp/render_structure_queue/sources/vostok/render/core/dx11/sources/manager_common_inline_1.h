////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef VOSTOK\RENDER\CORE\DX11\SOURCES\MANAGER_COMMON_INLINE_H_INCLUDED
#define VOSTOK\RENDER\CORE\DX11\SOURCES\MANAGER_COMMON_INLINE_H_INCLUDED

namespace vostok {
namespace render {

// STATE[STUB]
inline bool reclaim<vostok::render::res_pass,vostok::render::effect_manager::compare_predicate<vostok::render::res_pass> >(
	set< res_pass*, effect_manager::compare_predicate< res_pass > >&	container,
	res_pass const*		ptr
)
{
	return false;

	// FUNCTION BODY[0x12a540]: 7
	// <0x12a540>|0x000|+0x00a:'34'
	// <0x12a54a>|0x00a|+0x004:'35'
	// <0x12a54e>|0x00e|+0x030:'36'
	// <0x12a57e>|0x03e|+0x003:'37'
	// <0>
	// <1>
	// <0x12a581>|0x041|-0x001:'40'
	// <0x12a580>|0x040|+0x003:'41'
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
	// 	vostok::render::effect_manager::effect_holder_struct*
	// 	iterator_type;

	// typedef
	// 	vostok::render::effect_manager::effect_to_recompile_struct*
	// 	iterator_type;

	// typedef
	// 	vostok::render::effect_manager::shader_cache_info*
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

#endif // #ifndef VOSTOK\RENDER\CORE\DX11\SOURCES\MANAGER_COMMON_INLINE_H_INCLUDED
