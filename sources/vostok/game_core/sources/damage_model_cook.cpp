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

/*
static fixed_vector< console_commands::command_token, 12 >	damage_model_cook::m_hit_types;
static fixed_vector< fixed_string<24>, 12 >					damage_model_cook::m_hit_types_strings;
*/

// STATE[62.35%|DONE]
damage_model_cook::damage_model_cook( ) :
	resources::translate_query_cook( resources::damage_model_class, reuse_false, use_current_thread_id )
{
	resources::register_cook( this );
}

// STATE[93.90%|DONE]
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

// STATE[93.26%|DONE]: Regarding most todos, the memory for the whole thing was allocated once and calling free is correct. I will remove those comments ones I match the constructor.
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
				{} // it->first->~body_part_parameters( );										// sushi@NOTE: Uncommenting this line results in worse match, but I don't see any other reason to have this code.

			type->~hit_type_parameters( ); // VOSTOK_DELETE_IMPL( g_allocator, type );			// sushi@TODO: Either mistake by them, or someone else clears this memory.
		}

		while ( affects_threshold* threshold = part->pop_threshold( ) )
		{
			hit_affects_type_enum const* it_affects		= threshold->get_affects( );
			hit_affects_type_enum const* it_affects_end	= it_affects + threshold->get_affects_count( );
			for ( hit_affects_type_enum const* it = it_affects ; it != it_affects_end; ++it )
				it->~hit_affects_type_enum( );													// sushi@NOTE: Don't see the point of running manual destructors on enums.

			threshold->~affects_threshold( ); // VOSTOK_DELETE_IMPL( g_allocator, threshold );	// sushi@TODO: Either mistake by them, or someone else clears this memory.
		}

		part->~body_part_parameters( ); // VOSTOK_DELETE_IMPL( g_allocator, part );				// sushi@TODO: Either mistake by them, or someone else clears this memory.
	}

	model_res->~damage_model( );																// sushi@NOTE:`VOSTOK_DELETE_IMPL` could have been used instead.
	VOSTOK_FREE_IMPL( g_allocator, resource );													// sushi@MATCH: In target `free_helper` didn't inline into `free_helper_impl`. // sushi@TODO: Incorrect allocator?

	// FUNCTION BODY
	// <0x7610aa>|0x00a|+0x006:'39'		damage_model* model_res = static_cast<damage_model*>( resource );
	// <0>
	// <0x7610b0>|0x010|+0x015|[1]:'41'	while ( body_part_parameters* part = model_res->pop_body_part( ) )
	// <0>								{
	// <0x7610c5>|0x025|+0x011|[2]:'43'		while ( hit_type_parameters* type = part->pop_hit_type( ) )
	// <0>									{
	// <0x7610d6>|0x036|+0x015|[3]:'45'			std::pair< body_part_parameters *, float > const* it_bdb_begin	= type->get_bdb_coefficients( )
	// <0x7610eb>|0x04b|+0x015:'46'				std::pair< body_part_parameters *, float > const* it_bdb_end	= it_begin + m_bdb_count;
	// <0x761100>|0x060|+0x019|[4]:'47'			for ( std::pair<body_part_parameters *,float> const* it = it_bdb_begin ; it != it_bdb_end; ++it )
	// <0x761119>|0x079|+0x002:'48'					{}
	// <0>
	// <0x76111b>|0x07b|+0x01b:'50'				VOSTOK_DELETE_IMPL( g_allocator, type ); !
	// <0x761136>|0x096|+0x002:'51'			}
	// <0x761138>|0x098|+0x011|[2]:'52'		while ( affects_threshold* threshold = part->pop_threshold( ) )
	// <0>									{
	// <0x761149>|0x0a9|+0x015|[3]:'54'			hit_affects_type_enum const* it_affects		= threshold->get_affects( );
	// <0x76115e>|0x0be|+0x015:'55'				hit_affects_type_enum const* it_affects_end	= it_affects + threshold->get_affects_count( );
	// <0x761173>|0x0d3|+0x019|[4]:'56'			for ( hit_affects_type_enum const* it = it_affects ; it != it_affects_end; ++it )
	// <0x76118c>|0x0ec|+0x002:'57'					{}
	// <0>
	// <0x76118e>|0x0ee|+0x01b:'59'				VOSTOK_DELETE_IMPL( g_allocator, threshold ); !
	// <0x7611a9>|0x109|+0x002:'60'			}
	// <0x7611ab>|0x10b|+0x01b:'61'			VOSTOK_DELETE_IMPL( g_allocator, part ); !
	// <0x7611c6>|0x126|+0x005:'62'		}
	// <0>
	// <0x7611cb>|0x12b|+0x00e:'64'		model_res->~damage_model( );
	// <0x7611d9>|0x139|+0x00e:'65'		VOSTOK_DELETE_IMPL( g_allocator, resource );
	// ******
}

