#include "pch.h"
#include <vostok/animation/anim_track_common.h>
#include <vostok/render/core/backend.h>
#include "render_model_skeleton.h"

namespace vostok {
namespace render {

skeleton_render_surface::skeleton_render_surface( )
{
	m_vertex_input_type=skeletal_4_bones_mesh_vertex_input_type;

}

skeleton_render_model::skeleton_render_model( )
{
	m_bones_matrices_shader_constant	=
		backend::ref().register_constant_host( "bones_matrices",  rc_float );
	m_prev_bones_matrices_shader_constant	=
		backend::ref().register_constant_host( "prev_bones_matrices",  rc_float );

}

void skeleton_render_model::get_bind_pose( float4x4* matrices, u32 count ) const
{
	R_ASSERT(count == m_inverted_bones_matrices_in_bind_pose.size());

	for( u32 i=0; i<count; ++i)
	{
		float4x4& m					= matrices[i];
		float4x4 const& bind_matrix_inv	= m_inverted_bones_matrices_in_bind_pose[i];
		m.try_invert					( bind_matrix_inv );
	}

}

void skeleton_render_model::load_bones( memory::reader& bones_chunk )
{
	u16 bone_count				= bones_chunk.r_u16( );

	m_inverted_bones_matrices_in_bind_pose.resize		( bone_count );
	animation::frame			frm;

	for(u16 i=0; i<bone_count; ++i)
	{
		pcstr bone_name			= bones_chunk.r_string( );
		frm.translation			= bones_chunk.r_float3( );
		frm.rotation			= bones_chunk.r_float3( );
		frm.scale				= bones_chunk.r_float3( );

		float4x4 frm_matrix			= frame_matrix( frm );
		float4x4& bind_matrix_inv	= m_inverted_bones_matrices_in_bind_pose[i];
		bind_matrix_inv.try_invert	( frm_matrix );
		LOG_INFO("%s %f %f %f", bone_name, frm_matrix.c.x, frm_matrix.c.y, frm_matrix.c.z);
	}

}

void skeleton_render_model::update( vector< float4x4 > const& bones )
{
	for( u8 i = 0; i < m_childs_count; ++i )
	{
		skeleton_render_surface* s = static_cast< skeleton_render_surface* >( m_childs[i] );
		s->update	( bones );
	}

}

skeleton_render_model_instance::skeleton_render_model_instance( )
:m_surface_instances( NULL ),
m_instances_count	( 0 )
{
}

skeleton_render_model_instance::~skeleton_render_model_instance( )
{
	DELETE_ARRAY	( m_surface_instances );

}

void skeleton_render_model_instance::assign_original( skeleton_render_model_ptr v )
{
	m_original					= v;
	m_bones_matrices.resize		( m_original->m_inverted_bones_matrices_in_bind_pose.size() );
	m_prev_bones_matrices.resize( m_original->m_inverted_bones_matrices_in_bind_pose.size() );

	{
		render::vector< float4x4 >::iterator i			= m_bones_matrices.begin( );
		render::vector< float4x4 >::iterator const e	= m_bones_matrices.end( );
		for( ; i != e; ++i )
			(*i).identity			( );
	}

	{
		render::vector< float4x4 >::iterator i			= m_prev_bones_matrices.begin( );
		render::vector< float4x4 >::iterator const e	= m_prev_bones_matrices.end( );
		for( ; i != e; ++i )
			(*i).identity			( );
	}

	m_instances_count	= m_original->m_childs_count;
	m_surface_instances = NEW_ARRAY( render_surface_instance, m_instances_count );

	for( u8 i = 0; i < m_instances_count; ++i )
	{
		render_surface_instance& info = m_surface_instances[i];
		info.m_parent				= this;
		info.m_render_surface		= m_original->m_childs[i];
		info.m_transform			= &m_transform;
		info.m_flags				= visible_flag | cast_shadow_flag;
	}
}

u32 skeleton_render_model_instance::get_surfaces_count( u32 ) const
{
	return 1;
}

void skeleton_render_model_instance::get_surfaces(
	float4x4 const*							mat_vp,
	float3 const*							view_pos,
	vector< render_surface_instance* >&		list,
	bool									visible_only,
	u8										lod_id,
	u32										surface_flags
)
{

	list.reserve(list.size()+m_instances_count);

	for( u32 i = 0; i < m_instances_count; ++i )
	{
		render_surface_instance* inst = m_surface_instances+i;
		if( !visible_only || (inst->m_flags & surface_flags) )
			list.push_back		( inst );
	}

}

void skeleton_render_model_instance::get_surface_stats( u32 surface_id, surface_stats& stats ) const
{
	R_ASSERT(surface_id<m_instances_count);
	VOSTOK_UNREFERENCED_PARAMETER	( stats );

}

void skeleton_render_model_instance::update( )
{
	m_original->update		( m_bones_matrices );

	for( u32 i = 0; i < m_instances_count; ++i )
	{
		render_surface_instance* inst = m_surface_instances+i;
		if( inst->m_flags & visible_flag )
		{

			render::vector< float4x4 >::const_iterator inv_bones_it		= m_original->m_inverted_bones_matrices_in_bind_pose.begin(),
													   inv_bones_end	= m_original->m_inverted_bones_matrices_in_bind_pose.end(),
													   bones_it			= m_bones_matrices.begin();
			inst->m_render_surface->m_aabbox.zero();

#pragma message (VOSTOK_TODO("IronNick to IronNick: always change parent aabb?"))

			for (; inv_bones_it != inv_bones_end; ++inv_bones_it, ++bones_it)
				inst->m_render_surface->m_aabbox.modify(math::mul4x4(math::invert4x3(*inv_bones_it), math::transpose(*bones_it)).lines[3].xyz());
		}
	}

}

void skeleton_render_model_instance::set_constants( )
{
	u32 const max_bones_count		= 64;
	R_ASSERT_CMP					( m_bones_matrices.size( ), <=, max_bones_count );
	backend::ref().set_vs_constant	( m_original->m_bones_matrices_shader_constant, &*m_bones_matrices.begin(), max_bones_count );
	backend::ref().set_vs_constant	( m_original->m_prev_bones_matrices_shader_constant, &*m_prev_bones_matrices.begin(), max_bones_count );
}

void skeleton_render_model_instance::update_render_matrices( float4x4 const* matrices, u32 count )
{
	R_ASSERT					(m_bones_matrices.size() == count);
	R_ASSERT					( m_original->m_inverted_bones_matrices_in_bind_pose.size() == count );

	for(u32 i=0; i<count; ++i)
	{
		m_prev_bones_matrices[i]	= m_bones_matrices[i];
		m_bones_matrices[i]		= transpose( m_original->m_inverted_bones_matrices_in_bind_pose[i] * matrices[i] );
	}
}

bool skeleton_render_model_instance::get_locator( pcstr locator_name, model_locator_item& result ) const
{

	return m_original->get_locator( locator_name, result );

}

void skeleton_render_model_instance::get_bind_pose( float4x4* matrices, u32 count ) const
{
	m_original->get_bind_pose( matrices, count );

}

} // namespace render
} // namespace vostok
