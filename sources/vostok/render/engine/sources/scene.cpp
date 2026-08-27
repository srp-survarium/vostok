#include "pch.h"
#include "scene.h"
#include <vostok/console_command.h>
#include <vostok/collision/api.h>
#include <vostok/collision/space_partitioning_tree.h>
#include <vostok/render/core/backend.h>
#include <vostok/render/core/options.h>
#include <vostok/render/core/resource_manager.h>
#include <vostok/render/core/texture_named_instance.h>
#include <vostok/render/facade/common_types.h>
#include "ambient_volume.h"
#include "clouds.h"
#include "environment_probe.h"
#include "find_by_id_predicate.h"
#include "grass_world.h"
#include "material_manager.h"
#include "material_effects.h"
#include "moved_object_predicate_helper.h"
#include "portal_sector_system.h"
#include "render_particle_emitter_instance.h"
#include "render_surface.h"
#include "render_surface_instance.h"
#include "sky_ambient_occlusion.h"
#include "speedtree_forest.h"
#include "statistics.h"
#include "streaming.h"
#include "system_renderer.h"

static const u32 s_max_vertex_count	= 64*1024;

static bool s_use_poral_culling_value = true;
static vostok::console_commands::cc_bool s_use_poral_culling_cc(
	"r_use_portal_culling",
	s_use_poral_culling_value,
	false,
	vostok::console_commands::command_type_engine_internal
);

namespace vostok {
namespace render {

struct find_requested_texture_predicate {
	explicit find_requested_texture_predicate( res_texture_ptr const& texture ) :
		m_texture( texture )
	{
	}