// STATE[BLOCKED]
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

	// FUNCTION BODY
	// <0x760959>|0x009|+0x00b:'250'	const u32 body_parts_count = model_value.size( );
	// <0x760964>|0x014|+0x011:'251'	u32 result = sizeof( body_part_parameters ) * body_parts_count;
	// <0>
	// <0x760975>|0x025|+0x00b:'253'	configs::binary_config_value const* it_model		= model_value.begin( );
	// <0x760980>|0x030|+0x00b:'254'	configs::binary_config_value const* it_model_end	= model_value.end( );
	// <0>
	// <0x76098b>|0x03b|+0x017:'256'	for ( ; it_model != it_model_end ; ++it_model
	// <0>
	// <0x7609a2>|0x052|+0x006|[1]:'258'	configs::binary_config_value const& part_value	= *it_model;
	// <0x7609a8>|0x058|+0x010:'259'		configs::binary_config_value const& types_value = part_value["hit_types"];
	// <0>
	// <0x7609b8>|0x068|+0x011:'261'		result += sizeof( hit_type_parameters ) * types_value.size( );
	// <0>
	// <0x7609c9>|0x079|+0x00b:'263'		configs::binary_config_value const* it_type		= types_value.begin( );
	// <0x7609d4>|0x084|+0x00b:'264'		configs::binary_config_value const* it_type_end	= types_value.end( );
	// <0>
	// <0x7609df>|0x08f|+0x013:'266'		for ( ; it_type != it_type_end ; ++it_type )
	// <0>
	// <0x7609f2>|0x0a2|+0x006|[2]:'268'	configs::binary_config_value const& type_value = *it_type;
	// <0x7609f8>|0x0a8|+0x010:'269'		configs::binary_config_value const& bdbs_value = type_value["bdb_coeff"];
	// <0>
	// <0x760a08>|0x0b8|+0x011:'271'		result += 8 /* sizeof */ * bdbs_value.size( );
	// <0x760a19>|0x0c9|+0x002:'272'	}
	// <0>
	// <0x760a1b>|0x0cb|+0x010:'274'	configs::binary_config_value const& thresholds_value = part_value["thresholds"];
	// <0>
	// <0x760a2b>|0x0db|+0x011:'276'	result += sizeof ( affects_threshold ) * thresholds_value.size( );
	// <0>
	// <0x760a3c>|0x0ec|+0x00b:'278'	configs::binary_config_value const* it_threshold		= thresholds_value.begin( );
	// <0x760a47>|0x0f7|+0x00b:'279'	configs::binary_config_value const* it_threshold_end	= thresholds_value.end( );
	// <0>
	// <0x760a52>|0x102|+0x013:'281'	for ( ; it_threshold != it_threshold_end ; ++it_threshold )
	// <0>
	// <0x760a65>|0x115|+0x006|[2]:'283'	configs::binary_config_value const& threshold_value = *it_threshold;
	// <0x760a6b>|0x11b|+0x010:'284'		configs::binary_config_value const& affects_value	= threshold_value["affects"];
	// <0>
	// <0x760a7b>|0x12b|+0x011:'286'		result += sizeof( hit_affects_type_enum ) * affects_value.size( );
	// <0x760a8c>|0x13c|+0x002:'287'	}
	// <0x760a8e>|0x13e|+0x005:'288'	}
	// <0>
	// <0x760a93>|0x143|+0x003:'290'	return result;
	// ******
}

// STATE[BLOCKED]
static hit_type_parameters* create_hit_type_parameters(
	damage_model* const						model,
	memory::stack_allocator*				allocator,
	configs::binary_config_value const&		type_value
){
	const u32 bdb_coeffs_count = type_value["bdb_coeff"];

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

	// FUNCTION BODY[0x760c30]: 19
	// <0x760c39>|0x009|+0x017:'299'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <0x760c50>|0x020|+0x0aa:'306'
	// <0>
	// <0x760cfa>|0x0ca|+0x017:'308'	configs::binary_config_value const* it_bdb		= type_value["bdb_coe
	// <0x760d11>|0x0e1|+0x017:'309'	configs::binary_config_value const* it_bdb_end	= type_value["bdb_coeff"].end( );
	// <0>
	// <0x760d28>|0x0f8|+0x017:'311'	for ( ; it_bdb != it_bdb_end ; ++it_bdb )
	// <0>
	// <0x760d3f>|0x10f|+0x014:'313'
	// <0x760d53>|0x123|+0x00c:'314'
	// <0x760d5f>|0x12f|+0x0cd:'315'
	// <0x760e2c>|0x1fc|+0x005:'316'
	// <0x760e31>|0x201|+0x003:'317'
	// ******
}

