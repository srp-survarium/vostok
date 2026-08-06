#include "pch.h"
// claude@NOTE: legacy-harvest disposition: every remaining stub is absent from the legacy scene.cpp remainder (only the three select_* helpers survive there; clouds/tracers/probes/sky-AO/ambient-volumes/streaming/portal-system/grass/trample are new-in-target) - matcher-phase work.
#include "scene.h"
#include <vostok/console_command.h>
#include <vostok/collision/api.h>
#include <vostok/collision/space_partitioning_tree.h>
#include <vostok/render/core/backend.h>
#include <vostok/render/core/options.h>
#include <vostok/render/core/resource_manager.h>
#include <vostok/render/facade/common_types.h>
#include "ambient_volume.h"
#include "clouds.h"
#include "environment_probe.h"
#include "find_by_id_predicate.h"
#include "grass_world.h"
#include "material_manager.h"
#include "moved_object_predicate_helper.h"
#include "portal_sector_system.h"
#include "render_particle_emitter_instance.h"
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

private:
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

	// FUNCTION BODY[0x63ef30]: 12
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
	// <0x63ef31>|0x001|+0x046:'78'
	// <0x63ef77>|0x047|-0x003:'78'
	// <0x63ef74>|0x044|+0x006:'79'
	// ******
}

