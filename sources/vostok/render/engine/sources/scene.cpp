#include "pch.h"
#include "scene.h"
#include <vostok/collision/api.h>
#include <vostok/collision/space_partitioning_tree.h>
#include <vostok/render/core/backend.h>
#include <vostok/render/core/options.h>
#include <vostok/render/facade/common_types.h>
#include "ambient_volume.h"
#include "environment_probe.h"
#include "find_by_id_predicate.h"
#include "grass_world.h"
#include "moved_object_predicate_helper.h"
#include "render_particle_emitter_instance.h"
#include "sky_ambient_occlusion.h"
#include "speedtree_forest.h"
#include "statistics.h"
#include "streaming.h"
#include "system_renderer.h"

static const u32 s_max_vertex_count	= 64*1024;

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

// STATE[STUB]
base_scene_ptr scene::particle_engine::get_scene( particle::world& world )
{
	// FUNCTION BODY[0x63e880]: 2
	// <0>
	// <0x63e880>|0x000|+0x019:'91'
	// ******
	VOSTOK_UNREFERENCED_PARAMETER( world );
	return base_scene_ptr( );
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
	m_use_clouds		( renderer_configuration.m_has_clouds ),
	m_sky_enabled		( renderer_configuration.m_sky_enabled ),
	m_use_occlusion_culling( renderer_configuration.m_use_occlusion_culling ),
	m_portal_system		( NULL )
{
	// FUNCTION BODY[0x63f5e0]: 0
	// ******
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
	m_sky_material		= in_material;

	// FUNCTION BODY[0x63ebd0]: 7
	// <0x63ebd3>|0x003|+0x00d:'135'
	// <0x63ebe0>|0x010|+0x00c:'136'
	// <0>
	// <0x63ebec>|0x01c|+0x038:'138'
	// <0>
	// <0x63ec24>|0x054|+0x00e:'140'
	// <0x63ec32>|0x062|+0x013:'141'
	// ******
}

// STATE[STUB]
void scene::add_clouds( cloud_parameters const& parameters )
{
	// FUNCTION BODY[0x63d900]: 6
	// <0x63d901>|0x001|+0x011:'146'
	// <0>
	// <0x63d912>|0x012|+0x016:'148'
	// <0>
	// <0x63d928>|0x028|+0x01b:'150'
	// <0x63d943>|0x043|+0x013:'151'
	// <0x63d956>|0x056|-0x003:'151'
	// <0x63d953>|0x053|+0x015:'152'
	// ******
}

// STATE[STUB]
void scene::update_clouds( cloud_parameters const& parameters )
{
	// FUNCTION BODY[0x63d970]: 8
	// <0x63d971>|0x001|+0x006:'156'
	// <0>
	// <0x63d977>|0x007|+0x00d:'158'
	// <0>
	// <1>
	// <2>
	// <0x63d984>|0x014|-0x001:'162'
	// <0>
	// <0x63d983>|0x013|+0x007:'164'
	// ******
}

// STATE[STUB]
void scene::remove_clouds( )
{
	// FUNCTION BODY[0x63d1c0]: 3
	// <0x63d1c1>|0x001|+0x019:'168'
	// <0>
	// <0x63d1da>|0x01a|+0x007:'170'
	// ******
}

// STATE[STUB]
void scene::build_lpv_geometry( )
{
	// CALL SITE INFO
	// <0x63cf45> -> void < unknown >( vector< render_model_instance_impl_ptr >& )
	// ******

	// FUNCTION BODY[0x63cf30]: 4
	// <0x63cf30>|0x000|+0x017:'180'
	// <0>
	// <1>
	// <2>
	// ******
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

// STATE[STUB]
void scene::add_tracer( tracer_model_instance_ptr const& instance, float4x4 const& initialize_transform )
{
	// FUNCTION BODY[0x63e920]: 8
	// <0>
	// <1>
	// <0x63e922>|0x002|+0x011:'316'
	// <0x63e933>|0x013|+0x03f:'317'
	// <0x63e972>|0x052|-0x005:'317'
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x63e96d>|0x04d|+0x00c:'322'
	// ******
}

// STATE[STUB]
void scene::update_tracer( tracer_model_instance_ptr const& instance, float4x4 const& new_transform )
{
	// FUNCTION BODY[0x63cf60]: 9
	// <0x63cf61>|0x001|+0x014:'326'
	// <0>
	// <1>
	// <2>
	// <0x63cf75>|0x015|+0x015:'330'
	// <0>
	// <1>
	// <2>
	// <3>
	// ******
}

// STATE[STUB]
void scene::remove_tracer( tracer_model_instance_ptr const& instance )
{
	// FUNCTION BODY[0x63e850]: 7
	// <0x63e852>|0x002|+0x016:'339'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <0x63e868>|0x018|+0x009:'345'
	// ******
}

// STATE[STUB]
void scene::process_streaming(
	float4x4 const&		projection_matrix,
	float3				viewer_position,
	const u32			screen_size_x,
	const u32			screen_size_y
)
{
	// LOCALS
	// streamable_texture_info* 		info_it
	// streamable_texture_info* 		info_end
	// u32 								num_mips
	// u32 								num_wanted_mips
	// float 							distance
	// fs_new::virtual_path_string 		path
	// fs_new::virtual_path_string 		path_add
	// requested_streamable_texture 	texture_request
	// timing::timer 					creation_timer
	// float 							creation_time
	// u32 								index
	// streaming_ready_texture* 		first
	// ******

	// TYPEDEFS
	// typedef
	// 	scene::process_streaming::__l3::remove_requested_texture_predicate
	// 	scene::process_streaming::__l3::remove_requested_texture_predicate;

	// typedef
	// 	scene::process_streaming::__l2::remove_texture_predicate
	// 	scene::process_streaming::__l2::remove_texture_predicate;

	// typedef
	// 	scene::process_streaming::__l16::ready_texture_comparer
	// 	scene::process_streaming::__l16::ready_texture_comparer;

	// ******

	// FUNCTION BODY[0x640160]: 147
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
	// <0x64016d>|0x00d|+0x069:'374'
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
	// <0x6401d6>|0x076|+0x065:'391'
	// <0>
	// <0x64023b>|0x0db|+0x006:'393'
	// <0x640241>|0x0e1|+0x00e:'394'
	// <0>
	// <0x64024f>|0x0ef|+0x015:'396'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <0x640264>|0x104|+0x094:'404'
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x6402f8>|0x198|+0x00a:'409'
	// <0>
	// <1>
	// <2>
	// <0x640302>|0x1a2|+0x025:'413'
	// <0>
	// <1>
	// <0x640327>|0x1c7|+0x009:'416'
	// <0>
	// <0x640330>|0x1d0|+0x004:'418'
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
	// <0x640334>|0x1d4|+0x01c:'430'
	// <0x640350>|0x1f0|+0x017:'431'
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x640367>|0x207|+0x00a:'436'
	// <0>
	// <0x640371>|0x211|+0x00a:'438'
	// <0>
	// <0x64037b>|0x21b|+0x0bc:'440'
	// <0x640437>|0x2d7|+0x00a:'441'
	// <0x640441>|0x2e1|+0x036:'442'
	// <0>
	// <0x640477>|0x317|+0x017:'444'
	// <0x64048e>|0x32e|+0x07d:'445'
	// <0>
	// <0x64050b>|0x3ab|+0x027:'447'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <7>
	// <8>
	// <0x640532>|0x3d2|+0x0d6:'457'
	// <0x640608>|0x4a8|+0x09c:'458'
	// <0>
	// <1>
	// <2>
	// <0x6406a4>|0x544|+0x023:'462'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <0x6406c7>|0x567|+0x01b:'469'
	// <0>
	// <0x6406e2>|0x582|+0x005:'471'
	// <0>
	// <0x6406e7>|0x587|+0x02c:'473'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <0x640713>|0x5b3|+0x06d:'481'
	// <0>
	// <1>
	// <2>
	// <0x640780>|0x620|+0x02e:'485'
	// <0>
	// <0x6407ae>|0x64e|+0x041:'487'
	// <0>
	// <0x6407ef>|0x68f|+0x00f:'489'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <0x6407fe>|0x69e|+0x055:'496'
	// <0>
	// <0x640853>|0x6f3|+0x008:'498'
	// <0x64085b>|0x6fb|+0x011:'499'
	// <0>
	// <0x64086c>|0x70c|+0x012:'501'
	// <0>
	// <0x64087e>|0x71e|+0x01b:'503'
	// <0>
	// <1>
	// ******
}

// STATE[STUB]
void scene::add_trample( trample_desc const& desc )
{
	// FUNCTION BODY[0x63d3d0]: 4
	// <0x63d3d0>|0x000|+0x00b:'510'
	// <0>
	// <1>
	// <0x63d3db>|0x00b|+0x02c:'513'
	// <0x63d407>|0x037|-0x001:'513'
	// <0x63d406>|0x036|+0x008:'514'
	// ******
}

// STATE[STUB]
void scene::on_texture_loaded(
	resources::queries_result&		data,
	res_texture_ptr					texture,
	u32								num_mips,
	float							distance
)
{
	// LOCALS
	// streaming_ready_texture 			ready_texture
	// ******

	// FUNCTION BODY[0x63e070]: 22
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <0x63e07f>|0x00f|+0x040:'523'
	// <0>
	// <0x63e0bf>|0x04f|+0x029:'525'
	// <0>
	// <0x63e0e8>|0x078|+0x015:'527'
	// <0x63e0fd>|0x08d|+0x03f:'528'
	// <0x63e13c>|0x0cc|+0x057:'529'
	// <0>
	// <0x63e193>|0x123|+0x052:'531'
	// <0x63e1e5>|0x175|+0x005:'532'
	// <0>
	// <0x63e1ea>|0x17a|+0x019:'534'
	// <0>
	// <1>
	// <0x63e203>|0x193|+0x029:'537'
	// <0>
	// <1>
	// ******
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

	// CALL SITE INFO
	// <0x63eb70> -> void < unknown >( collision::object*, float4x4 const& )
	// ******

	// FUNCTION BODY[0x63eb00]: 9
	// <0>
	// <1>
	// <2>
	// <0x63eb01>|0x001|+0x058:'630'
	// <0x63eb59>|0x059|+0x019:'631'
	// <0>
	// <0x63eb72>|0x072|+0x031:'633'
	// <0>
	// <1>
	// ******
}

void scene::modify_model( render_model_instance_impl_ptr v )
{
	R_ASSERT					( std::find(m_render_model_instances.begin(), m_render_model_instances.end(), v) != m_render_model_instances.end());
	m_models_tree->move			( &v->m_collision_object, v->transform() );

	// CALL SITE INFO
	// <0x63e8be> -> void < unknown >( collision::object*, float4x4 const& )
	// ******

	// FUNCTION BODY[0x63e8a0]: 8
	// <0>
	// <1>
	// <2>
	// <0x63e8a1>|0x001|+0x01f:'643'
	// <0>
	// <1>
	// <0x63e8c0>|0x020|+0x031:'646'
	// <0>
	// ******
}

void scene::remove_model( render_model_instance_impl_ptr v )
{
	vector< render_model_instance_impl_ptr >::iterator it	= std::find(m_render_model_instances.begin(), m_render_model_instances.end(), v);
	R_ASSERT					( it != m_render_model_instances.end() );
	m_models_tree->erase		( &v->m_collision_object);
	m_render_model_instances.erase	( it );

	// CALL SITE INFO
	// <0x63ea9c> -> void < unknown >( collision::object* )
	// ******

	// FUNCTION BODY[0x63ea60]: 8
	// <0x63ea66>|0x006|+0x01e:'652'
	// <0>
	// <1>
	// <0x63ea84>|0x024|+0x01a:'655'
	// <0x63ea9e>|0x03e|+0x007:'656'
	// <0>
	// <0x63eaa5>|0x045|+0x028:'658'
	// <0>
	// ******
}

// STATE[STUB]
void scene::unmove_all_models( )
{
	// CALL SITE INFO
	// <0x63cefb> -> void < unknown >()
	// ******

	// FUNCTION BODY[0x63cef0]: 0
	// ******
}

// STATE[STUB]
void moved_object_predicate_helper::check_object( collision::object const& obj )
{
	// FUNCTION BODY[0x69180]: 2
	// <0x69180>|0x000|+0x00b:'676'
	// <0x6918b>|0x00b|+0x018:'677'
	// <0x691a3>|0x023|-0x003:'677'
	// <0x691a0>|0x020|+0x00f:'678'
	// ******
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

	BEGIN_TIMER(statistics::ref().visibility_stat_group.culling_time);

	if(options::ref().current.m_enabled_draw_models)
	{
		math::frustum view_frustum (mat_vp);

		selection.clear();

		collision::objects_type query_result(render::g_allocator);
		m_models_tree->cuboid_query( u32(-1), view_frustum, query_result);

		selection.reserve( selection.size() + query_result.size());

		collision::objects_type::const_iterator end = query_result.end();
		for( collision::objects_type::iterator it = query_result.begin(); it != end; ++it)
		{
			render_collision_object<render_model_instance_impl> const* const object =
				static_cast_checked<render_collision_object<render_model_instance_impl> const*>(*it);

			object->owner()->get_surfaces( &mat_vp, &view_pos, selection, moved_only, 0, surface_flags );
		}
	}
	END_TIMER;

	// LOCALS
	// math::frustum 					view_frustum
	// collision::object const* const* 	end
	// vectora< collision::object const* > query_result
	// boost::function< void( collision::object const& ) > callback
	// moved_object_predicate_helper 	helper
	// u8 								lod_id
	// ******

	// CALL SITE INFO
	// <0x63d70c> -> bool < unknown >( u32, math::cuboid const&, boost::function< void( collision::object const& ) >& )
	// <0x63d72d> -> < unknown >
	// <0x63d747> -> bool < unknown >( u32, math::cuboid const&, vectora< collision::object const* >& )
	// <0x63d790> -> math::aabb < unknown >()
	// <0x63d7e3> -> void < unknown >( float4x4 const*, float3 const*, vector< render_surface_instance* >&, bool, u8, u32 )
	// ******

	// FUNCTION BODY[0x63d620]: 57
	// <0x63d63a>|0x01a|+0x013:'684'
	// <0>
	// <1>
	// <0x63d64d>|0x02d|+0x012:'687'
	// <0>
	// <1>
	// <0x63d65f>|0x03f|+0x017:'690'
	// <0>
	// <0x63d676>|0x056|+0x00b:'692'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <0x63d681>|0x061|+0x00b:'698'
	// <0>
	// <0x63d68c>|0x06c|+0x02c:'700'
	// <0>
	// <0x63d6b8>|0x098|+0x016:'702'
	// <0>
	// <1>
	// <2>
	// <0x63d6ce>|0x0ae|+0x00d:'706'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <0x63d6db>|0x0bb|+0x01c:'712'
	// <0>
	// <0x63d6f7>|0x0d7|+0x017:'714'
	// <0x63d70e>|0x0ee|+0x024:'715'
	// <0x63d732>|0x112|+0x002:'716'
	// <0>
	// <0x63d734>|0x114|+0x015:'718'
	// <0>
	// <1>
	// <0x63d749>|0x129|+0x01b:'721'
	// <0>
	// <0x63d764>|0x144|+0x004:'723'
	// <0x63d768>|0x148|+0x010:'724'
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x63d778>|0x158|+0x006:'729'
	// <0>
	// <0x63d77e>|0x15e|+0x01b:'731'
	// <0>
	// <1>
	// <2>
	// <0x63d799>|0x179|+0x023:'735'
	// <0>
	// <0x63d7bc>|0x19c|+0x038:'737'
	// <0>
	// <1>
	// <0x63d7f4>|0x1d4|+0x010:'740'
	// ******
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

// STATE[STUB]
void scene::update_environment_probe( u32 id, environment_probe_properties const& properties )
{
	// FUNCTION BODY[0x640940]: 14
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x640945>|0x005|+0x022:'793'
	// <0x640967>|0x027|+0x004:'794'
	// <0>
	// <0x64096b>|0x02b|+0x045:'796'
	// <0x6409b0>|0x070|+0x016:'796'
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x6409c6>|0x086|-0x01d:'801'
	// <0>
	// <0x6409a9>|0x069|+0x016:'803'
	// <0x6409bf>|0x07f|+0x014:'803'
	// ******
}

// STATE[STUB]
void scene::remove_environment_probe( u32 id )
{
	// FUNCTION BODY[0x63d5a0]: 11
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x63d5a7>|0x007|+0x017:'811'
	// <0x63d5be>|0x01e|+0x004:'812'
	// <0>
	// <1>
	// <0x63d5c2>|0x022|+0x021:'815'
	// <0x63d5e3>|0x043|+0x02d:'816'
	// <0>
	// ******
}

// STATE[STUB]
void scene::update_sky_ambient_occlusion( u32 id, sky_ambient_occlusion_properties const& properties )
{
	// FUNCTION BODY[0x6408b0]: 15
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x6408bc>|0x00c|+0x018:'835'
	// <0>
	// <0x6408d4>|0x024|+0x004:'837'
	// <0>
	// <0x6408d8>|0x028|+0x03b:'839'
	// <0x640913>|0x063|+0x017:'839'
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x64092a>|0x07a|-0x01f:'844'
	// <0>
	// <0x64090b>|0x05b|+0x017:'846'
	// <0x640922>|0x072|+0x012:'846'
	// ******
}

// STATE[STUB]
void scene::remove_sky_ambient_occlusion( u32 id )
{
	// LOCALS
	// sky_ambient_occlusion* 			sky_ao
	// ******

	// FUNCTION BODY[0x63d530]: 11
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x63d536>|0x006|+0x018:'854'
	// <0x63d54e>|0x01e|+0x004:'855'
	// <0>
	// <1>
	// <0x63d552>|0x022|+0x016:'858'
	// <0x63d568>|0x038|+0x02f:'859'
	// <0>
	// ******
}

// STATE[STUB]
void scene::update_ambient_volume( u32 id, ambient_volume_properties const& properties )
{
	// FUNCTION BODY[0x63d870]: 15
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x63d87c>|0x00c|+0x018:'869'
	// <0>
	// <0x63d894>|0x024|+0x004:'871'
	// <0>
	// <0x63d898>|0x028|+0x039:'873'
	// <0x63d8d1>|0x061|+0x017:'873'
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x63d8e8>|0x078|-0x01f:'878'
	// <0>
	// <0x63d8c9>|0x059|+0x017:'880'
	// <0x63d8e0>|0x070|+0x011:'880'
	// ******
}

// STATE[STUB]
void scene::remove_ambient_volume( u32 id )
{
	// FUNCTION BODY[0x63d150]: 11
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x63d156>|0x006|+0x018:'888'
	// <0x63d16e>|0x01e|+0x004:'889'
	// <0>
	// <0x63d172>|0x022|+0x002:'891'
	// <0x63d174>|0x024|+0x016:'892'
	// <0x63d18a>|0x03a|+0x02c:'893'
	// <0>
	// ******
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
	m_volume_fogs.insert			( std::make_pair( id, in_parameters ) );

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

// STATE[STUB]
void scene::set_portal_system( resources::unmanaged_resource_ptr pss )
{
	// FUNCTION BODY[0x63e250]: 1
	// <0x63e250>|0x000|+0x03f:'1180'
	// ******
}

// STATE[STUB]
void scene::select_models_via_sectors(
	float4x4 const&							mat_vp,
	float3 const&							view_point,
	vector< render_surface_instance* >&		selection
)
{
	// FUNCTION BODY[0x63d500]: 1
	// <0x63d501>|0x001|+0x01f:'1191'
	// ******
}

// STATE[STUB]
void scene::draw_portal_system( system_renderer& r, float3 const& view_pos, float4x4 const& vp )
{
	// FUNCTION BODY[0x63fa60]: 2
	// <0x63fa60>|0x000|+0x00a:'1196'
	// <0x63fa6a>|0x00a|+0x00b:'1197'
	// ******
}

// STATE[STUB]
void scene::test_action_portal_system( )
{
	// FUNCTION BODY[0x63cec0]: 2
	// <0x63cec0>|0x000|+0x00a:'1202'
	// <0x63ceca>|0x00a|+0x007:'1203'
	// ******
}

// STATE[STUB]
void scene::set_grass( grass_world* w )
{
	// FUNCTION BODY[0x63ceb0]: 1
	// <0x63ceb0>|0x000|+0x006:'1208'
	// ******
}

// STATE[STUB]
void scene::reset_grass( grass_world* w )
{
	// FUNCTION BODY[0x63cf50]: 2
	// <0>
	// <0x63cf50>|0x000|+0x00a:'1214'
	// ******
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