	bool operator()( requested_streamable_texture const& other ) const
	{
		return other.texture == m_texture;
	}
	res_texture_ptr	m_texture;
};

STATIC_SIZE_ASSERT( find_requested_texture_predicate, 0x4 );

bool find_environment_probe_predicate::operator()( environment_probe const* const object )
{
	return object->m_id == m_id;
}

particle::render_particle_emitter_instance* scene::particle_engine::create_render_emitter_instance(
	particle::particle_emitter_instance&	particle_emitter_instance,
	particle::base_particle_list const&		particle_list,
	particle::billboard_parameters*			billboard_parameters,
	particle::beamtrail_parameters*			beamtrail_parameters,
	particle::enum_particle_locked_axis		locked_axis,
	particle::enum_particle_screen_alignment	screen_alignment,
	float4x4 const&							transform,
	float4 const&							instance_color
)
{
	return
		MT_NEW( render_particle_emitter_instance ) (
			*m_particle_world,
			particle_emitter_instance,
			particle_list,
			billboard_parameters,
			beamtrail_parameters,
			locked_axis,
			screen_alignment,
			transform,
			instance_color
		);

}

void scene::particle_engine::destroy( particle::render_particle_emitter_instance*& instance )
{
	render::render_particle_emitter_instance* render_instance = static_cast_checked< render::render_particle_emitter_instance*>(instance);
	MT_DELETE			( render_instance );
	instance			= 0;

}

base_scene_ptr scene::particle_engine::get_scene( particle::world& world )
{
	VOSTOK_UNREFERENCED_PARAMETER( world );
	return base_scene_ptr( m_scene );
}

scene::scene( scene_configuration const& renderer_configuration ) :
	m_particle_engine	( this ),
	m_decals_tree		( &*collision::new_space_partitioning_tree( g_allocator, 1.f, 1024 ) ),
	m_environment_probes_tree( &*collision::new_space_partitioning_tree( g_allocator, 1.f, 1024 ) ),
	m_models_tree		( &*collision::new_space_partitioning_tree( g_allocator, 1.f, 1024 ) ),
	m_lights			( NEW(lights_db) ),
	m_particle_world	( NULL ),
	m_speedtree_forest	( renderer_configuration.m_create_speedtree_world ? NEW(speedtree_forest) : NULL ),
	m_grass				( renderer_configuration.m_create_grass_world ? NEW(grass_world) : NULL ),
	m_clouds			( NULL ),
	m_scene_slomo		( 1.0f ),
	m_sky_enabled		( renderer_configuration.m_sky_enabled ),
	m_use_occlusion_culling( renderer_configuration.m_use_occlusion_culling ),
	m_portal_system		( NULL )
{
}

scene::~scene( )
{
	if ( m_sky_material.c_ptr( ) )
		material_manager::ref( ).remove_material_effects( m_sky_material );

	DELETE			( m_speedtree_forest );
	DELETE			( m_grass );

	collision::delete_space_partitioning_tree( m_decals_tree );
	collision::delete_space_partitioning_tree( m_models_tree );
	collision::delete_space_partitioning_tree( m_environment_probes_tree );
}

void scene::set_sky_material( material_effects_instance_ptr const& in_material )
{
	if ( m_sky_material.c_ptr( ) )
		material_manager::ref( ).remove_material_effects( m_sky_material );

	m_sky_material			= in_material;

	if ( m_sky_material )
		material_manager::ref( ).add_material_effects( m_sky_material, in_material->get_material_name( ) );
}

void scene::add_clouds( cloud_parameters const& parameters )
{
	if ( m_clouds )
	{
		DELETE				( m_clouds );
	}
	m_clouds				= NEW( clouds );
	m_clouds->initialize	( parameters );
}

void scene::update_clouds( cloud_parameters const& parameters )
{
	if ( !m_clouds )
	{
		add_clouds			( parameters );
		return;
	}

	m_clouds->initialize	( parameters );

}

void scene::remove_clouds( )
{
	DELETE					( m_clouds );

	m_clouds				= NULL;
}

void scene::build_lpv_geometry( )
{
	m_lpv_geometry.build	( m_render_model_instances );
}

void scene::on_fs_iterator_probes_ready( char* path, vfs::vfs_locked_iterator const& fs_it )
{
	if ( fs_it.is_end() || !fs_it.get_children_count() )
		return;

	vfs::vfs_iterator names_it	= fs_it.children_begin();
	vfs::vfs_iterator names_end	= fs_it.children_end();

	vector< environment_probe* >::iterator probes_it	= m_environment_probes.begin();
	vector< environment_probe* >::iterator probes_end	= m_environment_probes.end();

	typedef vector< fixed_string< 260 > > file_names_array;
	file_names_array used_names;
	file_names_array remove_names;

	for ( ; probes_it != probes_end; ++probes_it )
	{
		environment_probe* probe = *probes_it;
		fixed_string< 260 > probe_name;
		u32 const slash_position = probe->m_properties.texture_name.rfind( '/' );
		probe_name = probe->m_properties.texture_name.c_str() + slash_position + 1;
		used_names.push_back( probe_name );
	}

	for ( ; names_it != names_end; ++names_it )
	{
		fixed_string< 260 > name = names_it.get_name();
		name.set_length( name.rfind( '.' ) );

		if ( names_it.is_folder() )
			continue;

		if ( std::find( used_names.begin(), used_names.end(), name ) == used_names.end() &&
			 std::find( remove_names.begin(), remove_names.end(), name ) == remove_names.end() )
			remove_names.push_back( name );
	}

	fixed_string< 260 > probes_path;
	probes_path = m_environment_probes[0]->m_properties.texture_name;
	probes_path.set_length( probes_path.rfind( '/' ) );

	fs_new::synchronous_device_interface const& device = resources::get_synchronous_device();
	file_names_array::iterator it = remove_names.begin();

	for ( ; it != remove_names.end(); ++it )
	{
		fixed_string< 260 > logical_name_tga, logical_name_options;
		logical_name_tga.assignf( "resources.sources/textures/%s/%s.tga", probes_path.c_str(), it->c_str() );
		logical_name_options.assignf( "resources.sources/textures/%s/%s.options", probes_path.c_str(), it->c_str() );

		fs_new::native_path_string physical_path_tga;
		fs_new::native_path_string physical_path_options;
		resources::convert_virtual_to_physical_path( &physical_path_tga, logical_name_tga, resources::sources_mount );
		resources::convert_virtual_to_physical_path( &physical_path_options, logical_name_options, resources::sources_mount );

		device->erase( physical_path_tga );
		device->erase( physical_path_options );
	}

	FREE( path );
}

void scene::remove_unused_environment_cubemaps( )
{
	vector< environment_probe* >::iterator probe_it = m_environment_probes.begin();
	if ( probe_it == m_environment_probes.end() )
		return;

	fixed_string< 260 > path = (*probe_it)->m_properties.texture_name;
	u32 const slash_position = path.rfind( '/' );
	path.set_length( slash_position );

	u32 const path_size = path.length() + 1;
	char* allocated_path = ALLOC( char, path_size );
	memory::zero( allocated_path, path_size );
	memory::copy( allocated_path, path_size, path.c_str(), path_size );

	fixed_string< 260 > query_path;
	query_path.assignf( "resources.sources/textures/%s", allocated_path );

	resources::query_vfs_iterator(
		query_path.c_str(),
		boost::bind( &scene::on_fs_iterator_probes_ready, this, allocated_path, _1 ),
		g_allocator
	);
}

particle::world* scene::particle_world( )
{
	return m_particle_world.c_ptr( );

}

void scene::add_speedtree_instance( speedtree_instance_ptr instance, float4x4 const& transform, bool populate_forest )
{
	m_speedtree_forest->add_instance(instance, transform);
	if(populate_forest)
		m_speedtree_forest->populate_forest();

}

void scene::remove_speedtree_instance( speedtree_instance_ptr instance, bool populate_forest )
{
	m_speedtree_forest->remove_instance(instance);
	if(populate_forest)
		m_speedtree_forest->populate_forest();

}

void scene::set_speedtree_instance_transform( speedtree_instance_ptr instance, float4x4 const& transform, bool populate_forest )
{
	m_speedtree_forest->set_transform(instance, transform);
	if(populate_forest)
		m_speedtree_forest->populate_forest();

}

void scene::populate_speedtree_forest( )
{
	m_speedtree_forest->populate_forest();

}

void scene::add_tracer( tracer_model_instance_ptr const& instance, float4x4 const& initialize_transform )
{
	instance->m_transform		= initialize_transform;
	m_tracers.push_back			( instance );
}

void scene::update_tracer( tracer_model_instance_ptr const& instance, float4x4 const& new_transform )
{
	vector< tracer_model_instance_ptr >::iterator	it	= std::find( m_tracers.begin( ), m_tracers.end( ), instance );
	ASSERT						( it != m_tracers.end( ) );

	( *it )->m_transform		= new_transform;

}

void scene::remove_tracer( tracer_model_instance_ptr const& instance )
{
	vector< tracer_model_instance_ptr >::iterator	it	= std::find( m_tracers.begin( ), m_tracers.end( ), instance );
	ASSERT						( it != m_tracers.end( ) );
	m_tracers.erase				( it );
}

void scene::process_streaming(
	float4x4 const&		projection_matrix,
	float3				viewer_position,
	const u32			screen_size_x,
	const u32			screen_size_y
)
{
	struct remove_texture_predicate {
		bool operator()( streamable_texture_info const& info )
		{
			return info.texture->m_loaded && info.texture->m_reference_count == 1;
		}
	};

	streaming_textures.erase(
		std::remove_if( streaming_textures.begin( ), streaming_textures.end( ), remove_texture_predicate( ) ),
		streaming_textures.end( )
	);

	struct remove_requested_texture_predicate {
		bool operator()( requested_streamable_texture const& req )
		{
			return req.texture.c_ptr( ) && req.texture->m_reference_count == 1;
		}
	};

	requested_streamable_textures.erase(
		std::remove_if(
			requested_streamable_textures.begin( ),
			requested_streamable_textures.end( ),
			remove_requested_texture_predicate( )
		),
		requested_streamable_textures.end( )
	);

	streamable_texture_info* info_it	= streaming_textures.begin( );
	streamable_texture_info* info_end	= streaming_textures.end( );
	for ( ; info_it != info_end; ++info_it )
	{
		requested_streamable_texture* requested_it = std::find_if(
			requested_streamable_textures.begin( ),
			requested_streamable_textures.end( ),
			find_requested_texture_predicate( info_it->texture )
		);

		u32 num_mips = info_it->texture->num_mips;
		if ( requested_it != requested_streamable_textures.end( ) )
		{
			continue;
		}

		u32 num_wanted_mips = 0;
		float distance;
		for (
			streaming_texture_instance* instance_it = info_it->instances.begin( );
			instance_it != info_it->instances.end( );
			++instance_it
		)
		{
			num_wanted_mips = math::max(
				num_wanted_mips,
				(u32)calculate_needed_texture_mip_levels(
					projection_matrix,
					viewer_position,
					instance_it->object_sphere,
					screen_size_x,
					screen_size_y,
					instance_it->texel_factor,
					distance
				)
			);
		}

		if ( num_wanted_mips == num_mips )
		{
			continue;
		}

		requested_streamable_texture texture_request;
		texture_request.texture		= info_it->texture;
		texture_request.num_mips	= num_wanted_mips;
		texture_request.path		= info_it->path;
		requested_streamable_textures.push_back( texture_request );

		fs_new::virtual_path_string path;
		fs_new::virtual_path_string path_add( texture_request.path.c_str( ) );
		path.assignf( "%s/%s.dds", "resources/textures", path_add.c_str( ) );

		resources::request requests[] = {
			{ path.c_str( ), resources::texture_wrapper_class }
		};
		resources::query_resources(
			requests,
			array_size( requests ),
			boost::bind(
				&scene::on_texture_loaded,
				this,
				_1,
				texture_request.texture,
				num_wanted_mips,
				distance
			),
			g_allocator
		);
	}

	if ( ready_streaming_textures.empty( ) )
		return;

	struct ready_texture_comparer {
		bool operator()( streaming_ready_texture const& left, streaming_ready_texture const& right )
		{
			return left.distance > right.distance;
		}
	};

	std::sort(
		ready_streaming_textures.begin( ),
		ready_streaming_textures.end( ),
		ready_texture_comparer( )
	);

	u32 index = 0;
	timing::timer creation_timer;
	float creation_time = 0.f;
	while ( !ready_streaming_textures.empty( ) && index < 4 && creation_time <= 2.f )
	{
		creation_timer.start( );
		streaming_ready_texture* first = ready_streaming_textures.begin( );
		resource_manager::ref( ).on_texture_loaded( first->data, first->name.c_str( ), first->num_mips );
		creation_time += creation_timer.get_elapsed_sec( ) * 1000.f;

		requested_streamable_texture* requested_it = std::find_if(
			requested_streamable_textures.begin( ),
			requested_streamable_textures.end( ),
			find_requested_texture_predicate( first->texture )
		);
		if ( requested_it != requested_streamable_textures.end( ) )
			requested_streamable_textures.erase( requested_it );

		ready_streaming_textures.erase( ready_streaming_textures.begin( ) );
		++index;
	}
}

void scene::add_trample( trample_desc const& desc )
{
	if ( !m_grass )
		return;

	m_grass->add_trample	( desc );
}

void scene::on_texture_loaded(
	resources::queries_result&		data,
	res_texture_ptr					texture,
	u32								num_mips,
	float							distance
)
{
	requested_streamable_texture* requested_it = std::find_if(
		requested_streamable_textures.begin( ),
		requested_streamable_textures.end( ),
		find_requested_texture_predicate( texture )
	);
	if ( requested_it == requested_streamable_textures.end( ) || !data[0].is_successful( ) )
		return;

	streaming_ready_texture ready_texture;
	ready_texture.texture	= texture;
	ready_texture.data		= data[0].get_managed_resource( );
	ready_texture.name		= data[0].get_requested_path( );
	ready_texture.num_mips	= num_mips;
	ready_texture.distance	= distance;
	ready_streaming_textures.push_back( ready_texture );
}

void scene::gather_streamable_textures( render_model_instance_impl_ptr model, bool update_only )
{
	struct find_texture_predicate {
		explicit find_texture_predicate( res_texture* texture ) :
			m_texture( texture )
		{
		}

		bool operator()( streamable_texture_info const& other ) const
		{
			return other.texture == m_texture;
		}

		res_texture* m_texture;
	};

	vector< render_surface_instance* > surfaces;
	float3 temp_view_pos( 0.f, 0.f, 0.f );
	float4x4 temp_vp_matrix = float4x4( ).identity( );
	model->get_surfaces( &temp_vp_matrix, &temp_view_pos, surfaces, false, 0xaa, 3 );

	render_surface_instance** it = surfaces.begin( );
	render_surface_instance** end = surfaces.end( );
	for ( ; it != end; ++it )
	{
		render_surface_instance* surface_instance = *it;

		math::aabb bbox = surface_instance->m_render_surface->m_aabbox;
		bbox.modify( *surface_instance->m_transform );

		vector< texture_named_instance > effect_used_textures;
		surface_instance->m_render_surface->get_material_effects( ).get_used_textures(
			effect_used_textures
		);

		streaming_texture_instance texture_instance;
		texture_instance.object_sphere = bbox.sphere( );
		texture_instance.texel_factor = surface_instance->m_render_surface->m_streaming_texture_factor;
		texture_instance.surface_instance = surface_instance;

		texture_named_instance* tex_it = effect_used_textures.begin( );
		for ( ; tex_it != effect_used_textures.end( ); ++tex_it )
		{
			streamable_texture_info* info_it = std::find_if(
				streaming_textures.begin( ),
				streaming_textures.end( ),
				find_texture_predicate( tex_it->texture )
			);

			if ( info_it == streaming_textures.end( ) )
			{
				streamable_texture_info info;
				info.texture = tex_it->texture;
				info.path = tex_it->path;
				info.instances.push_back( texture_instance );
				streaming_textures.push_back( info );
				continue;
			}

			streaming_texture_instance* instance_it = update_only ?
				std::find(
					info_it->instances.begin( ),
					info_it->instances.end( ),
					texture_instance
				) :
				info_it->instances.end( );
			if ( instance_it == info_it->instances.end( ) )
				info_it->instances.push_back( texture_instance );
			else
				instance_it->object_sphere = texture_instance.object_sphere;
		}
	}
}

void scene::add_model( render_model_instance_impl_ptr v )
{
	R_ASSERT( std::find(m_render_model_instances.begin(), m_render_model_instances.end(), v) == m_render_model_instances.end());
	m_render_model_instances.push_back		( v );
	m_models_tree->insert( &v->m_collision_object, v->transform() );

	gather_streamable_textures				( v, false );
}

void scene::modify_model( render_model_instance_impl_ptr v )
{
	R_ASSERT					( std::find(m_render_model_instances.begin(), m_render_model_instances.end(), v) != m_render_model_instances.end());
	m_models_tree->move			( &v->m_collision_object, v->transform() );

	gather_streamable_textures	( v, true );
}

void scene::remove_model( render_model_instance_impl_ptr v )
{
	vector< render_model_instance_impl_ptr >::iterator it	= std::find(m_render_model_instances.begin(), m_render_model_instances.end(), v);
	R_ASSERT					( it != m_render_model_instances.end() );
	m_models_tree->erase		( &v->m_collision_object);
	m_render_model_instances.erase	( it );

	select_model				( v, false );
}

void scene::unmove_all_models( )
{
	m_models_tree->unmove_all	( );
}

void moved_object_predicate_helper::check_object( collision::object const& obj )
{
	if ( obj.is_moved( ) )
		m_array.push_back		( &obj );
}

void scene::select_models(
	float4x4 const&							mat_vp,
	vector< render_surface_instance* >&		selection,
	float3 const&							view_pos,
	u32										surface_flags,
	bool									moved_only
)
{
	if (!options::ref().current.m_enabled_draw_models)
		return;

	if ( m_portal_system && s_use_poral_culling_value )
	{
		select_models_via_sectors		( mat_vp, view_pos, selection );

		if ( !selection.empty( ) )
			return;
	}
	math::frustum view_frustum (mat_vp);

	selection.clear();

	collision::objects_type query_result(render::g_allocator);

	if ( moved_only )
	{
		moved_object_predicate_helper						helper( query_result );
		boost::function< void ( collision::object const& ) >	callback =
			boost::bind( &moved_object_predicate_helper::check_object, &helper, _1 );

		m_models_tree->cuboid_query( u32(-1), view_frustum, callback);
	}
	else
		m_models_tree->cuboid_query( u32(-1), view_frustum, query_result);

	selection.reserve( selection.size() + query_result.size());

	collision::objects_type::const_iterator end = query_result.end();
	for( collision::objects_type::iterator it = query_result.begin(); it != end; ++it)
	{
		render_model_instance_impl* const model =
			static_cast_checked<render_collision_object<render_model_instance_impl> const*>(*it)->owner( );

		// claude@NOTE: the transformed bounding box is computed and dropped -
		// the target keeps both calls (virtual get_aabb + out-of-line
		// aabb::modify) and never consumes the result.
		model->get_aabb( ).modify		( model->transform( ) );

		u8 const lod_id					= fixed_lod_value == -1 ? u8( -1 ) : u8( fixed_lod_value );

		model->get_surfaces( &mat_vp, &view_pos, selection, true, lod_id, surface_flags );
	}
}

void scene::update_models( )
{
	vector< render_model_instance_impl_ptr >::iterator			it	=	m_render_model_instances.begin();
	vector< render_model_instance_impl_ptr >::const_iterator	end =	m_render_model_instances.end();

	for( ; it !=  end; ++it)
		(*it)->update();
}

void scene::add_light( u32 id, light_props* props )
{
	m_lights->add_light( id, props);

}

void scene::update_light( u32 id, light_props* props )
{
	m_lights->update_light( id, props);

}

void scene::remove_light( u32 id )
{
	m_lights->remove_light( id);

}

void scene::update_environment_probe( u32 id, environment_probe_properties const& properties )
{
	vector< environment_probe* >::iterator	i =
		std::find_if( m_environment_probes.begin( ), m_environment_probes.end( ), find_environment_probe_predicate( id ) );
	if ( i == m_environment_probes.end( ) )
	{
		m_environment_probes.push_back		( NEW( environment_probe )( m_environment_probes_tree, properties, id ) );
		return;
	}

	( *i )->set_properties					( properties );

}

void scene::remove_environment_probe( u32 id )
{
	vector< environment_probe* >::iterator	i =
		std::find_if( m_environment_probes.begin( ), m_environment_probes.end( ), find_environment_probe_predicate( id ) );
	if ( i != m_environment_probes.end( ) )
	{
		DELETE								( *i );
		m_environment_probes.erase			( i );
	}
}

void scene::update_sky_ambient_occlusion( u32 id, sky_ambient_occlusion_properties const& properties )
{
	vector< sky_ambient_occlusion* >::iterator	i =
		std::find_if( m_sky_ao_volumes.begin( ), m_sky_ao_volumes.end( ), find_by_id_predicate< sky_ambient_occlusion >( id ) );

	if ( i == m_sky_ao_volumes.end( ) )
	{
		m_sky_ao_volumes.push_back			( NEW( sky_ambient_occlusion )( properties, id ) );
		return;
	}

	( *i )->set_properties					( properties );

}

void scene::remove_sky_ambient_occlusion( u32 id )
{
	vector< sky_ambient_occlusion* >::iterator	i =
		std::find_if( m_sky_ao_volumes.begin( ), m_sky_ao_volumes.end( ), find_by_id_predicate< sky_ambient_occlusion >( id ) );
	if ( i != m_sky_ao_volumes.end( ) )
	{
		sky_ambient_occlusion* sky_ao		= *i;
		DELETE								( sky_ao );
		m_sky_ao_volumes.erase				( i );
	}
}

void scene::update_ambient_volume( u32 id, ambient_volume_properties const& properties )
{
	vector< ambient_volume* >::iterator		i =
		std::find_if( m_ambient_volumes.begin( ), m_ambient_volumes.end( ), find_by_id_predicate< ambient_volume >( id ) );

	if ( i == m_ambient_volumes.end( ) )
	{
		m_ambient_volumes.push_back			( NEW( ambient_volume )( properties, id ) );
		return;
	}

	( *i )->set_properties					( properties );

}

void scene::remove_ambient_volume( u32 id )
{
	vector< ambient_volume* >::iterator		i =
		std::find_if( m_ambient_volumes.begin( ), m_ambient_volumes.end( ), find_by_id_predicate< ambient_volume >( id ) );
	if ( i != m_ambient_volumes.end( ) )
	{
		ambient_volume* volume				= *i;
		DELETE								( volume );
		m_ambient_volumes.erase				( i );
	}
}

void scene::update_lpv_occluder( u32 id, float4x4 const& transform )
{
	associative_vector< u32, float4x4, vector, std::less< u32 > >::iterator	i = m_lpv_occluders.find( id );
	if ( i == m_lpv_occluders.end( ) )
		m_lpv_occluders.insert			( std::make_pair( id, transform ) );
	else
		i->second						= transform;
}

void scene::remove_lpv_occluder( u32 id )
{
	associative_vector< u32, float4x4, vector, std::less< u32 > >::iterator	i = m_lpv_occluders.find( id );
	if ( i != m_lpv_occluders.end( ) )
		m_lpv_occluders.erase			( i );
}

void scene::add_decal( u32 id, decal_properties const& properties )
{
	m_decals.push_back						(NEW(decal_instance_node)(NEW(decal_instance)(m_decals_tree, properties, id)));

}

void scene::update_decal( u32 id, decal_properties const& properties )
{
	decal_instance_node* instance			= m_decals.front();

	bool found								= false;
	while (instance)
	{
		if (instance->decal->m_id == id)
		{
			instance->decal->set_properties	(properties);
			found							= true;
		}
		instance							= m_decals.get_next_of_object(instance);
	}
	if (!found)
	{
		add_decal							(id, properties);
	}

}

void scene::remove_decal( u32 id )
{

	decal_instance_node* instance			= m_decals.front();

	while (instance)
	{
		if (instance->decal->m_id == id)
		{
			m_decals.erase					(instance);
			DELETE							(instance);
			break;
		}
		instance							= m_decals.get_next_of_object(instance);
	}

}

void scene::add_volume_fog( u32 id, volume_fog_parameters const& in_parameters )
{
	m_volume_fogs.insert			( std::pair< u32, volume_fog_parameters >( id, in_parameters ) );

}

void scene::update_volume_fog( u32 id, volume_fog_parameters const& in_parameters )
{
	associative_vector< u32, volume_fog_parameters, vector, std::less< u32 > >::iterator	i = m_volume_fogs.find( id );
	if ( i != m_volume_fogs.end( ) )
		i->second						= in_parameters;
	else
		m_volume_fogs.insert			( std::pair< u32, volume_fog_parameters >( id, in_parameters ) );
}

void scene::remove_volume_fog( u32 id )
{
	associative_vector< u32, volume_fog_parameters, vector, std::less< u32 > >::iterator	i = m_volume_fogs.find( id );
	if ( i != m_volume_fogs.end( ) )
		m_volume_fogs.erase			( i );
}

void scene::select_volume_fog_instances( float4x4 const& vp, vector< volume_fog_parameters >& out_instances )
{
	for ( associative_vector< u32, volume_fog_parameters, vector, std::less< u32 > >::iterator i = m_volume_fogs.begin( ); i != m_volume_fogs.end( ); ++i )
	{
		math::frustum view_frustum( vp );
		math::aabb bbox = math::create_identity_aabb( );
		bbox.modify( i->second.transform );

		if ( view_frustum.test_inexact( bbox ) == math::intersection_outside )
			continue;

		out_instances.push_back( i->second );
	}
}

void scene::set_slomo( float time_multiplier )
{
	m_scene_slomo = time_multiplier;
}

void scene::update_lines( const u32 add_count )
{
	if ( m_line_indices.size( ) + add_count >= s_max_vertex_count )
		render_lines			( false );

	ASSERT						( m_line_indices.size() + add_count < s_max_vertex_count );

}

void scene::render_lines( bool covering_effect )
{
	if ( m_line_vertices.empty( ) ) {
		ASSERT					( m_line_indices.empty( ), "lines are empty, but not the pairs" );
		return;
	}

	ASSERT						( !m_line_indices.empty( ), "lines aren't empty, but not the pairs" );

	system_renderer::ref().draw_lines	(
		&*m_line_vertices.begin(),
		&*m_line_vertices.end(),
		&*m_line_indices.begin(),
		&*m_line_indices.end(),
		covering_effect
	);

	m_line_vertices.resize		( 0 );
	m_line_indices.resize		( 0 );
}

void scene::update_triangles( const u32 add_count )
{
	R_ASSERT_CMP				( add_count, <=, s_max_vertex_count );
	R_ASSERT_CMP				( m_triangle_vertices.size(), <=, s_max_vertex_count );
	if ( m_triangle_vertices.size( ) + add_count >= s_max_vertex_count )
		render_triangles		( false );

	ASSERT						( m_triangle_vertices.size( ) + add_count < s_max_vertex_count );

}

void scene::render_triangles( bool covering_effect )
{
	if ( m_triangle_vertices.empty( ) )
		return;

	ASSERT						( ( m_triangle_indices.size( ) % 3 ) == 0, "triangle indices count isn't divisible by 3" );

	system_renderer::ref().draw_triangles	(
		&*m_triangle_vertices.begin(),
		&*m_triangle_vertices.end(),
		&*m_triangle_indices.begin(),
		&*m_triangle_indices.end(),
		covering_effect
	);
	m_triangle_vertices.resize	( 0 );
	m_triangle_indices.resize	( 0 );
}

void scene::draw_lines( vectora< vertex_colored > const& vertices, vectora< u16 > const& indices )
{
	ASSERT						( indices.size( ) % 2 == 0 );
	update_lines				( indices.size( ) );

	u16	const n					= ( u16 ) m_line_vertices.size( );
	m_line_vertices.insert		( m_line_vertices.end(), vertices.begin( ), vertices.end( ) );

	u16 const*	i				= indices.begin( );
	u16 const*	e				= indices.end( );
	for ( ; i != e; ++i )
		m_line_indices.push_back( n + *i );

}

void scene::draw_triangles( vectora< vertex_colored > const& vertices, vectora< u16 > const& indices )
{
	update_triangles			( indices.size( ) );

	u16	const n					= ( u16 ) m_triangle_vertices.size( );
	m_triangle_vertices.insert	( m_triangle_vertices.end(), vertices.begin( ), vertices.end( ) );

	u16 const*	i				= indices.begin( );
	u16 const*	e				= indices.end( );
	for ( ; i != e; ++i )
		m_triangle_indices.push_back( n + *i );

	ASSERT						( ( m_triangle_indices.size( ) % 3 ) == 0, "triangle indices count isn't divisible by 3" );

}

void scene::flush(
	boost::function< void( bool ) > const&	on_draw_scene,
	bool	all_depth_used,
	bool	all_depth_unused
)
{
	if ( all_depth_used )
	{
		backend::ref().reset_depth_stencil_target();

		on_draw_scene				( true );
		render_lines				( false );
		render_triangles			( false );
	}

	if ( all_depth_unused )
	{
		backend::ref().reset_depth_stencil_target	( );
		backend::ref().clear_depth_stencil			( D3D_CLEAR_DEPTH|D3D_CLEAR_STENCIL, 1.0f, 0);

		// render selected models
		system_renderer::ref().draw_render_models_selection				( m_selected_models );
		system_renderer::ref().draw_particle_system_instance_selections	( m_particle_system_instances );
		system_renderer::ref().draw_speedtree_instance_selections		( m_speedtree_instances );

		on_draw_scene				( false );
		render_lines				( true );
		render_triangles			( true );
	}
}

void scene::select_model( render_model_instance_impl_ptr const& instance, const bool is_selected )
{
	vector< render_model_instance_impl_ptr >::iterator found	= std::find( m_selected_models.begin(), m_selected_models.end(), instance );
	if ( found == m_selected_models.end() )
	{
		if ( is_selected )
			m_selected_models.push_back		( instance );
		return;
	}

	if ( !is_selected )
		m_selected_models.erase				( found );

}

void scene::set_portal_system( resources::unmanaged_resource_ptr pss )
{
	m_portal_system			= NEW( culling::portal_sector_system )( static_cast_resource_ptr< culling::portal_sector_structure_ptr >( pss ) );
}

void scene::select_models_via_sectors(
	float4x4 const&							mat_vp,
	float3 const&							view_point,
	vector< render_surface_instance* >&		selection
)
{
	m_portal_system->select_models	( m_models_tree, view_point, mat_vp, selection );
}

void scene::draw_portal_system( system_renderer& r, float3 const& view_pos, float4x4 const& vp )
{
	if ( m_portal_system )
		m_portal_system->render		( r, view_pos, vp );
}

void scene::test_action_portal_system( )
{
	if ( m_portal_system )
		m_portal_system->test_action( );
}

void scene::set_grass( grass_world* w )
{
	m_grass					= w;
}

void scene::reset_grass( grass_world* w )
{
	ASSERT					( m_grass == w );
	m_grass					= NULL;
}

void scene::dump_scene_statistics( ) const
{
	struct sort_predicate {
		bool operator()(
			render_model_instance_impl_ptr const& left,
			render_model_instance_impl_ptr const& right
		) const
		{
			return left->get_surfaces_count( ) > right->get_surfaces_count( );
		}
	};

	vector< render_model_instance_impl_ptr > dump_instances = m_render_model_instances;
	std::sort( dump_instances.begin( ), dump_instances.end( ), sort_predicate( ) );

	render_model_instance_impl_ptr* it = dump_instances.begin( );
	u32 index = 1;
	for ( ; it != dump_instances.end( ); ++it, ++index )
	{
		fixed_string< 128 > model_name( "<unknown>" );

		if ( model_name.length( ) > 17 )
			model_name = model_name.substr( 17 );
		LOG_INFO( "%d: surfaces: %d, model: %s", index, ( *it )->get_surfaces_count( ), model_name.c_str( ) );
	}
}

} // namespace render
} // namespace vostok
