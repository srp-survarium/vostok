#include "pch.h"
#include <vostok/collision/api.h>
#include <vostok/collision/space_partitioning_tree.h>
#include <vostok/console_command.h>
#include <vostok/math_randoms_generator.h>
#include <vostok/render/core/backend.h>
#include <vostok/render/core/options.h>
#include <vostok/render/engine/vertex_colored.h>
#include "grass_patch.h"
#include "grass_world.h"
#include "renderer_context.h"
#include "scene_view.h"
#include "statistics.h"
#include "system_renderer.h"

namespace vostok {
namespace render {

static bool s_draw_grass_debug_value				=	false;
static vostok::console_commands::cc_bool s_draw_grass_debug("draw_grass_debug", s_draw_grass_debug_value, false, vostok::console_commands::command_type_engine_internal);

static bool s_use_grass_patches_sorting_value	=	true;
static vostok::console_commands::cc_bool s_use_grass_patches_sorting("use_grass_patches_sorting", s_use_grass_patches_sorting_value, false, vostok::console_commands::command_type_engine_internal);

static bool s_use_grass_instances_sorting_value	=	false;
static vostok::console_commands::cc_bool s_use_grass_instances_sorting("use_grass_instances_sorting", s_use_grass_instances_sorting_value, false, vostok::console_commands::command_type_engine_internal);

static u32 point_random_x;
static u32 point_random_z;
static u32 model_index_random;
static u32 model_orientation_random;
static u32 model_density_random;
static u32 model_scale_random;

void setup_seed_clk( );

typedef vector<grass_patch*>	grass_patches_type;
typedef vector<grass_instance*>	grass_instances_type;

grass_world::grass_world( ) :
	m_patches_tree						( 0 ),
	m_ambient_color						( 0 ),
	m_c_environment_skylight_upper_color	( 0 ),
	m_c_environment_skylight_lower_color	( 0 ),
	m_c_environment_skylight_parameters	( 0 ),
	m_c_sun_direction					( 0 ),
	m_c_sun_color						( 0 ),
	m_patch_parameters					( 0 ),
	m_trample_parameters					( 0 ),
	m_shadow_cascade_index_parameter		( 0 ),
	m_wind_info_parameters					( 0 )
{
	m_patches_tree									=	&*collision::new_space_partitioning_tree(g_allocator, 1.f, 1024);
	m_ambient_color									=	backend::ref( ).register_constant_host( "ambient_color", rc_float );
	setup_seed_clk( );
	m_c_environment_skylight_upper_color			=	backend::ref( ).register_constant_host( "environment_skylight_upper_color", rc_float );
	m_c_environment_skylight_lower_color			=	backend::ref( ).register_constant_host( "environment_skylight_lower_color", rc_float );
	m_c_environment_skylight_parameters			=	backend::ref( ).register_constant_host( "environment_skylight_parameters", rc_float );
	m_c_sun_direction								=	backend::ref( ).register_constant_host( "sun_direction", rc_float );
	m_c_sun_color									=	backend::ref( ).register_constant_host( "sun_color", rc_float );
	m_patch_parameters								=	backend::ref( ).register_constant_host( "patch_parameters", rc_float );
	m_trample_parameters							=	backend::ref( ).register_constant_host( "trample_parameters", rc_float );
	m_shadow_cascade_index_parameter				=	backend::ref( ).register_constant_host( "shadow_cascade_index", rc_int );
	m_wind_info_parameters							=	backend::ref( ).register_constant_host( "wind_info_parameters", rc_float );
}

void grass_world::set_wind_parameters( float2 const& dir, float const strength )
{
	backend::ref( ).set_vs_constant( m_wind_info_parameters, float3( dir.x, dir.y, strength ) );
}

void grass_world::set_patch_parameters( grass_patch* patch )
{
	backend::ref( ).set_vs_constant( m_patch_parameters, float4( patch->m_origin, 16.f ) );
}

void grass_world::set_trample_parameters( trample_desc& desc )
{
	backend::ref( ).set_ps_constant( m_trample_parameters, desc.multiplier );
}

void grass_world::set_shadow_parameters( u32 const )
{
	backend::ref( ).set_vs_constant( m_shadow_cascade_index_parameter, 0 );
}

grass_world::~grass_world( )
{
	// FUNCTION BODY[0x6370f0]
	grass_patches_type::const_iterator it_p			=	m_patches.begin();
	grass_patches_type::const_iterator end_p		=	m_patches.end();

	for (; it_p != end_p; ++it_p)
	{
		grass_patch* patch							=	*it_p;
		DELETE										(patch);
	}

	grass_templates_type::iterator	it_t			=	m_templates.begin();
	grass_templates_type::iterator	end_t			=	m_templates.end();

	for (; it_t != end_t; ++it_t)
	{
		grass_template* templ						=	(*it_t);

		DELETE										(templ);
	}

	collision::delete_space_partitioning_tree		(m_patches_tree);
}

u32 grass_world::add_template( grass_render_model_ptr const& )
{
	// claude@NOTE: legacy body diverged - legacy add_template takes an explicit in_id and returns void; canonical generates and returns the id; matcher-phase work.
	// STATE[STUB]
	// FUNCTION BODY[0x636920]
	return 0;
}

void grass_world::add_trample( trample_desc const& desc )
{
	m_trample_array.push_back	( desc );
}

u32 grass_world::add_instance( u32 const, math::color const&, float4x4 const&, u8 const, float const )
{
	// claude@NOTE: legacy body diverged - legacy add_instance takes explicit in_id, lacks layer/wind_scale, returns void; matcher-phase work.
	// STATE[STUB]
	// FUNCTION BODY[0x636090]
	return 0;
}

void grass_world::remove_instance( u32 const in_id )
{
	// FUNCTION BODY[0x635d90]
	grass_templates_type::const_iterator	it		=	m_templates.begin();
	grass_templates_type::const_iterator	end		=	m_templates.end();

	for (; it != end; ++it)
	{
		grass_template* templ						=	*it;

		grass_instances_type::iterator it_instance	=	templ->m_instances.begin();
		grass_instances_type::iterator end_instance	=	templ->m_instances.end();

		for (; it_instance != end_instance; ++it_instance)
		{
			grass_instance* instance				=	*it_instance;

			if (instance->m_index == in_id)
			{
				DELETE								(instance);
				templ->m_instances.erase			(it_instance);
				return;
			}
		}
	}
}

grass_template* grass_world::id_to_template( u32 const id ) const
{
	// FUNCTION BODY[0x635a70]
	grass_template* result							=	NULL;
	grass_templates_type::const_iterator	it		=	m_templates.begin();
	grass_templates_type::const_iterator	end		=	m_templates.end();

	for (; it != end; ++it)
	{
		result										=	(*it);

		if (result->m_index == id)
			break;
	}

	return result;
}

grass_template* grass_world::find_template( grass_render_model_ptr const& model ) const
{
	// STATE[STUB]
	// claude@NOTE: no legacy ancestor (legacy looked templates up by id only); matcher-phase.
	// FUNCTION BODY[0x635a20]
	VOSTOK_UNREFERENCED_PARAMETER	( model );
	return 0;
}

grass_patch* grass_world::find_patch( float3 const& point )
{
	// FUNCTION BODY[0x6359c0]
	grass_patches_type::const_iterator it			=	m_patches.begin();
	grass_patches_type::const_iterator end			=	m_patches.end();

	for (; it != end; ++it)
	{
		grass_patch* patch							=	*it;
		if (
			patch->get_aabb( ).min.x <= point.x &&
			patch->get_aabb( ).min.z <= point.z &&
			patch->get_aabb( ).max.x >= point.x &&
			patch->get_aabb( ).max.z >= point.z
		)
		{
			return patch;
		}
	}
	return NULL;
}

void grass_world::remove_patches( )
{
	grass_patch** it_p					=	m_patches.begin( );
	grass_patch* const* end_p			=	m_patches.end( );

	for ( ; it_p != end_p; ++it_p )
	{
		grass_patch* patch				=	*it_p;
		DELETE								( patch );
	}

	m_patches.clear( );
	m_visible_patches.clear( );
}

void grass_world::populate( float const patch_size_ground )
{
	remove_patches( );

	grass_template** it					=	m_templates.begin( );
	grass_template** end				=	m_templates.end( );

	for ( ; it != end; ++it )
	{
		grass_template* templ			=	*it;

		grass_instance** it_instance	=	templ->m_instances.begin( );
		grass_instance** end_instance	=	templ->m_instances.end( );

		for ( ; it_instance != end_instance; ++it_instance )
		{
			grass_instance* instance	=	*it_instance;
			float3 origin				=	instance->m_transform.c.xyz( );
			float3 origin_aligned		=	float3(
				math::floor( origin.x / patch_size_ground ) * patch_size_ground + patch_size_ground * .5f,
				0.f,
				math::floor( origin.z / patch_size_ground ) * patch_size_ground + patch_size_ground * .5f
			);

			grass_patch* new_patch		=	find_patch( origin_aligned );
			if ( !new_patch )
			{
				new_patch					=	NEW( grass_patch )(
					m_patches_tree,
					templ,
					origin_aligned,
					patch_size_ground
				);
				m_patches.push_back		( new_patch );
				new_patch->m_instances.push_back( instance );
			}
			else
			{
				new_patch->m_instances.push_back( instance );
			}
		}
	}

	grass_patch** it_patch				=	m_patches.begin( );
	grass_patch* const* end_patch		=	m_patches.end( );
	for ( ; it_patch != end_patch; ++it_patch )
	{
		grass_patch* patch				=	*it_patch;
		patch->init_collision( );
		patch->merge_instances( );
	}
}

void grass_world::merge_patches( )
{
	// STATE[STUB]
	// FUNCTION BODY[0x6370c0]
}

// claude@NOTE: residual cause - our LTCG partial-inlines the s_draw_grass_debug_value guard
// into renderer::draw_debug (the cmp/je moves to the call site and the standalone body
// loses it, -0xd bytes and a different prologue/frame layout); the target keeps the whole
// guard here. The rest is register-allocation drift inside the four inlined math::floor
// expansions of math::color.
void grass_world::render_debug( renderer_context* context )
{
	if ( !s_draw_grass_debug_value )
		return;

	VOSTOK_UNREFERENCED_PARAMETER		( context );

	grass_patch* const* it				= m_visible_patches.begin( );
	grass_patch* const* end				= m_visible_patches.end( );
	float const fnum_patches			= float( m_visible_patches.size( ) );
	float fpatch_index					= 0.f;

	math::random32 r;

	for ( ; it != end; ++it )
	{
		grass_patch* patch				= *it;

		system_renderer::ref( ).draw_aabb	(
			patch->get_aabb( ),
			math::color( 1.f - fpatch_index / fnum_patches, 1.f - fpatch_index / fnum_patches, 1.f - fpatch_index / fnum_patches, 1.f )
		);

		grass_instance** it_instance	= patch->m_instances.begin( );
		grass_instance** end_instance	= patch->m_instances.end( );
		float const fnum_instances		= float( patch->m_instances.size( ) );
		float finstance_index			= 0.f;

		for ( ; it_instance != end_instance; ++it_instance )
		{
			grass_instance* instance	= *it_instance;
			float3 origin				= instance->m_transform.c.xyz( );

			math::color const c			= math::color(
				1.f - finstance_index / fnum_instances,
				1.f - finstance_index / fnum_instances,
				1.f - finstance_index / fnum_instances,
				1.f );

			vertex_colored vertices[]	= {
				vertex_colored( origin, c ),
				vertex_colored( origin + float3( r.random_f( 2.f ) - 1.f, 1.2f, r.random_f( 2.f ) - 1.f ), c )
			};
			u16 indices[]				= { 0, 1 };
			system_renderer::ref( ).draw_lines( vertices, vertices + array_size( vertices ), indices, indices + array_size( indices ), false );

			finstance_index				+= 1.f;
		}

		fpatch_index					+= 1.f;
	}
}

bool sort_grass_patch_predicate::operator()( grass_patch const* left, grass_patch const* right ) const
{
	return math::squared_length( left->m_origin - m_view_pos ) <
		math::squared_length( right->m_origin - m_view_pos );
}

void grass_world::process_sorting( float3 const& viewer_position, bool sort_instances )
{
	if ( s_use_grass_patches_sorting_value )
		std::sort(
			m_visible_patches.begin( ),
			m_visible_patches.end( ),
			sort_grass_patch_predicate( viewer_position )
		);

	if ( !s_use_grass_instances_sorting_value || !sort_instances )
		return;

	grass_patch** it = m_visible_patches.begin( );
	grass_patch** end = m_visible_patches.end( );
	u32 num_sorted_patches = 0;
	for ( ; it != end; ++it ) {
		if ( num_sorted_patches > 5 )
			break;

		(*it)->sort_instances( viewer_position );
		++num_sorted_patches;
	}
}

void grass_world::process_culling( renderer_context* context, float const first_lod_distance )
{
	// FUNCTION BODY[0x6361b0]
	m_visible_patches.clear							();

	statistics::ref().grass_stat_group.num_total_patches.value	=	m_patches.size();

	collision::objects_type objects					(	g_allocator );
	objects.reserve									(m_patches.size());

	math::frustum view_frustum						(context->get_vp());

	m_patches_tree->cuboid_query					(u32(-1), view_frustum, objects );

	float const first_lod_distance_squared			=	first_lod_distance * first_lod_distance;

	for (collision::objects_type::const_iterator it = objects.begin(), end = objects.end(); it != end; ++it)
	{
		grass_patch* patch							=	static_cast<grass_patch*>((*it)->user_data());

		float const to_aabb_center_squared			=	math::squared_length(context->get_view_pos() - patch->get_aabb().center());

		if (first_lod_distance_squared < to_aabb_center_squared)
			continue;

		patch->m_current_lod_index						=	0;
		float const distance							=	math::sqrt( to_aabb_center_squared );
		if ( distance > options::ref( ).current.m_grass_lod1_distance )
			patch->m_current_lod_index					=	1;
		else if ( distance > options::ref( ).current.m_grass_lod2_distance )
			patch->m_current_lod_index					=	2;

		patch->m_current_lod_index						=	patch->get_valid_lod_index(
			patch->m_current_lod_index
		);
		m_visible_patches.push_back					(patch);
	}

	process_sorting(
		context->get_view_pos( ),
		!( context->scene_view( )->get_render_frame_index( ) & 31 )
	);
}

void grass_world::accumulate_trample( renderer* in_renderer, renderer_context* in_context )
{
	grass_patch* const* it_patch			= m_patches.begin( );
	grass_patch* const* end_patch		= m_patches.end( );

	for ( ; it_patch != end_patch; ++it_patch )
	{
		if ( options::ref( ).current.m_use_hiz_occlusion_culling && (*it_patch)->is_occluded( ) )
			continue;

		trample_desc* it_trample			= m_trample_array.begin( );
		trample_desc* end_trample		= m_trample_array.end( );

		for ( ; it_trample != end_trample; ++it_trample )
			(*it_patch)->try_accumulate_trample( *it_trample, this, in_renderer, in_context );
	}

	m_trample_array.clear( );
}

void grass_world::remove_trample( )
{
	grass_patch* const* it_patch			= m_patches.begin( );
	grass_patch* const* end_patch		= m_patches.end( );

	for ( ; it_patch != end_patch; ++it_patch )
		(*it_patch)->remove_trample( );
}

void grass_world::render(
	renderer_context*		context,
	float3 const&			viewer_position,
	enum_render_stage_type	stage_type,
	u32 const				tech_index,
	float const				draw_distance,
	bool,
	res_effect*				debug_effect,
	bool					shadow_pass,
	u32 const				cascade_index
)
{
	// FUNCTION BODY[0x638150]
	if ( !options::ref( ).current.m_draw_grass )
		return;

	if (
		!m_templates.empty( ) &&
		!m_templates.front( )->m_instances.empty( ) &&
		m_patches.empty( )
	)
		m_need_populate = true;

	if ( m_need_populate ) {
		populate( 16.f );
		m_need_populate = false;
	}

	grass_patch** it_patch = m_visible_patches.begin( );
	grass_patch* const* end_patch = m_visible_patches.end( );
	for ( ; it_patch != end_patch; ++it_patch ) {
		grass_patch* patch = *it_patch;
		if ( options::ref( ).current.m_use_hiz_occlusion_culling && patch->is_occluded( ) )
			continue;

		if ( shadow_pass ) {
			math::frustum shadow_frustum( context->get_culling_vp( ) );
			if ( shadow_frustum.test_inexact( patch->get_aabb( ) ) == math::intersection_outside )
				continue;
		}

		patch->render(
			this,
			context,
			viewer_position,
			stage_type,
			tech_index,
			draw_distance,
			debug_effect,
			cascade_index
		);
	}
}

void grass_world::add_grass_layer( grass_layer_desc*, grass_layer_data*, bool, bool )
{
	// STATE[STUB]
	// FUNCTION BODY[0x637b70]
}

void grass_world::update_grass_layer(
	grass_layer_desc*,
	grass_layer_data*,
	bool,
	bool,
	bool
)
{
	// STATE[STUB]
	// FUNCTION BODY[0x637940]
}

void grass_world::clear( )
{
	// STATE[STUB]
	// FUNCTION BODY[0x636fb0]
}

void grass_world::remove_grass_layer( u8, bool )
{
	// STATE[STUB]
	// FUNCTION BODY[0x635e40]
}

void grass_world::remove_instances( vector<u32> const& )
{
	// STATE[STUB]
	// FUNCTION BODY[0x635e10]
}

void setup_seed_clk( )
{
	u32 const seed							= GetTickCount( );
	point_random_x							= seed;
	point_random_z							= seed;
	model_index_random						= seed;
	model_orientation_random				= seed;
	model_density_random					= seed;
	model_scale_random						= seed;
}

u8 select_model_template( float*, float, u8 )
{
	// STATE[STUB]
	// FUNCTION BODY[0x635b50]
	return 0;
}

float select_model_orientation( )
{
	// STATE[STUB]
	// FUNCTION BODY[0x635b10]
	return 0.0f;
}

float select_model_scale( float, float )
{
	// STATE[STUB]
	// FUNCTION BODY[0x635ac0]
	return 0.0f;
}

void grass_world::grass_layer_resources_ready_from_cook(
	resources::queries_result&,
	grass_layer_desc*,
	grass_layer_data*,
	bool
)
{
	// STATE[STUB]
	// FUNCTION BODY[0x636da0]
}

void grass_world::grass_layer_resources_ready(
	resources::queries_result&,
	grass_layer_desc*,
	grass_layer_data*,
	bool
)
{
	// STATE[STUB]
	// FUNCTION BODY[0x636990]
}

void grass_world::remove_layer_instances( u8, float2 const&, float2 const& )
{
	// STATE[STUB]
	// FUNCTION BODY[0x635f50]
}

} // namespace render
} // namespace vostok