// STATE[BLOCKED]
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

	// FUNCTION BODY[0x760aa0]: 17
	// <0x760aa9>|0x009|+0x030:'324'
	// <0x760ad9>|0x039|+0x00b:'325'
	// <0x760ae4>|0x044|+0x015:'326'
	// <0x760af9>|0x059|+0x00f:'327'
	// <0x760b08>|0x068|+0x00c:'328'	ASSERT( UNKNOWN_EXPRESSION_T( bodypart ) );
	// <0x760b14>|0x074|+0x075:'329'
	// <0>
	// <0x760b89>|0x0e9|+0x00b:'331'
	// <0x760b94>|0x0f4|+0x00b:'332'
	// <0>
	// <0x760b9f>|0x0ff|+0x013:'334'	for ( ; it_affect != it_affect_end ; ++it_affect )
	// <0>								{
	// <0x760bb2>|0x112|+0x00b:'336'		const u32				affect_type = *it_affect;
	// <0x760bbd>|0x11d|+0x006:'337'		hit_affects_type_enum	type		= (hit_affects_type_enum)affect_type;
	// <0x760bc3>|0x123|+0x05b:'338'
	// <0x760c1e>|0x17e|+0x002:'339'	}
	// <0x760c20>|0x180|+0x003:'340'
	// ******
}

// STATE[BLOCKED]
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

	// FUNCTION BODY[0x760e40]: 25
	// <0x760e46>|0x006|+0x010:'350'
	// <0>
	// <0x760e56>|0x016|+0x00b:'352'
	// <0x760e61>|0x021|+0x00b:'353'
	// <0>
	// <0x760e6c>|0x02c|+0x013:'355'	for ( ; it_type != it_type_end ; ++it_type )
	// <0>								{
	// <0x760e7f>|0x03f|+0x006|[1]:'357'	configs::binary_config_value const& type_value = *it_type;
	// <0>
	// <0x760e85>|0x045|+0x017:'359'		hit_type_parameters* hit_type_params = create_hit_type_parame
	// <0x760e9c>|0x05c|+0x00c:'360'
	// <0x760ea8>|0x068|+0x002:'361'	}
	// <0>
	// <0x760eaa>|0x06a|+0x010:'363'	configs::binary_config_value const& thresholds_value = part_value["thresholds"];
	// <0>
	// <0x760eba>|0x07a|+0x00b:'365'	configs::binary_config_value const* it_threshold	 = thresholds_value.begin( );
	// <0x760ec5>|0x085|+0x00b:'366'	configs::binary_config_value const* it_threshold_end = thresholds_value.end( );
	// <0>
	// <0x760ed0>|0x090|+0x013:'368'	for ( ; it_threshold != it_threshold_end ; ++it_threshold )
	// <0>
	// <0x760ee3>|0x0a3|+0x006|[1]:'370'	configs::binary_config_value const& threshold_value = *it_threshold;
	// <0>
	// <0x760ee9>|0x0a9|+0x017:'372'
	// <0x760f00>|0x0c0|+0x00e:'373'
	// <0>
	// ******
}

// STATE[BLOCKED] sushi@NOTE: Possibly requires proper casts
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

	// FUNCTION BODY
	// <0>
	// <7>
	// <0x760876>|0x006|+0x0cf:'413'
	// <0x760945>|0x0d5|+0x003:'414'
	// ******
}

