////////////////////////////////////////////////////////////////////////////
//	Created		: 03.09.2010
//	Author		: Armen Abroyan
//	Copyright (C) GSC Game World - 2010
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "scene_view.h"
#include <vostok/scaleform/sources/flash_movie.h>

namespace vostok {
namespace render {

struct priority_less : public std::binary_function<
	survarium::flash_movie_resource_ptr const&,
	survarium::flash_movie_resource_ptr const&,
	bool
> {
	bool operator()(
		survarium::flash_movie_resource_ptr const& left,
		survarium::flash_movie_resource_ptr const& right
	) const
	{
		return left->movie->GetPriority( ) < right->movie->GetPriority( );
	}
};

scene_view::scene_view( ):
	luminance_pass_index							( 0 ),
	need_recalc_atmosphere						( true ),
	m_view_mode									( lit_view_mode ),
	m_particles_render_mode						( vostok::particle::normal_particle_render_mode ),
	m_use_post_process							( true ),
	m_render_frame_index						( 0 ),
	m_visible_particle_instances				( g_allocator ),
	m_flash_text_manager						( NULL ),
	m_num_frustum_culled_surfaces				( 0 ),
	m_num_frustum_culled_lights					( 0 ),
	m_num_frustum_culled_grass_patches			( 0 ),
	m_num_frustum_culled_particle_instances		( 0 ),
	m_num_frustum_culled_decals					( 0 ),
	m_num_occlusion_culled_surfaces				( 0 ),
	m_num_occlusion_culled_lights				( 0 ),
	m_num_occlusion_culled_grass_patches		( 0 ),
	m_num_occlusion_culled_particle_instances	( 0 ),
	m_num_occlusion_culled_decals				( 0 ),
	m_culling_results							( NULL ),
	m_atmosphere_prev_targets_size				( u32( -1 ) )
{
	m_prev_frame_luminance_parameters	= math::float4(0.0f, 0.0f, 0.0f, 0.0f);
	m_frame_luminance_parameters		= math::float4(0.0f, 0.0f, 0.0f, 0.0f);
	memory::zero						( m_histogram_values );
}

scene_view::~scene_view()
{
}

void scene_view::add_movie( survarium::flash_movie_resource_ptr& movie )
{
	m_flash_movies.push_back( movie );
	std::sort( m_flash_movies.begin( ), m_flash_movies.end( ), priority_less( ) );
}

void scene_view::remove_movie( survarium::flash_movie_resource_ptr& movie )
{
	render::vector< survarium::flash_movie_resource_ptr >::iterator const iterator =
		std::find( m_flash_movies.begin( ), m_flash_movies.end( ), movie );
	m_flash_movies.erase( iterator );
}

void scene_view::add_text_manager( survarium::flash_text_manager* tm )
{
	m_flash_text_manager = tm;
}

void scene_view::remove_text_manager( survarium::flash_text_manager* )
{
	m_flash_text_manager = NULL;
}

} // namespace render
} // namespace vostok
