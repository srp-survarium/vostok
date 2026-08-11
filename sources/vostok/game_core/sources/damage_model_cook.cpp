////////////////////////////////////////////////////////////////////////////
//	Created 	: 06.12.2025
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include <vostok/game_core/damage_model_cook.h>

#include <vostok/game_core/damage_model.h>
#include <vostok/game_core/body_part_parameters.h>
#include <vostok/game_core/hit_type_parameters.h>
#include <vostok/game_core/affects_threshold.h>

namespace survarium {

fixed_vector< console_commands::command_token, 12 >	damage_model_cook::m_hit_types;
console_commands::cc_token*							damage_model_cook::m_hit_types_commands;
fixed_vector< fixed_string<24>, 12 >				damage_model_cook::m_hit_types_strings;
u32 g_current_hit_type;	// sushi@TODO: There is `get_current_hit_type` function, which possibly relies on this. I didn't find any references to it though, so skipping for now.

damage_model_cook::damage_model_cook( ) :
	resources::translate_query_cook( resources::damage_model_class, reuse_false, use_current_thread_id )
{
	resources::register_cook( this );
}

void damage_model_cook::translate_query( resources::query_result_for_cook& parent )
{
	resources::query_resource(
		parent.get_requested_path( ),
		resources::binary_config_class_impl,
		boost::bind( &damage_model_cook::on_hit_params_received, this, _1 ),
		g_allocator,
		NULL,
		&parent
	);
}

void damage_model_cook::delete_resource( resources::resource_base* resource )
{
	damage_model* model_res = static_cast_checked< damage_model* >( resource );

	while ( body_part_parameters* part = model_res->pop_body_part( ) )
	{
		while ( hit_type_parameters* type = part->pop_hit_type( ) )
		{
			std::pair< body_part_parameters*, float > const* it_bdb_begin	= type->get_bdb_coefficients( );
			std::pair< body_part_parameters*, float > const* it_bdb_end		= it_bdb_begin + type->get_bdb_coeffs_count( );

			for ( std::pair<body_part_parameters*, float> const* it = it_bdb_begin ; it != it_bdb_end; ++it )
				{} // it->first->~body_part_parameters( );										// sushi@NOTE: Uncommenting this line results in worse match.

			type->~hit_type_parameters( );
		}

		while ( affects_threshold* threshold = part->pop_threshold( ) )
		{
			hit_affects_type_enum const* it_affects		= threshold->get_affects( );
			hit_affects_type_enum const* it_affects_end	= it_affects + threshold->get_affects_count( );
			for ( hit_affects_type_enum const* it = it_affects ; it != it_affects_end; ++it )
				it->~hit_affects_type_enum( );

			threshold->~affects_threshold( );
		}

		part->~body_part_parameters( );
	}

	model_res->~damage_model( );
	VOSTOK_FREE_IMPL( g_allocator, resource );	// claude@MATCH: target keeps free_helper out-of-line (free_helper_impl); base inlines free_helper.
}

static u32 calculate_model_size( configs::binary_config_value const& model_value )
{
	const u32 body_parts_count	= model_value.size( );
	u32 result					= sizeof( body_part_parameters ) * body_parts_count;

	configs::binary_config_value const* it_model		= model_value.begin( );
	configs::binary_config_value const* it_model_end	= model_value.end( );

	for ( ; it_model != it_model_end ; ++it_model )
	{
		configs::binary_config_value const& part_value	= *it_model;
		configs::binary_config_value const& types_value = part_value["hit_types"];

		result += sizeof( hit_type_parameters ) * types_value.size( );

		configs::binary_config_value const* it_type		= types_value.begin( );
		configs::binary_config_value const* it_type_end	= types_value.end( );

		for ( ; it_type != it_type_end ; ++it_type )
		{
			configs::binary_config_value const& type_value = *it_type;
			configs::binary_config_value const& bdbs_value = type_value["bdb_coeff"];

			result += sizeof( bdb_coeff ) * bdbs_value.size( ); // sushi@NOTE: Might be incorrect.
		}

		configs::binary_config_value const& thresholds_value = part_value["thresholds"];

		result += sizeof( affects_threshold ) * thresholds_value.size( );

		configs::binary_config_value const* it_threshold		= thresholds_value.begin( );
		configs::binary_config_value const* it_threshold_end	= thresholds_value.end( );

		for ( ; it_threshold != it_threshold_end ; ++it_threshold )
		{
			configs::binary_config_value const& threshold_value = *it_threshold;
			configs::binary_config_value const& affects_value	= threshold_value["affects"];

			result += sizeof( hit_affects_type_enum ) * affects_value.size( );
		}
	}

	return result;
}

static hit_type_parameters* create_hit_type_parameters(
	damage_model* const						model,
	memory::stack_allocator*				allocator,
	configs::binary_config_value const&		type_value
){
	const u32 bdb_coeffs_count = type_value["bdb_coeff"].size( );

	hit_type_parameters* new_hit_type = VOSTOK_NEW_IMPL( allocator, hit_type_parameters )(
		type_value.key( ),
		(float)type_value["absorption"],
		(float)type_value["armor"],
		(float)type_value["reduce"],
		bdb_coeffs_count
	);

	configs::binary_config_value const* it_bdb		= type_value["bdb_coeff"].begin( );
	configs::binary_config_value const* it_bdb_end	= type_value["bdb_coeff"].end( );

	for ( ; it_bdb != it_bdb_end ; ++it_bdb )
	{
		body_part_parameters* const body_part = model->get_body_part( it_bdb->key( ) );
		ASSERT( UNKNOWN_EXPRESSION_T( body_part ) );
		VOSTOK_NEW_IMPL( allocator, bdb_coeff )(	// `hit_type_parameters` implicitly expects bdb_coefficients to be after its allocation.
			body_part,
			(float)*it_bdb
		);
	}

	return new_hit_type;
}

static affects_threshold* create_threshold(
	memory::stack_allocator*				allocator,
	configs::binary_config_value const&		threshold_value,
	damage_model* const						model
)
{
	configs::binary_config_value	affects_table = threshold_value["affects"];
	const u32 						affects_count = affects_table.size( );
	pcstr 							bodypart_name = threshold_value["target_bodypart"];
	body_part_parameters* 			bodypart	  = model->get_body_part( bodypart_name );
	ASSERT( UNKNOWN_EXPRESSION_T( bodypart ) );
	affects_threshold* 				new_threshold = VOSTOK_NEW_IMPL( allocator, affects_threshold )(
		threshold_value["value"],
		affects_count,
		bodypart
	);

	configs::binary_config_value const* it_affect	  = affects_table.begin( );
	configs::binary_config_value const* it_affect_end = affects_table.end( );

	for ( ; it_affect != it_affect_end ; ++it_affect )
	{
		const u32				affect_type = *it_affect;
		hit_affects_type_enum	type		= (hit_affects_type_enum)affect_type; // sushi@TODO: Maybe there is a safe cast removed on MasterGold.
		VOSTOK_NEW_IMPL( allocator, hit_affects_type_enum )( type );
	}
	return new_threshold;
}

static void fill_body_part_parameters(
	body_part_parameters*					body_part,
	damage_model* const						model,
	memory::stack_allocator*				allocator,
	configs::binary_config_value const&		part_value
)
{
	configs::binary_config_value const& types_value = part_value["hit_types"];

	configs::binary_config_value const* it_type		= types_value.begin( );
	configs::binary_config_value const* it_type_end	= types_value.end( );

	for ( ; it_type != it_type_end ; ++it_type )
	{
		configs::binary_config_value const& type_value		= *it_type;

		hit_type_parameters*				hit_type_params = create_hit_type_parameters( model, allocator, type_value );
		body_part->add_hit_type( hit_type_params );
	}

	configs::binary_config_value const& thresholds_value = part_value["thresholds"];

	configs::binary_config_value const* it_threshold	 = thresholds_value.begin( );
	configs::binary_config_value const* it_threshold_end = thresholds_value.end( );

	for ( ; it_threshold != it_threshold_end ; ++it_threshold )
	{
		configs::binary_config_value const& threshold_value = *it_threshold;

		affects_threshold* 					new_threshold   = create_threshold( allocator, threshold_value, model );
		body_part->add_threshold( new_threshold );
	}
}

static body_part_parameters* create_body_part_parameters(
	memory::stack_allocator*				allocator,
	configs::binary_config_value const&		part_value,
	damage_model&							model,
	const u8								damage_group
)
{
	body_part_parameters* new_part = VOSTOK_NEW_IMPL( allocator, body_part_parameters )(
		part_value["name"],
		part_value["health"],
		part_value["regeneration_speed"],
		part_value["regeneration_timeout"],
		part_value["can_be_assigned"],
		model,
		damage_group
	);
	return new_part;
}

static void fill_damage_model(
	damage_model* const						model,
	memory::stack_allocator*				allocator,
	configs::binary_config_value const&		model_value,
	configs::binary_config_value const&		damage_groups
)
{
	configs::binary_config_value const* it_model		= model_value.begin( );
	configs::binary_config_value const* it_model_end	= model_value.end( );

	for ( ; it_model != it_model_end ; ++it_model )
	{
		configs::binary_config_value const& part_value	 = *it_model;
		u8 									damage_group = u8(-1);

		typedef configs::binary_config_value const* config_iterator;
		ASSERT( UNKNOWN_EXPRESSION );
		for ( u8 i = 0 ; i != damage_groups.size( ) ; ++i )
			for ( config_iterator it_part = damage_groups[i].begin( ), it_part_end = damage_groups[i].end( ) ; it_part != it_part_end ; ++it_part )
				if ( strings::equal( part_value["name"], *it_part ) )
				{
					damage_group = i;
					break;
				}

 		body_part_parameters* new_body_part = create_body_part_parameters( allocator, part_value, *model, damage_group );
		model->add_body_part( new_body_part );
	}

	it_model = model_value.begin( );
	for ( ; it_model != it_model_end ; ++it_model )
	{
		configs::binary_config_value const& part_value	= *it_model;
		body_part_parameters* 				body_part	= model->get_body_part( part_value["name"] );
		ASSERT( UNKNOWN_EXPRESSION_T( body_part ) );
		fill_body_part_parameters( body_part, model, allocator, part_value );
	}
}

void damage_model_cook::on_hit_params_received( resources::queries_result& data )
{
	resources::query_result_for_cook* const parent = data.get_parent_query( );
	if ( !data.is_successful( ) )
	{
		ASSERT( UNKNOWN_EXPRESSION ); // sushi@TODO: There are more similar cases. Possibly there are example to match this better.
		parent->finish_query( result_error );
		return;
	}

	static bool	hit_types_initialized = false;

	configs::binary_config_ptr config = static_cast_resource_ptr< configs::binary_config_ptr >( data[0].get_unmanaged_resource( ) );
	configs::binary_config_value const& config_value	= config->get_root( );
	configs::binary_config_value const& params_value	= config_value["hit_params"];
	configs::binary_config_value const& damage_groups	= config_value["damage_groups"];

	if ( !hit_types_initialized )
	{
		configs::binary_config_value const& hit_types = config_value["hit_types_available"];

		configs::binary_config_value const* it_hit		= hit_types.begin( );
		configs::binary_config_value const* it_hit_end	= hit_types.end( );

		for ( u32 i = 0 ; it_hit != it_hit_end ; ++it_hit, ++i )
		{
			m_hit_types_strings.push_back( fixed_string<24>( (pcstr)*it_hit ) );
			console_commands::command_token new_command = { i, m_hit_types_strings.back( ).c_str( ) };
			m_hit_types.push_back( new_command );
		}
		m_hit_types_commands = VOSTOK_NEW_IMPL( g_allocator, console_commands::cc_token )(	// claude@MATCH: target stores the new cc_token into static m_hit_types_commands
			"hit_type",
			g_current_hit_type,
			m_hit_types.begin( ),
			m_hit_types.size( ),
			false,
			console_commands::command_type_user_specific,
			console_commands::execution_filter_general
		);
		hit_types_initialized = true;
	}

	const u32 model_buffer_size = calculate_model_size( params_value );
	pcstr description			= "damage_model_memory";
	void* model_buffer			= VOSTOK_MALLOC_IMPL( g_allocator, model_buffer_size, description );

	memory::stack_allocator stack_allocator;
	stack_allocator.initialize( model_buffer, model_buffer_size, description );

	affects_applying_type_enum affects_applying_type;
	variant< 32 >* ud			= parent->user_data( );
	ASSERT( UNKNOWN_EXPRESSION_T( ud && ud->try_get( affects_applying_type ) ) );
	ud->try_get( affects_applying_type );

	damage_model* const new_model = VOSTOK_NEW_IMPL( stack_allocator, damage_model )( affects_applying_type );
	fill_damage_model( new_model, &stack_allocator, params_value, damage_groups );
	ASSERT( UNKNOWN_EXPRESSION );

	parent->set_unmanaged_resource( new_model, resources::memory_usage_type( resources::nocache_memory, sizeof( damage_model ) ) );
	parent->finish_query( result_success );
}

} // namespace survarium