void scene::particle_engine::destroy( particle::render_particle_emitter_instance*& instance )
{
	render::render_particle_emitter_instance* render_instance = static_cast_checked< render::render_particle_emitter_instance*>(instance);
	MT_DELETE			( render_instance );
	instance			= 0;

	// CALL SITE INFO
	// <0x63d431> -> void* < unknown >( u32 )
	// ******

	// FUNCTION BODY[0x63d410]: 3
	// <0x63d411>|0x001|+0x007:'83'
	// <0x63d418>|0x008|+0x028:'84'
	// <0x63d440>|0x030|+0x00b:'85'
	// <0x63d44b>|0x03b|-0x004:'85'
	// <0x63d447>|0x037|+0x00b:'86'
	// ******
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
	DELETE			( m_speedtree_forest );
	DELETE			( m_grass );

	collision::delete_space_partitioning_tree( m_decals_tree );
	collision::delete_space_partitioning_tree( m_environment_probes_tree );
	collision::delete_space_partitioning_tree( m_models_tree );

	// FUNCTION BODY[0x63f110]: 9
	// <0x63f117>|0x007|+0x018:'122'
	// <0x63f12f>|0x01f|+0x00c:'123'
	// <0>
	// <0x63f13b>|0x02b|+0x02d:'125'
	// <0x63f168>|0x058|+0x03f:'126'
	// <0>
	// <0x63f1a7>|0x097|+0x035:'128'
	// <0x63f1dc>|0x0cc|+0x035:'129'
	// <0x63f211>|0x101|+0x035:'130'
	// ******
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

// STATE[STUB]
void scene::on_fs_iterator_probes_ready( char* path, vfs::vfs_locked_iterator const& fs_it )
{
	// LOCALS
	// vfs::vfs_iterator 				names_end
	// vector< fixed_string< 260 > > 	remove_names
	// fixed_string< 260 > 				probes_path
	// vector< fixed_string< 260 > > 	used_names
	// vfs::vfs_iterator 				names_it
	// fixed_string< 260 > 				probe_name
	// fixed_string< 260 > 				name
	// fs_new::synchronous_device_interface const& device
	// fixed_string< 260 > 				logical_name_options
	// fs_new::native_path_string 		physical_path_tga
	// fs_new::native_path_string 		physical_path_options
	// fixed_string< 260 > 				logical_name_tga
	// ******

	// TYPEDEFS
	// typedef
	// 	vector< fixed_string< 260 > >
	// 	file_names_array;

	// ******

	// FUNCTION BODY[0x63d990]: 62
	// <0x63d99e>|0x00e|+0x024:'188'
	// <0>
	// <1>
	// <0x63d9c2>|0x032|+0x00c:'191'
	// <0x63d9ce>|0x03e|+0x00c:'192'
	// <0>
	// <0x63d9da>|0x04a|+0x006:'194'
	// <0x63d9e0>|0x050|+0x006:'195'
	// <0>
	// <1>
	// <2>
	// <0x63d9e6>|0x056|+0x00c:'199'
	// <0x63d9f2>|0x062|+0x00c:'200'
	// <0>
	// <0x63d9fe>|0x06e|+0x008:'202'
	// <0>
	// <0x63da06>|0x076|+0x002:'204'
	// <0x63da08>|0x078|+0x01d:'205'
	// <0x63da25>|0x095|+0x021:'206'
	// <0x63da46>|0x0b6|+0x034:'207'
	// <0x63da7a>|0x0ea|+0x019:'208'
	// <0>
	// <1>
	// <0x63da93>|0x103|+0x01d:'211'
	// <0>
	// <0x63dab0>|0x120|+0x050:'213'
	// <0x63db00>|0x170|+0x027:'214'
	// <0>
	// <0x63db27>|0x197|+0x00f:'216'
	// <0>
	// <1>
	// <2>
	// <0x63db36>|0x1a6|+0x034:'220'
	// <0x63db6a>|0x1da|+0x035:'221'
	// <0>
	// <1>
	// <0x63db9f>|0x20f|+0x010:'224'
	// <0x63dbaf>|0x21f|+0x038:'225'
	// <0x63dbe7>|0x257|+0x02a:'226'
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x63dc11>|0x281|+0x015:'231'
	// <0>
	// <1>
	// <0x63dc26>|0x296|+0x010:'234'
	// <0>
	// <0x63dc36>|0x2a6|+0x01e:'236'
	// <0x63dc54>|0x2c4|+0x045:'237'
	// <0x63dc99>|0x309|+0x01d:'238'
	// <0>
	// <1>
	// <0x63dcb6>|0x326|+0x1b9:'241'
	// <0x63de6f>|0x4df|+0x157:'242'
	// <0>
	// <0x63dfc6>|0x636|+0x021:'244'
	// <0x63dfe7>|0x657|+0x02d:'245'
	// <0>
	// <1>
	// <2>
	// <0x63e014>|0x684|+0x01e:'249'
	// ******
}

// STATE[STUB]
void scene::remove_unused_environment_cubemaps( )
{
	// LOCALS
	// fixed_string< 260 > 				path
	// fixed_string< 260 > 				query_path
	// ******

	// FUNCTION BODY[0x63ff70]: 25
	// <0x63ff80>|0x010|+0x008:'254'
	// <0>
	// <1>
	// <0x63ff88>|0x018|+0x00c:'257'
	// <0>
	// <1>
	// <0x63ff94>|0x024|+0x03e:'260'
	// <0x63ffd2>|0x062|+0x026:'261'
	// <0x63fff8>|0x088|+0x009:'262'
	// <0>
	// <0x640001>|0x091|+0x008:'264'
	// <0x640009>|0x099|+0x00d:'265'
	// <0x640016>|0x0a6|+0x00b:'266'
	// <0x640021>|0x0b1|+0x012:'267'
	// <0>
	// <0x640033>|0x0c3|+0x009:'269'
	// <0x64003c>|0x0cc|+0x037:'270'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <0x640073>|0x103|+0x0e3:'277'
	// <0>
	// ******
}

particle::world* scene::particle_world( )
{
	return m_particle_world.c_ptr( );

	// FUNCTION BODY[0x63cf90]: 1
	// <0x63cf90>|0x000|+0x006:'283'
	// ******
}

void scene::add_speedtree_instance( speedtree_instance_ptr instance, float4x4 const& transform, bool populate_forest )
{
	m_speedtree_forest->add_instance(instance, transform);
	if(populate_forest)
		m_speedtree_forest->populate_forest();

	// FUNCTION BODY[0x63f090]: 3
	// <0x63f090>|0x000|+0x036:'288'
	// <0x63f0c6>|0x036|+0x007:'289'
	// <0x63f0cd>|0x03d|+0x00c:'290'
	// ******
}

void scene::remove_speedtree_instance( speedtree_instance_ptr instance, bool populate_forest )
{
	m_speedtree_forest->remove_instance(instance);
	if(populate_forest)
		m_speedtree_forest->populate_forest();

	// FUNCTION BODY[0x63f010]: 3
	// <0x63f016>|0x006|+0x033:'295'
	// <0x63f049>|0x039|+0x006:'296'
	// <0x63f04f>|0x03f|+0x00c:'297'
	// ******
}

void scene::set_speedtree_instance_transform( speedtree_instance_ptr instance, float4x4 const& transform, bool populate_forest )
{
	m_speedtree_forest->set_transform(instance, transform);
	if(populate_forest)
		m_speedtree_forest->populate_forest();

	// FUNCTION BODY[0x63ef90]: 3
	// <0x63ef90>|0x000|+0x039:'302'
	// <0x63efc9>|0x039|+0x007:'303'
	// <0x63efd0>|0x040|+0x00c:'304'
	// ******
}

void scene::populate_speedtree_forest( )
{
	m_speedtree_forest->populate_forest();

	// FUNCTION BODY[0x63ef80]: 1
	// <0x63ef80>|0x000|+0x00c:'309'
	// ******
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
		bool operator()( requested_streamable_texture const& texture )
		{
			return texture.texture && texture.texture->m_reference_count == 1;
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
			continue;

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
			continue;

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

// STATE[STUB]
void scene::gather_streamable_textures( render_model_instance_impl_ptr model, bool update_only )
{
	// LOCALS
	// float3 							temp_view_pos
	// render_surface_instance** 		end
	// vector< render_surface_instance* > surfaces
	// float4x4 						temp_vp_matrix
	// render_surface_instance** 		it
	// math::aabb 						bbox
	// streaming_texture_instance 		texture_instance
	// vector< texture_named_instance > effect_used_textures
	// texture_named_instance* 			tex_it
	// streamable_texture_info 			info
	// ******

	// TYPEDEFS
	// typedef
	// 	scene::gather_streamable_textures::__l2::find_texture_predicate
	// 	scene::gather_streamable_textures::__l2::find_texture_predicate;

	// ******

	// CALL SITE INFO
	// <0x63e33c> -> void < unknown >( float4x4 const*, float3 const*, vector< render_surface_instance* >&, bool, u8, u32 )
	// ******

	// FUNCTION BODY[0x63e2c0]: 79
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
	// <0x63e2cc>|0x00c|+0x008:'556'
	// <0x63e2d4>|0x014|+0x033:'557'
	// <0>
	// <0x63e307>|0x047|+0x037:'559'
	// <0>
	// <0x63e33e>|0x07e|+0x004:'561'
	// <0x63e342>|0x082|+0x00c:'562'
	// <0>
	// <0x63e34e>|0x08e|+0x4b9:'564'
	// <0x63e807>|0x547|-0x4b1:'564'
	// <0>
	// <0x63e356>|0x096|+0x006:'566'
	// <0>
	// <0x63e35c>|0x09c|+0x007:'568'
	// <0x63e363>|0x0a3|+0x02b:'569'
	// <0>
	// <0x63e38e>|0x0ce|+0x027:'571'
	// <0>
	// <1>
	// <0x63e3b5>|0x0f5|+0x016:'574'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <0x63e3cb>|0x10b|+0x0b3:'581'
	// <0x63e47e>|0x1be|+0x025:'582'
	// <0x63e4a3>|0x1e3|+0x004:'583'
	// <0>
	// <0x63e4a7>|0x1e7|+0x00a:'585'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <0x63e4b1>|0x1f1|+0x025:'592'
	// <0>
	// <0x63e4d6>|0x216|+0x008:'594'
	// <0>
	// <0x63e4de>|0x21e|+0x04a:'596'
	// <0x63e528>|0x268|+0x0b6:'597'
	// <0x63e5de>|0x31e|+0x014:'598'
	// <0x63e5f2>|0x332|+0x059:'599'
	// <0>
	// <0x63e64b>|0x38b|+0x03b:'601'
	// <0x63e686>|0x3c6|+0x0ca:'602'
	// <0x63e750>|0x490|+0x002:'603'
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
	// <0x63e752>|0x492|+0x018:'614'
	// <0>
	// <0x63e76a>|0x4aa|+0x004:'616'
	// <0x63e76e>|0x4ae|+0x037:'617'
	// <0x63e7a5>|0x4e5|+0x002:'618'
	// <0x63e7a7>|0x4e7|+0x032:'619'
	// <0>
	// <1>
	// <0x63e7d9>|0x519|+0x032:'622'
	// ******
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
	BEGIN_TIMER(statistics::ref().visibility_stat_group.models_updating_time);

	vector< render_model_instance_impl_ptr >::iterator			it	=	m_render_model_instances.begin();
	vector< render_model_instance_impl_ptr >::const_iterator	end =	m_render_model_instances.end();

	for( ; it !=  end; ++it)
		(*it)->update();

	END_TIMER;

	// CALL SITE INFO
	// <0x63d0e9> -> void < unknown >()
	// ******

	// FUNCTION BODY[0x63d0d0]: 10
	// <0>
	// <1>
	// <2>
	// <0x63d0d1>|0x001|+0x007:'748'
	// <0x63d0d8>|0x008|+0x006:'749'
	// <0>
	// <0x63d0de>|0x00e|+0x004:'751'
	// <0x63d0e2>|0x012|+0x012:'752'
	// <0>
	// <1>
	// ******
}

void scene::add_light( u32 id, light_props* props )
{
	m_lights->add_light( id, props);

	// FUNCTION BODY[0x640a30]: 1
	// <0x640a30>|0x000|+0x011:'759'
	// ******
}

void scene::update_light( u32 id, light_props* props )
{
	m_lights->update_light( id, props);

	// FUNCTION BODY[0x6409e0]: 7
	// <0x6409e3>|0x003|+0x03d:'764'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// ******
}

void scene::remove_light( u32 id )
{
	m_lights->remove_light( id);

	// FUNCTION BODY[0x63ef20]: 1
	// <0x63ef21>|0x001|+0x00e:'775'
	// ******
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
	associative_vector< u32, float4x4, vector, std::less< u32 > >::iterator	i = m_lpv_occluders.lower_bound( id );
	if ( i == m_lpv_occluders.end( ) || id < i->first )
		m_lpv_occluders.insert			( std::make_pair( id, transform ) );
	else
		i->second						= transform;

	// FUNCTION BODY[0x63d330]: 6
	// <0x63d33e>|0x00e|+0x03e:'899'
	// <0>
	// <1>
	// <0x63d37c>|0x04c|+0x035:'902'
	// <0>
	// <0x63d3b1>|0x081|-0x006:'904'
	// <0x63d3ab>|0x07b|+0x013:'905'
	// ******
}

void scene::remove_lpv_occluder( u32 id )
{
	associative_vector< u32, float4x4, vector, std::less< u32 > >::iterator	i = m_lpv_occluders.lower_bound( id );
	if ( i != m_lpv_occluders.end( ) && i->first == id )
		m_lpv_occluders.erase			( i );

	// FUNCTION BODY[0x63d050]: 4
	// <0x63d057>|0x007|+0x041:'909'
	// <0>
	// <0x63d098>|0x048|+0x008:'911'
	// <0x63d0a0>|0x050|+0x020:'912'
	// ******
}

void scene::add_decal( u32 id, decal_properties const& properties )
{
	m_decals.push_back						(NEW(decal_instance_node)(NEW(decal_instance)(m_decals_tree, properties, id)));

	// FUNCTION BODY[0x63e9b0]: 1
	// <0x63e9b1>|0x001|+0x08b:'917'
	// <0x63ea3c>|0x08c|-0x007:'917'
	// <0x63ea35>|0x085|+0x016:'918'
	// ******
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

	// FUNCTION BODY[0x63eec0]: 16
	// <0x63eec6>|0x006|+0x00e:'922'
	// <0>
	// <0x63eed4>|0x014|+0x002:'924'
	// <0x63eed6>|0x016|+0x00a:'925'
	// <0>
	// <0x63eee0>|0x020|+0x00a:'927'
	// <0>
	// <0x63eeea>|0x02a|+0x007:'929'
	// <0x63eef1>|0x031|+0x002:'930'
	// <0>
	// <0x63eef3>|0x033|+0x007:'932'
	// <0>
	// <0x63eefa>|0x03a|+0x004:'934'
	// <0>
	// <0x63eefe>|0x03e|+0x00b:'936'
	// <0>
	// ******
}

void scene::remove_decal( u32 id )
{
	// LOCALS
	// scene::decal_instance_node* 		instance
	// ******

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

	// FUNCTION BODY[0x63d820]: 12
	// <0x63d821>|0x001|+0x009:'942'
	// <0>
	// <0x63d82a>|0x00a|+0x006:'944'
	// <0>
	// <0x63d830>|0x010|+0x018:'946'
	// <0>
	// <0x63d848>|0x028|-0x00e:'948'
	// <0>
	// <1>
	// <2>
	// <0x63d83a>|0x01a|+0x014:'952'
	// <0x63d84e>|0x02e|-0x008:'952'
	// <0>
	// <0x63d846>|0x026|+0x026:'954'
	// ******
}

void scene::add_volume_fog( u32 id, volume_fog_parameters const& in_parameters )
{
	m_volume_fogs.insert			( std::pair< u32, volume_fog_parameters >( id, in_parameters ) );

	// FUNCTION BODY[0x63d2f0]: 1
	// <0x63d2fc>|0x00c|+0x024:'958'
	// ******
}

void scene::update_volume_fog( u32 id, volume_fog_parameters const& in_parameters )
{
	associative_vector< u32, volume_fog_parameters, vector, std::less< u32 > >::iterator	i = m_volume_fogs.lower_bound( id );
	if ( i == m_volume_fogs.end( ) || id < i->first )
		m_volume_fogs.insert			( std::make_pair( id, in_parameters ) );
	else
		i->second						= in_parameters;

	// FUNCTION BODY[0x63d460]: 10
	// <0x63d471>|0x011|+0x03f:'963'
	// <0>
	// <0x63d4b0>|0x050|+0x008:'965'
	// <0>
	// <0x63d4b8>|0x058|+0x016:'967'
	// <0>
	// <1>
	// <2>
	// <0x63d4ce>|0x06e|-0x009:'971'
	// <0>
	// <0x63d4c5>|0x065|+0x02d:'973'
	// ******
}

void scene::remove_volume_fog( u32 id )
{
	associative_vector< u32, volume_fog_parameters, vector, std::less< u32 > >::iterator	i = m_volume_fogs.lower_bound( id );
	if ( i != m_volume_fogs.end( ) && i->first == id )
		m_volume_fogs.erase			( i );

	// FUNCTION BODY[0x63cfd0]: 4
	// <0x63cfd7>|0x007|+0x041:'977'
	// <0>
	// <0x63d018>|0x048|+0x008:'979'
	// <0x63d020>|0x050|+0x020:'980'
	// ******
}

// STATE[STUB]
void scene::select_volume_fog_instances( float4x4 const& vp, vector< volume_fog_parameters >& out_instances )
{
	// LOCALS
	// math::aabb 						bbox
	// math::frustum 					view_frustum
	// ******

	// FUNCTION BODY[0x63d1f0]: 21
	// <0x63d1f0>|0x000|+0x027:'985'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <0x63d217>|0x027|+0x06b:'991'
	// <0x63d282>|0x092|+0x011:'992'
	// <0>
	// <0x63d293>|0x0a3|+0x00f:'994'
	// <0>
	// <1>
	// <0x63d2a2>|0x0b2|+0x039:'997'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <7>
	// ******
}

void scene::set_slomo( float time_multiplier )
{
	m_scene_slomo = time_multiplier;

	// FUNCTION BODY[0x63cee0]: 1
	// <0x63cee0>|0x000|+0x008:'1010'
	// ******
}

void scene::update_lines( const u32 add_count )
{
	if ( m_line_indices.size( ) + add_count >= s_max_vertex_count )
		render_lines			( false );

	ASSERT						( m_line_indices.size() + add_count < s_max_vertex_count );

	// FUNCTION BODY[0x63fa30]: 4
	// <0x63fa30>|0x000|+0x01a:'1015'
	// <0x63fa4a>|0x01a|+0x007:'1016'
	// <0>
	// <1>
	// ******
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

	// FUNCTION BODY[0x63f9b0]: 17
	// <0x63f9b6>|0x006|+0x014:'1023'
	// <0>
	// <0x63f9ca>|0x01a|+0x002:'1025'
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
	// <0x63f9cc>|0x01c|+0x02a:'1036'
	// <0>
	// <0x63f9f6>|0x046|+0x013:'1038'
	// <0x63fa09>|0x059|+0x016:'1039'
	// ******
}

void scene::update_triangles( const u32 add_count )
{
	R_ASSERT_CMP				( add_count, <=, s_max_vertex_count );
	R_ASSERT_CMP				( m_triangle_vertices.size(), <=, s_max_vertex_count );
	if ( m_triangle_vertices.size( ) + add_count >= s_max_vertex_count )
		render_triangles		( false );

	ASSERT						( m_triangle_vertices.size( ) + add_count < s_max_vertex_count );

	// FUNCTION BODY[0x63f980]: 6
	// <0>
	// <1>
	// <0x63f980>|0x000|+0x01b:'1046'
	// <0x63f99b>|0x01b|+0x007:'1047'
	// <0>
	// <1>
	// ******
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

	// FUNCTION BODY[0x63f900]: 14
	// <0x63f906>|0x006|+0x016:'1054'
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
	// <0x63f91c>|0x01c|+0x02b:'1065'
	// <0x63f947>|0x047|+0x013:'1066'
	// <0x63f95a>|0x05a|+0x016:'1067'
	// ******
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

	// LOCALS
	// u16 const* 						e
	// const u16 						n
	// u16 const* 						i
	// ******

	// OTHER SYMBOLS
	// Label(LabelSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x62ee66 }, flags: ProcedureFlags { nofpo: false, int: false, far: false, never: false, notreached: true, cust_call: false, noinline: false, optdbginfo: false }, name: RawString("$LN272") })
	// Label(LabelSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x62ee66 }, flags: ProcedureFlags { nofpo: false, int: false, far: false, never: false, notreached: true, cust_call: false, noinline: false, optdbginfo: false }, name: RawString("$LN274") })
	// ******

	// FUNCTION BODY[0x63fc70]: 10
	// <0>
	// <0x63fc78>|0x008|+0x030:'1073'
	// <0>
	// <0x63fca8>|0x038|+0x00c:'1075'
	// <0x63fcb4>|0x044|+0x04b:'1076'
	// <0>
	// <0x63fcff>|0x08f|+0x002:'1078'
	// <0x63fd01>|0x091|+0x00b:'1079'
	// <0x63fd0c>|0x09c|+0x014:'1080'
	// <0x63fd20>|0x0b0|+0x13c:'1081'
	// <0x63fe5c>|0x1ec|-0x00a:'1081'
	// ******
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

	// LOCALS
	// u16 const* 						e
	// const u16 						n
	// u16 const* 						i
	// ******

	// OTHER SYMBOLS
	// Label(LabelSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x62ec66 }, flags: ProcedureFlags { nofpo: false, int: false, far: false, never: false, notreached: true, cust_call: false, noinline: false, optdbginfo: false }, name: RawString("$LN280") })
	// Label(LabelSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x62ec66 }, flags: ProcedureFlags { nofpo: false, int: false, far: false, never: false, notreached: true, cust_call: false, noinline: false, optdbginfo: false }, name: RawString("$LN282") })
	// ******

	// FUNCTION BODY[0x63fa80]: 11
	// <0x63fa88>|0x008|+0x037:'1086'
	// <0>
	// <0x63fabf>|0x03f|+0x005:'1088'
	// <0x63fac4>|0x044|+0x045:'1089'
	// <0>
	// <0x63fb09>|0x089|+0x002:'1091'
	// <0x63fb0b>|0x08b|+0x00b:'1092'
	// <0x63fb16>|0x096|+0x00a:'1093'
	// <0x63fb20>|0x0a0|+0x13c:'1094'
	// <0x63fc5c>|0x1dc|-0x00a:'1094'
	// <0>
	// <1>
	// ******
}

void scene::flush(
	boost::function< void( bool ) > const&	on_draw_scene,
	bool	all_depth_used,
	bool	all_depth_unused
)
{
	backend::ref().reset_depth_stencil_target();

	on_draw_scene				( true );
	render_lines				( false );
	render_triangles			( false );

	backend::ref().reset_depth_stencil_target	( );
	backend::ref().clear_depth_stencil			( D3D_CLEAR_DEPTH|D3D_CLEAR_STENCIL, 1.0f, 0);

	// render selected models
	system_renderer::ref().draw_render_models_selection				( m_selected_models );
	system_renderer::ref().draw_particle_system_instance_selections	( m_particle_system_instances );
	system_renderer::ref().draw_speedtree_instance_selections		( m_speedtree_instances );

	on_draw_scene				( false );
	render_lines				( true );
	render_triangles			( true );

	// FUNCTION BODY[0x63fe70]: 33
	// <0x63fe70>|0x000|+0x00d:'1101'
	// <0>
	// <0x63fe7d>|0x00d|+0x020:'1103'
	// <0>
	// <0x63fe9d>|0x02d|+0x009:'1105'
	// <0x63fea6>|0x036|+0x009:'1106'
	// <0x63feaf>|0x03f|+0x009:'1107'
	// <0>
	// <1>
	// <0x63feb8>|0x048|+0x00b:'1110'
	// <0>
	// <0x63fec3>|0x053|+0x020:'1112'
	// <0x63fee3>|0x073|+0x02e:'1113'
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
	// <0x63ff11>|0x0a1|+0x012:'1126'
	// <0x63ff23>|0x0b3|+0x012:'1127'
	// <0x63ff35>|0x0c5|+0x012:'1128'
	// <0>
	// <0x63ff47>|0x0d7|+0x009:'1130'
	// <0x63ff50>|0x0e0|+0x009:'1131'
	// <0x63ff59>|0x0e9|+0x009:'1132'
	// <0>
	// ******
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

	// FUNCTION BODY[0x63e980]: 10
	// <0x63e982>|0x002|+0x016:'1166'
	// <0x63e998>|0x018|+0x004:'1167'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <0x63e99c>|0x01c|+0x009:'1175'
	// ******
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

// STATE[STUB]
void scene::dump_scene_statistics( ) const
{
	// LOCALS
	// vector< render_model_instance_impl_ptr > dump_instances
	// fixed_string< 128 > 				model_name
	// ******

	// TYPEDEFS
	// typedef
	// 	scene::dump_scene_statistics::__l2::sort_predicate
	// 	scene::dump_scene_statistics::__l2::sort_predicate;

	// ******

	// CALL SITE INFO
	// <0x63edff> -> u32 < unknown >() const
	// ******

	// FUNCTION BODY[0x63ec50]: 33
	// <0x63ec5b>|0x00b|+0x05d:'1219'
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
	// <16>
	// <17>
	// <0x63ecb8>|0x068|+0x03c:'1238'
	// <0>
	// <1>
	// <2>
	// <0x63ecf4>|0x0a4|+0x013:'1242'
	// <0x63ed07>|0x0b7|-0x0a0:'1243'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <0x63ec67>|0x017|+0x12a:'1250'
	// <0x63ed91>|0x141|+0x0ed:'1250'
	// <0>
	// ******
}

} // namespace render
} // namespace vostok