// STATE[STUB]
static void fill_damage_model(
	damage_model* const						model,
	memory::stack_allocator*				allocator,
	configs::binary_config_value const&		model_value,
	configs::binary_config_value const&		damage_groups
)
{
	// LOCALS
	// configs::binary_config_value const* it_model
	// configs::binary_config_value const* it_model_end
	// u8 								damage_group<1>
	// configs::binary_config_value const& part_value<1>
	// body_part_parameters* 			new_body_part<1>
	// u8 								i<2>
	// configs::binary_config_value const* it_part<3>
	// configs::binary_config_value const* it_part_end<3>
	// configs::binary_config_value const& part_value<1>
	// body_part_parameters* 			body_part<1>
	// ******

	// TYPEDEFS
	// typedef
	// 	configs::binary_config_value const*
	// 	config_iterator;

	// ******

	// FUNCTION BODY[0x760f20]: 68
	// <0x760f27>|0x007|+0x00b:'419'
	// <0x760f32>|0x012|+0x00b:'420'
	// <0>
	// <7>
	// <0x760f3d>|0x01d|+0x017:'429'
	// <0>
	// <0x760f54>|0x034|+0x006|[1]:'431'
	// <0x760f5a>|0x03a|+0x004:'432'
	// <0>
	// <1>
	// <0x760f5e>|0x03e|+0x00c:'435'
	// <0x760f6a>|0x04a|+0x01f|[2]:'436'
	// <0x760f89>|0x069|+0x03f|[3]:'437'
	// <0x760fc8>|0x0a8|+0x02a:'438'
	// <0>
	// <0x760ff2>|0x0d2|+0x006:'440'
	// <0x760ff8>|0x0d8|+0x002:'441'
	// <0x760ffa>|0x0da|+0x002:'442'
	// <0>
	// <1>
	// <0x760ffc>|0x0dc|+0x021:'445'
	// <0x76101d>|0x0fd|+0x00c:'446'
	// <0>
	// <4>
	// <0x761029>|0x109|+0x005:'452'
	// <0>
	// <1>
	// <2>
	// <0x76102e>|0x10e|+0x00b:'456'
	// <0>
	// <6>
	// <0x761039>|0x119|+0x013:'464'
	// <0>
	// <0x76104c>|0x12c|+0x006|[1]:'466'
	// <0>
	// <0x761052>|0x132|+0x01e:'468'
	// <0x761070>|0x150|+0x00c:'469'
	// <0x76107c>|0x15c|+0x01a:'470'
	// <0>
	// <15>
	// ******
}

// STATE[STUB]
void damage_model_cook::on_hit_params_received( resources::queries_result& data )
{
	// LOCALS
	// variant< 32 >* 					ud
	// configs::binary_config_value const& params_value
	// pcstr 							description
	// void* 							model_buffer
	// damage_model* const 				new_model
	// resources::query_result_for_cook* const parent
	// const u32 						model_buffer_size
	// affects_applying_type_enum 		affects_applying_type
	// configs::binary_config_value const& config_value
	// configs::binary_config_ptr 		config
	// memory::stack_allocator 			stack_allocator
	// configs::binary_config_value const& damage_groups
	// configs::binary_config_value const& hit_types<1>
	// configs::binary_config_value const* it_hit_end<1>
	// configs::binary_config_value const* it_hit<1>
	// u32 								i<2>
	// console_commands::command_token 	new_command<3>
	// ******

	// STATICS
	// static bool 						hit_types_initialized = <0x4c26671>;
	// ******

	// FUNCTION BODY[0x7611f0]: 56
	// <0x761200>|0x010|+0x00b:'491'
	// <0x76120b>|0x01b|+0x00f:'492'
	// <0>
	// <0x76121a>|0x02a|+0x00c:'494'
	// <0x761226>|0x036|+0x00c:'495'
	// <0x761232>|0x042|+0x005:'496'
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x761237>|0x047|+0x022:'501'
	// <0x761259>|0x069|+0x010:'502'
	// <0x761269>|0x079|+0x010:'503'
	// <0x761279>|0x089|+0x010:'504'
	// <0>
	// <0x761289>|0x099|+0x00f:'506'
	// <0>
	// <0x761298>|0x0a8|+0x010|[1]:'508'
	// <0>
	// <1>
	// <2>
	// <0x7612a8>|0x0b8|+0x00b:'512'
	// <0x7612b3>|0x0c3|+0x00b:'513'
	// <0>
	// <0x7612be>|0x0ce|+0x027|[2]:'515'
	// <0>
	// <0x7612e5>|0x0f5|+0x046|[3]:'517'
	// <0x76132b>|0x13b|+0x034:'518'
	// <0x76135f>|0x16f|+0x00e:'519'
	// <0x76136d>|0x17d|+0x005:'520'
	// <0x761372>|0x182|+0x0bc:'521'
	// <0x76142e>|0x23e|+0x007:'522'
	// <0>
	// <1>
	// <0x761435>|0x245|+0x00f:'525'
	// <0x761444>|0x254|+0x007:'526'
	// <0>
	// <0x76144b>|0x25b|+0x01d:'528'
	// <0>
	// <0x761468>|0x278|+0x008:'530'
	// <0x761470>|0x280|+0x017:'531'
	// <0>
	// <1>
	// <0x761487>|0x297|+0x00b:'534'
	// <0x761492>|0x2a2|+0x012:'535'
	// <0x7614a4>|0x2b4|+0x00c:'536'
	// <0>
	// <0x7614b0>|0x2c0|+0x071:'538'
	// <0x761521>|0x331|+0x018:'539'
	// <0x761539>|0x349|+0x012:'540'
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x76154b>|0x35b|+0x036:'545'
	// <0x761581>|0x391|+0x00c:'546'
	// ******
}

} // namespace survarium
