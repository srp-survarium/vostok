////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef VOSTOK\RENDER\ENGINE\SOURCES\RENDER_MODEL_SKELETON_H_INCLUDED
#define VOSTOK\RENDER\ENGINE\SOURCES\RENDER_MODEL_SKELETON_H_INCLUDED

namespace vostok {
namespace render {

// STATE[STUB]
inline bool skeleton_render_surface::is_dynamic( ) const
{
	return false;

	// FUNCTION BODY[0x22700]
	// ******
}

// STATE[STUB]
inline math::aabb skeleton_render_model_instance::get_aabb( )
{
	return vostok::math::aabb();

	// FUNCTION BODY[0x6f3f0]
	// ******
}

// STATE[STUB]
inline u32 skeleton_render_model_instance::get_surfaces_count( ) const
{
	return 0;

	// FUNCTION BODY[0x6f450]
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

#endif // #ifndef VOSTOK\RENDER\ENGINE\SOURCES\RENDER_MODEL_SKELETON_H_INCLUDED
