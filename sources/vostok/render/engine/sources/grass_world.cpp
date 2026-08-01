#include "pch.h"
// claude@NOTE: legacy-harvest disposition: the trample/layer/merge/sort/shader-parameter stubs below have no legacy ancestor (absent from the legacy grass_world twin; subsystems are new-in-target); the four DIVERGED fns carry their own notes - matcher-phase work.
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
#include "statistics.h"
#include "system_renderer.h"

namespace vostok {
namespace render {

static bool s_draw_grass_debug_value				=	false;
static vostok::console_commands::cc_bool s_draw_grass_debug("draw_grass_debug", s_draw_grass_debug_value, false, vostok::console_commands::command_type_engine_internal);

static u32 point_random_x;
static u32 point_random_z;
static u32 model_index_random;
static u32 model_orientation_random;
static u32 model_density_random;
static u32 model_scale_random;

void setup_seed_clk( );

// claude@NOTE: the legacy grass_world.h declared these typedefs in-class; the canonical
// header keeps raw vector members, so they live file-local here.
typedef vector<grass_patch*>	grass_patches_type;
typedef vector<grass_template*>	grass_templates_type;
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
	// FUNCTION BODY[0x6364e0]
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

void grass_world::set_wind_parameters( float2 const&, float )
{
	// STATE[STUB]
	// FUNCTION BODY[0x635d10]
}

void grass_world::set_patch_parameters( grass_patch* )
{
	// STATE[STUB]
	// FUNCTION BODY[0x635c70]
}

void grass_world::set_trample_parameters( trample_desc& desc )
{
	// FUNCTION BODY[0x635c40]
	backend::ref( ).set_ps_constant( m_trample_parameters, desc.multiplier );
}

void grass_world::set_shadow_parameters( u32 )
{
	// STATE[STUB]
	// FUNCTION BODY[0x635be0]
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

u32 grass_world::add_instance( u32, math::color const&, float4x4 const&, u8, float )
{
	// claude@NOTE: legacy body diverged - legacy add_instance takes explicit in_id, lacks layer/wind_scale, returns void; matcher-phase work.
	// STATE[STUB]
	// FUNCTION BODY[0x636090]
	return 0;
}

void grass_world::remove_instance( u32 in_id )
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

grass_template* grass_world::id_to_template( u32 id ) const
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
	// claude@NOTE: legacy overload also filtered by grass_template*; the canonical
	// point-only overload drops that filter.
	grass_patches_type::const_iterator it			=	m_patches.begin();
	grass_patches_type::const_iterator end			=	m_patches.end();

	for (; it != end; ++it)
	{
		grass_patch* patch							=	*it;
		if (patch->get_aabb().min <= point &&
			patch->get_aabb().max >= point)
		{
			return patch;
		}
	}
	return NULL;
}

void grass_world::remove_patches( )
{
	// STATE[STUB]
	// FUNCTION BODY[0x636ee0]
}

void grass_world::populate( float )
{
	// claude@NOTE: legacy body diverged - legacy populate has a retired patch_size_height param and instancing tail (create_patch_render_buffers); matcher-phase work.
	// STATE[STUB]
	// FUNCTION BODY[0x637b90]
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

bool sort_grass_patch_predicate::operator()( grass_patch const*, grass_patch const* ) const
{
	// STATE[STUB]
	// FUNCTION BODY[0x70670]
	return false;
}

void grass_world::process_sorting( float3 const&, bool )
{
	// STATE[STUB]
	// FUNCTION BODY[0x636130]
}

void grass_world::process_culling( renderer_context* context, float first_lod_distance )
{
	// FUNCTION BODY[0x6361b0]
	m_visible_patches.clear							();
	m_visible_patches.reserve						(256);

	statistics::ref().grass_stat_group.num_total_patches.value	=	m_patches.size();

	collision::objects_type objects					=	g_allocator;
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

		m_visible_patches.push_back					(patch);
	}
}

void grass_world::accumulate_trample( renderer* in_renderer, renderer_context* in_context )
{
	// FUNCTION BODY[0x6371c0]
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
	// FUNCTION BODY[0x635bb0]
	grass_patch* const* it_patch			= m_patches.begin( );
	grass_patch* const* end_patch		= m_patches.end( );

	for ( ; it_patch != end_patch; ++it_patch )
		(*it_patch)->remove_trample( );
}

void grass_world::render(
	renderer_context*,
	float3 const&,
	enum_render_stage_type,
	u32,
	float,
	bool,
	res_effect*,
	bool,
	u32
)
{
	// claude@NOTE: legacy body diverged - legacy render draws via retired per-patch instance decl/vb members and lacks debug_effect/shadow_pass/cascade_index; matcher-phase work.
	// STATE[STUB]
	// FUNCTION BODY[0x638150]
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
	// FUNCTION BODY[0x635990]
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
