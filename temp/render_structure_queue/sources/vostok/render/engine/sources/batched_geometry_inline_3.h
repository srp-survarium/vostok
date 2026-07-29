////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef VOSTOK\RENDER\ENGINE\SOURCES\BATCHED_GEOMETRY_INLINE_H_INCLUDED
#define VOSTOK\RENDER\ENGINE\SOURCES\BATCHED_GEOMETRY_INLINE_H_INCLUDED

namespace vostok {
namespace render {

// STATE[STUB]
inline  batched_geometry<vostok::render::lpv_vertex>::~batched_geometry<vostok::render::lpv_vertex>( )
{
	// FUNCTION BODY[0x694f0]: 1
	// <0x694f8>|0x008|+0x00d:'36'
	// ******
}

// STATE[STUB]
inline void batched_geometry<vostok::render::lpv_vertex>::finalize_batch( )
{
	// LOCALS
	// untyped_buffer_ptr 				vb
	// untyped_buffer_ptr 				ib
	// ******

	// FUNCTION BODY[0x6ae50]: 33
	// <0x6ae5d>|0x00d|+0x03d:'167'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <0x6ae9a>|0x04a|+0x046:'175'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <0x6aee0>|0x090|+0x031:'182'
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
	// <0x6af11>|0x0c1|+0x0b9:'196'
	// <0x6afca>|0x17a|+0x06d:'197'
	// <0x6b037>|0x1e7|+0x029:'198'
	// <0x6b060>|0x210|+0x029:'199'
	// ******
}

// STATE[STUB]
inline void batched_geometry<vostok::render::lpv_vertex>::invalidate( )
{
	// FUNCTION BODY[0x6b0d0]: 12
	// <0x6b0d2>|0x002|+0x004:'205'
	// <0x6b0d6>|0x006|+0x008:'206'
	// <0>
	// <0x6b0de>|0x00e|+0x004:'208'
	// <0>
	// <0x6b0e2>|0x012|+0x01c:'210'
	// <0x6b0fe>|0x02e|+0x028:'211'
	// <0>
	// <1>
	// <0x6b126>|0x056|+0x04b:'214'
	// <0x6b171>|0x0a1|+0x029:'215'
	// <0x6b19a>|0x0ca|+0x029:'216'
	// ******
}

// STATE[STUB]
inline void batched_geometry<vostok::render::lpv_vertex>::add_data(
	batched_vertex_source const*	vertices,
	const u32						num_vertices,
	u16 const*						indices,
	const u32						num_indices,
	float4x4 const&					transform,
	material_effects_instance_ptr const&	in_materail_effects_instance
)
{
	// LOCALS
	// float3 							normal
	// float3 							not_modified_pos
	// base_basis 						basis
	// batched_vertex_source 			vertex
	// ******

	// CALL SITE INFO
	// <0x69b19> -> void < unknown >( batched_vertex_source const&, float3 const& )
	// ******

	// FUNCTION BODY[0x69600]: 43
	// <0x6960c>|0x00c|+0x01d:'227'
	// <0>
	// <1>
	// <2>
	// <0x69629>|0x029|+0x029:'231'
	// <0>
	// <0x69652>|0x052|+0x006:'233'
	// <0x69658>|0x058|+0x00e:'234'
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x69666>|0x066|+0x00a:'239'
	// <0x69670>|0x070|+0x055:'240'
	// <0>
	// <0x696c5>|0x0c5|+0x03c:'242'
	// <0x69701>|0x101|+0x004:'242'
	// <0>
	// <0x69705>|0x105|+0x029:'244'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <0x6972e>|0x12e|-0x05b:'250'
	// <0>
	// <0x696d3>|0x0d3|+0x0ec:'252'
	// <0x697bf>|0x1bf|+0x050:'252'
	// <0>
	// <0x6980f>|0x20f|+0x0ca:'254'
	// <0x698d9>|0x2d9|+0x099:'255'
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x69972>|0x372|+0x045:'260'
	// <0x699b7>|0x3b7|+0x00f:'261'
	// <0>
	// <0x699c6>|0x3c6|+0x0ee:'263'
	// <0>
	// <0x69ab4>|0x4b4|+0x051:'265'
	// <0>
	// <0x69b05>|0x505|+0x026:'267'
	// <0>
	// <1>
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
	// 	std::pair< u32, float4x4 >*
	// 	iterator_type;

	// typedef
	// 	std::pair< u32, vostok::render::volume_fog_parameters >*
	// 	iterator_type;

	// typedef
	// 	u16*
	// 	iterator_type;

	// typedef
	// 	void**
	// 	iterator_type;

	// typedef
	// 	vostok::fixed_string< 260 >*
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
	// 	vostok::render::render_model_instance_impl_ptr*
	// 	iterator_type;

	// typedef
	// 	vostok::render::requested_streamable_texture*
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
	// 	vostok::render::shadow_vertex*
	// 	iterator_type;

	// typedef
	// 	vostok::render::signature_layout_pair*
	// 	iterator_type;

	// typedef
	// 	vostok::render::speedtree_instance_ptr*
	// 	iterator_type;

	// typedef
	// 	vostok::render::streamable_texture_info*
	// 	iterator_type;

	// typedef
	// 	vostok::render::streaming_ready_texture*
	// 	iterator_type;

	// typedef
	// 	vostok::render::streaming_texture_instance*
	// 	iterator_type;

	// typedef
	// 	vostok::render::texture_named_instance*
	// 	iterator_type;

	// typedef
	// 	vostok::render::tracer_model_instance_ptr*
	// 	iterator_type;

	// typedef
	// 	vostok::render::vertex_colored*
	// 	iterator_type;

	// typedef
	// 	vostok::render::volume_fog_parameters*
	// 	iterator_type;

	// typedef
	// 	vostok::resources::unmanaged_resource_ptr*
	// 	iterator_type;

	// ******

} // namespace render
} // namespace vostok

#endif // #ifndef VOSTOK\RENDER\ENGINE\SOURCES\BATCHED_GEOMETRY_INLINE_H_INCLUDED
