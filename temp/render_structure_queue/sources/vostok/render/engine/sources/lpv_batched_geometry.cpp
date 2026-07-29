////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "vostok\render\engine\sources\lpv_batched_geometry.h"

namespace vostok {
namespace render {

// STATE[STUB]
 lpv_batched_geometry::lpv_batched_geometry( )
{
	// FUNCTION BODY[0x784970]: 0
	// ******
}

// STATE[STUB]
void lpv_batched_geometry::add_vertex( batched_vertex_source const& in_vertex, float3 const& __formal )
{
	// LOCALS
	// lpv_vertex 						vertex
	// ******

	// FUNCTION BODY[0x784590]: 5
	// <0>
	// <0x784593>|0x003|+0x00f:'39'
	// <0x7845a2>|0x012|+0x004:'40'
	// <0>
	// <0x7845a6>|0x016|+0x03c:'42'
	// <0x7845e2>|0x052|-0x006:'42'
	// <0x7845dc>|0x04c|+0x011:'43'
	// ******
}

// STATE[STUB]
void lpv_batched_geometry::build( vector< render_model_instance_impl_ptr >& model_instances )
{
	// LOCALS
	// vector< float4x4 > 				matrices
	// vector< render_surface* > 		surfaces
	// render_model_instance_impl_ptr* 	end
	// render_model_instance_impl_ptr* 	it
	// vector< render_surface_instance* > model_surfaces
	// render_surface_instance** 		end_surf
	// render_surface_instance** 		it_surf
	// ******

	// TYPEDEFS
	// typedef
	// 	vector< float4x4 >
	// 	surface_matrices_type;

	// typedef
	// 	vector< render_surface* >
	// 	surfaces_type;

	// ******

	// CALL SITE INFO
	// <0x784664> -> void < unknown >( float4x4 const*, float3 const*, vector< render_surface_instance* >&, bool, u8, u32 )
	// <0x78491c> -> void < unknown >( batched_geometry_interface*, float4x4 const& )
	// ******

	// OTHER SYMBOLS
	// Label(LabelSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x773970 }, flags: ProcedureFlags { nofpo: false, int: false, far: false, never: false, notreached: true, cust_call: false, noinline: false, optdbginfo: false }, name: RawString("$LN643") })
	// Label(LabelSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x773911 }, flags: ProcedureFlags { nofpo: false, int: false, far: false, never: false, notreached: true, cust_call: false, noinline: false, optdbginfo: false }, name: RawString("$LN644") })
	// ******

	// FUNCTION BODY[0x784600]: 30
	// <0x784607>|0x007|+0x00a:'47'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <0x784611>|0x011|+0x006:'54'
	// <0x784617>|0x017|+0x02f:'55'
	// <0>
	// <1>
	// <2>
	// <0x784646>|0x046|+0x020:'59'
	// <0>
	// <0x784666>|0x066|+0x004:'61'
	// <0x78466a>|0x06a|+0x01c:'62'
	// <0>
	// <1>
	// <2>
	// <0x784686>|0x086|+0x281:'66'
	// <0x784907>|0x307|-0x179:'66'
	// <0x78478e>|0x18e|+0x16b:'67'
	// <0x7848f9>|0x2f9|-0x023:'67'
	// <0>
	// <1>
	// <0x7848d6>|0x2d6|+0x027:'70'
	// <0>
	// <0x7848fd>|0x2fd|+0x004:'72'
	// <0x784901>|0x301|+0x010:'73'
	// <0x784911>|0x311|+0x002:'73'
	// <0x784913>|0x313|+0x015:'74'
	// <0>
	// <0x784928>|0x328|+0x00a:'76'
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
	// 	vostok::render::lpv_vertex*
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
