////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef VOSTOK\RENDER\ENGINE\SOURCES\BATCHED_GEOMETRY_INLINE_H_INCLUDED
#define VOSTOK\RENDER\ENGINE\SOURCES\BATCHED_GEOMETRY_INLINE_H_INCLUDED

namespace vostok {
namespace render {

// STATE[STUB]
inline void batched_geometry<vostok::render::lpv_vertex>::for_each_batch_render(
	renderer_context*		context,
	boost::function< void( geometry_batch const& ) > const&	pre_render_predicate,
	boost::function< void( geometry_batch const& ) > const&	post_render_predicate
)
{
	// LOCALS
	// math::frustum 					view_frustum
	// geometry_batch const* 			end
	// ******

	// FUNCTION BODY[0x7d2d0]: 29
	// <0x7d2db>|0x00b|+0x01e:'93'
	// <0>
	// <1>
	// <2>
	// <0x7d2f9>|0x029|+0x006:'97'
	// <0>
	// <0x7d2ff>|0x02f|+0x019:'99'
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x7d318>|0x048|+0x008:'104'
	// <0>
	// <1>
	// <2>
	// <0x7d320>|0x050|+0x013:'108'
	// <0>
	// <1>
	// <0x7d333>|0x063|+0x008:'111'
	// <0>
	// <0x7d33b>|0x06b|+0x00d:'113'
	// <0>
	// <0x7d348>|0x078|+0x078:'115'
	// <0>
	// <0x7d3c0>|0x0f0|+0x021:'117'
	// <0>
	// <1>
	// <2>
	// <3>
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
	// 	vostok::render::lpv_render_surface*
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

#endif // #ifndef VOSTOK\RENDER\ENGINE\SOURCES\BATCHED_GEOMETRY_INLINE_H_INCLUDED
