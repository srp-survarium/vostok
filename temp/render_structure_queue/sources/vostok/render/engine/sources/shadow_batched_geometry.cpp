////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "vostok\render\engine\sources\shadow_batched_geometry.h"

namespace vostok {
namespace render {

// STATE[STUB]
 shadow_batched_geometry::shadow_batched_geometry( )
{
	// FUNCTION BODY[0x70b890]: 0
	// ******
}

// STATE[STUB]
void shadow_batched_geometry::add_vertex( batched_vertex_source const& in_vertex, float3 const& not_modified_position )
{
	// LOCALS
	// shadow_vertex 					vertex
	// ******

	// FUNCTION BODY[0x70b650]: 5
	// <0>
	// <0x70b659>|0x009|+0x00b:'39'
	// <0x70b664>|0x014|+0x00a:'40'
	// <0x70b66e>|0x01e|+0x008:'41'
	// <0x70b676>|0x026|+0x05e:'42'
	// <0x70b6d4>|0x084|-0x008:'42'
	// <0x70b6cc>|0x07c|+0x012:'43'
	// ******
}

// STATE[STUB]
void shadow_batched_geometry::build( vector< render_model_instance_impl_ptr >& model_instances )
{
	// LOCALS
	// vector< `shadow_batched_geometry::build'::`2'::surface_set > surfaces
	// render_model_instance_impl_ptr* 	end
	// vector< render_surface_instance* > model_surfaces
	// render_surface_instance** 		end_surf
	// shadow_batched_geometry::build::__l2::surface_set set
	// render_model_instance_impl_ptr* 	it
	// ******

	// TYPEDEFS
	// typedef
	// 	shadow_batched_geometry::build::__l9::sort_predicate
	// 	shadow_batched_geometry::build::__l9::sort_predicate;

	// typedef
	// 	shadow_batched_geometry::build::__l2::surface_set
	// 	shadow_batched_geometry::build::__l2::surface_set;

	// typedef
	// 	vector< `shadow_batched_geometry::build'::`2'::surface_set >
	// 	surfaces_type;

	// ******

	// CALL SITE INFO
	// <0x70b74e> -> void < unknown >( float4x4 const*, float3 const*, vector< render_surface_instance* >&, bool, u8, u32 )
	// <0x70b852> -> void < unknown >( batched_geometry_interface*, float4x4 const& )
	// ******

	// FUNCTION BODY[0x70b6f0]: 47
	// <0x70b6f7>|0x007|+0x00a:'47'
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
	// <0x70b701>|0x011|+0x006:'58'
	// <0x70b707>|0x017|+0x0e5:'59'
	// <0x70b7ec>|0x0fc|-0x0bc:'59'
	// <0>
	// <1>
	// <2>
	// <0x70b730>|0x040|+0x020:'63'
	// <0>
	// <0x70b750>|0x060|+0x004:'65'
	// <0x70b754>|0x064|+0x00e:'66'
	// <0>
	// <1>
	// <0x70b762>|0x072|+0x00d:'69'
	// <0>
	// <1>
	// <2>
	// <0x70b76f>|0x07f|+0x012:'73'
	// <0x70b781>|0x091|+0x042:'74'
	// <0>
	// <1>
	// <0x70b7c3>|0x0d3|+0x02d:'77'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <0x70b7f0>|0x100|+0x04e:'85'
	// <0>
	// <0x70b83e>|0x14e|+0x006:'87'
	// <0>
	// <1>
	// <0x70b844>|0x154|+0x017:'90'
	// <0>
	// <1>
	// <0x70b85b>|0x16b|+0x00a:'93'
	// ******
}

// STATE[STUB]
bool `vostok::render::shadow_batched_geometry::build'::`9'::sort_predicate::operator()(
	shadow_batched_geometry::build::__l2::surface_set const&	arg_0 /* vostok::render::shadow_batched_geometry::build::__l2::surface_set const& left */,
	shadow_batched_geometry::build::__l2::surface_set const&	arg_1 /* vostok::render::shadow_batched_geometry::build::__l2::surface_set const& right */
) const
{
	return false;

	// FUNCTION BODY[0x70b630]: 1
	// <0x70b630>|0x000|+0x016:'81'
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
	// 	vostok::render::shader_constant*
	// 	iterator_type;

	// typedef
	// 	vostok::render::shader_constant_binding*
	// 	iterator_type;

	// typedef
	// 	vostok::render::shader_constant_buffer_ptr*
	// 	iterator_type;

	// typedef
	// 	vostok::render::shadow_batched_geometry::build::__l2::surface_set*
	// 	iterator_type;

	// typedef
	// 	vostok::render::shadow_vertex*
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
