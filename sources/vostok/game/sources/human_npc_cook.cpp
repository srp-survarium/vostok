////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "human_npc_cook.h"
#include <vostok/resources.h>
#include <vostok/resources_query_result.h>
#include <vostok/configs_binary_config.h>
#include <vostok/configs_binary_config_value.h>

namespace survarium {

 human_npc_cook::human_npc_cook( game_world& world ) :
	translate_query_cook( resources::human_npc_class, reuse_true, use_any_thread_id ),
	m_game_world( world )
{
}

void human_npc_cook::translate_query( resources::query_result_for_cook& parent )
{
	configs::binary_config_value* t_object	= ( configs::binary_config_value* )( parent.creation_data_from_user().c_ptr() );

	if ( t_object != 0 )
	{
		pcstr npc_brain_config_path		= ( *t_object )["brain"];
		resources::query_resource		(
			npc_brain_config_path,
			resources::binary_config_class,
			boost::bind( &human_npc_cook::on_queried_data_received, this, _1 ),
			g_allocator,
			parent.user_data(),
			&parent
		);
		return;
	}

	resources::query_resource			(
		parent.get_requested_path(),
		resources::binary_config_class,
		boost::bind( &human_npc_cook::on_queried_data_received, this, _1 ),
		g_allocator,
		parent.user_data(),
		&parent
	);
}

void human_npc_cook::delete_resource( resources::resource_base* resource )
{
	VOSTOK_DELETE_IMPL					( g_allocator, resource );
}

void human_npc_cook::on_queried_data_received( resources::queries_result& data )
{
	resources::query_result_for_cook* const	parent		= data.get_parent_query();
	if ( !data.is_successful() )
	{
		R_ASSERT										( data.is_successful(), "couldn't retrieve npc config options" );
		parent->finish_query							( result_error );
		return;
	}

	configs::binary_config_ptr config					= static_cast_resource_ptr< configs::binary_config_ptr >( data[0].get_unmanaged_resource() );

	on_npc_options_received								( config->get_root(), *parent );
}

// claude@NOTE: PARKED - decoded but not bodied this pass (15 stmts, lines 70-119; needs
// careful per-statement reconstruction of a 4-element request array with per-request variant
// user_data). Decode for the next matcher:
//   L70: binary_config_value const& attributes = config_value["attributes"];   (operator[])
//   L71: human_npc* const human = NEW human_npc( m_game_world );   (malloc_impl 0x2E0 +
//        human_npc::human_npc([this+0x20]=m_game_world); NULL-guarded)
//   L73/83/97: brain_unit_path = attributes["brain_unit"], model_path = attributes["model"],
//        space_graph_path = attributes["animation_space_graph"]  (each operator[] then [eax])
//   ~L85: human->set_sound_emitter / intrusive_ptr::set on [human+0x9C] from
//        m_game_world.get_*sound* (the [edi+20h]->[+0xA8]->[+0x84] virtual world_user accessor)
//   L86/91/94: variant<32>::set<ai::brain_unit_cook_params>( {unknown_data_class, human} ) +
//        a physics_world variant (type_to_int<physics::world*>::get) + the params[4] array;
//        the "resources/animations/single/slot..." default-animation request path (line 0x52)
//   L98/99/101: requests[4] built; query_resources( requests, 4,
//        boost::bind(&on_subresources_loaded, this, _1, human), g_allocator, params, &parent )
//   L119: drop the human ref on the way out (unmanaged_intrusive_base::destroy guard)
// NEXT: reconstruct the variant user_data array + the 4 request paths, then verify with
// structure-diff. Not symbol-blocked (set_*/human_npc ctor all exist).
// STATE[STUB]
void human_npc_cook::on_npc_options_received(
	configs::binary_config_value const&		config_value,
	resources::query_result_for_cook&		parent
)
{
	// LOCALS
	// pcstr 							brain_unit_path
	// pcstr 							space_graph_path
	// pcstr 							model_path
	// ai::brain_unit_cook_params 		cook_brain_unit_params
	// variant< 32 > 					brain_unit_params
	// variant< 32 > const*[4] 			params
	// configs::binary_config_value* 	project_config
	// variant< 32 > 					physics_world
	// resources::request[4] 			requests
	// human_npc* const 				human
	// ******

	// CALL SITE INFO
	// <0x768621> -> sound::world_user& < unknown >() const
	// ******

	// FUNCTION BODY[0x768560]: 50
	// <0x76856f>|0x00f|+0x00e:'70'
	// <0x76857d>|0x01d|+0x024:'71'
	// <0>
	// <0x7685a1>|0x041|+0x034:'73'
	// <0>
	// <1>
	// <0x7685d5>|0x075|+0x00e:'76'
	// <0>
	// <1>
	// <0x7685e3>|0x083|+0x014:'79'
	// <0>
	// <1>
	// <0x7685f7>|0x097|+0x012:'82'
	// <0>
	// <1>
	// <0x768609>|0x0a9|+0x01a:'85'
	// <0x768623>|0x0c3|+0x01f:'86'
	// <0>
	// <0x768642>|0x0e2|+0x004:'88'
	// <0>
	// <1>
	// <0x768646>|0x0e6|+0x027:'91'
	// <0>
	// <1>
	// <0x76866d>|0x10d|+0x01c:'94'
	// <0>
	// <1>
	// <2>
	// <0x768689>|0x129|+0x004:'98'
	// <0x76868d>|0x12d|+0x016:'99'
	// <0>
	// <0x7686a3>|0x143|+0x008:'101'
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
	// <0x7686ab>|0x14b|+0x0e6:'119'
	// ******
}

// claude@NOTE: PARKED - decoded but not bodied this pass (17 stmts, lines 124-155; intricate
// resource_ptr lifetime management). Decode for the next matcher:
//   L124: binary_config_value human_attributes_config = data[0].get_root()["attributes"];
//   L126: if ( !data.is_successful() ) { parent->finish_query( result_error ); return; }
//        (the cmp [data+0x40],1 == result_success; error path finish_query_impl(1,1,0xb))
//   L127-145: extract from data[1..]: brain_unit_ptr (unmanaged), model_ptr
//        (animated_model_instance_ptr), new_graph (animation_space_graph_ptr),
//        default_animation (managed_resource_ptr) and set them on `human`:
//        human->set_brain_unit / set_model / set_animation_space_graph / set_default_animation
//        (member offsets 0x154 m_brain_unit, 0x2c4 m_default_animation; set_model/space_graph
//        called as static helpers in asm)
//   L146-152: parent->set_unmanaged_resource( human, {nocache_memory, 0x2E0} );
//        finish_query( result_success ); m_game_world.on_npc_attributes_received(
//        human_attributes_config, human )  (the [human+0x20]->m_game_world)
//   L154/155: drop the local resource_ptr refs (intrusive dtors).
// NEXT: reconstruct the data[i] resource extraction order + the set_* calls, verify
// structure-diff. Not symbol-blocked (set_*/on_npc_attributes_received exist).
// STATE[STUB]
void human_npc_cook::on_subresources_loaded( resources::queries_result& data, human_npc* const human )
{
	// LOCALS
	// resources::unmanaged_resource_ptr brain_unit_ptr
	// animated_model_instance_ptr 		model_ptr
	// animation_space_graph_ptr 		new_graph
	// resources::query_result_for_cook* const parent
	// resources::managed_resource_ptr 	default_animation
	// configs::binary_config_value 	human_attributes_config
	// ******

	// FUNCTION BODY[0x768270]: 31
	// <0x768270>|0x000|+0x00a:'123'	{
	// <0x76827a>|0x00a|+0x00e:'124'
	// <0>
	// <0x768288>|0x018|+0x025:'126'
	// <0x7682ad>|0x03d|+0x010:'127'
	// <0>
	// <0x7682bd>|0x04d|+0x024:'129'
	// <0>
	// <1>
	// <0x7682e1>|0x071|+0x016:'132'
	// <0>
	// <1>
	// <2>
	// <0x7682f7>|0x087|+0x041:'136'
	// <0x768338>|0x0c8|+0x049:'137'
	// <0>
	// <0x768381>|0x111|+0x046:'139'
	// <0x7683c7>|0x157|+0x009:'140'
	// <0>
	// <0x7683d0>|0x160|+0x02f:'142'
	// <0x7683ff>|0x18f|+0x034:'143'
	// <0>
	// <0x768433>|0x1c3|+0x04a:'145'
	// <0x76847d>|0x20d|+0x00b:'146'
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x768488>|0x218|+0x03a:'151'
	// <0x7684c2>|0x252|+0x00f:'152'
	// <0>
	// <0x7684d1>|0x261|-0x1e3:'154'
	// <0x7682ee>|0x07e|+0x202:'155'
	// <0x7684f0>|0x280|      :'155'	}
	// ******
}

} // namespace survarium
