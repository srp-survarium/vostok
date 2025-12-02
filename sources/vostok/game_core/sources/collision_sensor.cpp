////////////////////////////////////////////////////////////////////////////
//	Created 	: 03.11.2025
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include <vostok/game_core/collision_sensor.h>

#include <vostok/physics/base_physics_object.h>
#include <vostok/physics/contact_test_predicate.h>
#include <vostok/physics/world.h>
#include <vostok/game_core/base_project.h>
#include <vostok/game_core/collision_geometry.h>

using physics::base_physics_object;
using configs::binary_config_value;

namespace survarium {

// STATE[82.21%|PARTIAL]: `link_resolver` constructor is called in target to set `vtable`.
collision_sensor::collision_sensor( ) :
	link_resolver					( ),
	m_collision_geometries			( NULL ),
	m_collision_geometries_count	( 0 ),
	m_is_active						( false )

{
}

// STATE[100%|DONE]
collision_sensor::~collision_sensor( )
{
	VOSTOK_FREE_IMPL( g_allocator, m_collision_geometries );

	// FUNCTION BODY
	// <0x59af2c>|0x000|0x000:'24'
	// ******
}

// STATE[94.15%|DONE]: LTCG for `binary_config_value::operator[]`.
void collision_sensor::load( binary_config_value const& cfg )
{
	binary_config_value collision_table = cfg["collision_geometries"];
	m_collision_geometries_count = collision_table.size( );
	ASSERT( UNKNOWN_EXPRESSION );

	m_collision_geometries = (collision_geometry**)VOSTOK_MALLOC_IMPL( g_allocator, sizeof( collision_geometry* ) * m_collision_geometries_count, "collision_geometries" );

	// FUNCTION BODY
	// <0x59b01a>|0x000|0x000:'29'
	// <0x59b04a>|0x030|0x030:'30'
	// <0x59b058>|0x03e|0x00e:'31'
	// <1>
	// <0x59b064>|0x04a|0x00c:'33'
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <7>
	// <8>
	// <9>
	// ******
}

// STATE[BLOCKED] sushi@TODO: I don't understand -4 in `get_object_by_name`. cast?
void collision_sensor::resolve_links( base_project* p, binary_config_value cfg )
{
	binary_config_value collision_table = cfg["collision_geometries"];
	for ( u32 i = 0 ; i < m_collision_geometries_count ; ++i ) // sushi@TODO: NOT COUNT
	{
		pcstr geom_name = collision_table[i]["name"];
		// m_old_objects[i] = p->get_object_by_name( geom_name );
	}

	// FUNCTION BODY
	// <0x59a8a9>|0x000|0x000:'47'
	// <0x59a8d9>|0x030|0x030|[1]:'48'
	// <1>
	// <0x59a8f6>|0x04d|0x01d|[2]:'50'
	// <0x59a915>|0x06c|0x01f:'51'
	// <1>
	// ******
}

struct objects_filter_predicate { // sushi@NOTE: Might be in a different place
	inline	explicit	objects_filter_predicate( collision_sensor const* sensor ) : m_sensor( sensor ) { }
	inline	bool		operator()				( base_physics_object* obj ) const { return m_sensor->is_filter_passed( obj ); }

public:
	/* 0x0000 */	collision_sensor const*		m_sensor;
}; // struct objects_filter_predicate

// STATE[SKIPPED]
bool remove_loosed_ptrs_predicate( base_physics_object* object )
{
	return object == NULL;
}

// STATE[97.60%|PARTIAL]: See `notify_and_add_incoming_objects` regarding alloca.
void collision_sensor::tick( u32 time_delta_ms, u32 current_time_ms )
{
	VOSTOK_UNREFERENCED_PARAMETERS( time_delta_ms, current_time_ms );

	u32 old_objects_count = m_old_objects.size( );

	m_old_objects.erase(
		std::remove_if( 
			m_old_objects.begin( ),
			m_old_objects.end( ),
			remove_loosed_ptrs_predicate
		),
		m_old_objects.end( )
	);
	if ( old_objects_count != m_old_objects.size( ) )
		on_objetcs_loosed( m_old_objects ); // sushi@NOTE: Interesting that this callback doesn't pass "loosed" object

	u32 objects_count = 0;
	for ( u32 i = 0 ; i < m_collision_geometries_count ; ++i )
		objects_count += m_collision_geometries[i]->get_overlapping_objects_count( );

	if ( objects_count == 0 )
	{
		buffer_vector<base_physics_object *> sensed_objects( NULL, 0 );
		notify_and_erase_left_objects( sensed_objects ); // sushi@NOTE: This will remove everything, right?
		return;
	}
	// not all maybe
	buffer_vector<base_physics_object *> all_sensed_objects(  ALLOCA( objects_count * sizeof( base_physics_object* ) ), objects_count );
	for ( u32 i = 0 ; i < m_collision_geometries_count ; ++i )
		m_collision_geometries[i]->get_overlapping_objects( all_sensed_objects );

	filter_sensed_objects( all_sensed_objects );

	u32 all_sensed_objects_count = all_sensed_objects.size( );
	buffer_vector<base_physics_object *> sensed_objects(  ALLOCA( all_sensed_objects_count * sizeof( base_physics_object* ) ), all_sensed_objects_count );

	for ( u32 i = 0 ; i < all_sensed_objects_count ; ++i )
	{
		if ( contact_test( all_sensed_objects[i] ) )
			sensed_objects.push_back( all_sensed_objects[i] );
	}

	notify_and_erase_left_objects( sensed_objects );
	notify_objects_inside( );
	notify_and_add_incoming_objects( sensed_objects );


	// FUNCTION BODY
	// <0x59ab90>|0x000|0x000:'95'
	// <1>
	// <0x59abb3>|0x023|0x023:'97'
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <7>
	// <8>
	// <0x59abc4>|0x034|0x011:'106'
	// <0x59ac7e>|0x0ee|0x0ba:'107'		if ( old_objects_count != m_old_objects.size( ) )
	// <0x59ac91>|0x101|0x013:'108'			on_objetcs_loosed( m_old_objects );
	// <1>
	// <0x59acae>|0x11e|0x01d:'110'		u32 objects_count = 0;
	// <0x59acb5>|0x125|0x007|[1]:'111'	for ( u32 i = 0 ; i < m_collision_geometries_count ; ++i )
	// <0x59acd5>|0x145|0x020:'112'			objects_count += m_collision_geometries[i]->get_overlapping_objects_count( );
	// <1>
	// <0x59acf1>|0x161|0x01c:'114'		if ( objects_count == 0 )
	// <1>
	// <0x59acf7>|0x167|0x006|[2]:'116'		buffer_vector<base_physics_object *> sensed_objects( NULL, 0 );
	// <0x59ad12>|0x182|0x01b:'117'			notify_and_erase_left_objects( sensed_objects );
	// <0x59ad21>|0x191|0x00f:'118'			return;
	// <1>
	// <2>
	// <0x59ad47>|0x1b7|0x026:'121'		buffer_vector<base_physics_object *> all_sensed_objects(  ALLOCA( objects_count * sizeof( base_physics_object * ) ), objects_count );
	// <0x59ad72>|0x1e2|0x02b|[2]:'122'	for ( u32 i = 0 ; i < m_collision_geometries_count ; ++i )
	// <0x59ad92>|0x202|0x020:'123'			m_collision_geometries[i]->get_overlapping_objects( all_sensed_objects );
	// <1>
	// <0x59adac>|0x21c|0x01a:'125'		filter_sensed_objects( all_sensed_objects );
	// <1>
	// <0x59adbb>|0x22b|0x00f:'127'		u32 all_sensed_objects_count = all_sensed_objects.size( );
	// <0x59adc7>|0x237|0x00c:'128'		buffer_vector<base_physics_object *> sensed_objects(  ALLOCA( all_sensed_objects_count * sizeof( base_physics_object* ) ), all_sensed_objects_count );
	// <1>
	// <0x59adfb>|0x26b|0x034|[3]:'130'	for ( u32 i = 0 ; i < all_sensed_objects_count ; ++i )
	// <1>
	// <0x59ae15>|0x285|0x01a:'132'
	// <0x59ae51>|0x2c1|0x03c:'133'
	// <0x59ae75>|0x2e5|0x024:'134'
	// <1>
	// <0x59ae77>|0x2e7|0x002:'136'
	// <0x59ae86>|0x2f6|0x00f:'137'
	// <0x59ae91>|0x301|0x00b:'138'
	// ******
}

// STATE[100%|DONE]
bool collision_sensor::is_filter_passed( base_physics_object* object ) const
{
	return object != NULL;

	// FUNCTION BODY
	// <0x59a1e7>|0x000|0x000:'143'
	// ******
}

struct left_objects_predicate {
	inline explicit	left_objects_predicate	( 
						buffer_vector<base_physics_object* > const&	current_objects,
						buffer_vector<base_physics_object *>&		objects_to_delete
					) :
					m_current_objects	( &current_objects ),
					m_objects_to_delete	( &objects_to_delete ) {}

			// STATE[97.67%|PARTIAL]: target didn't generate xor after `std::find`.
			bool	operator()				( base_physics_object* obj ) const
			{
				bool result = std::find( m_current_objects->begin( ), m_current_objects->end( ), obj ) == m_current_objects->end( );
				if ( !result )
					return false;

				m_objects_to_delete->push_back( obj );
				return true;

				// FUNCTION BODY
				// <0xc6619>|0x000|0x000:'156'
				// <0xc665c>|0x043|0x043:'157'
				// <0xc6664>|0x04b|0x008:'158'
				// <1>
				// <0xc6668>|0x04f|0x004:'160'
				// <0xc6677>|0x05e|0x00f:'161'
				// ******
			}

public:
	/* 0x0000 */	buffer_vector< base_physics_object* > const*	m_current_objects;
	/* 0x0004 */	buffer_vector< base_physics_object* >*			m_objects_to_delete;
}; // struct left_objects_predicate

STATIC_SIZE_ASSERT(left_objects_predicate, 0x8);

// STATE[100%|DONE]
void collision_sensor::notify_and_erase_left_objects( buffer_vector<base_physics_object *>& sensed_objects )
{
	buffer_vector<base_physics_object *> objects_to_delete(  ALLOCA( m_old_objects.size( ) * sizeof( base_physics_object * ) ), m_old_objects.size( ) );

	m_old_objects.erase(
		std::remove_if( 
			m_old_objects.begin( ),
			m_old_objects.end( ),
			left_objects_predicate( sensed_objects, objects_to_delete )
		),
		m_old_objects.end( )
	);

	if ( !objects_to_delete.empty( ) )
		 on_leave( objects_to_delete );

	// FUNCTION BODY
	// <0x59a6a0>|0x000|0x000:'171'
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <7>
	// <0x59a709>|0x069|0x069:'179'
	// <1>
	// <0x59a7d8>|0x138|0x0cf:'181'
	// <0x59a7ea>|0x14a|0x012:'182'
	// ******
}

// STATE[92.62%|PARTIAL]
// sushi@NOTE: The first line seems to be written differently.
// For my code compiler generates two `size` calls after ALLOCA is called. The second one comes from the call to `fill_uninitialized`.
// Target also this behaviour in `notify_and_erase_left_objects`.
// Possibly they used `VOSTOK_ALLOCA_IMPL` directly, or `fill_uninitialized` got LTCG'ed. They never really use `VOSTOK_ALLOCA_IMPL` directly though.
void collision_sensor::notify_and_add_incoming_objects( buffer_vector<base_physics_object *>& sensed_objects )
{	
	buffer_vector<base_physics_object *> incoming_objects( ALLOCA( sensed_objects.size( ) * sizeof( base_physics_object * ) ), sensed_objects.size( ) );
	base_physics_object** inc_it	= sensed_objects.begin( );
	base_physics_object** inc_end	= sensed_objects.end( );
	for ( ; inc_it != inc_end ; ++inc_it )
		if ( std::find( m_old_objects.begin( ), m_old_objects.end( ), *inc_it ) == m_old_objects.end( ) )
			incoming_objects.push_back( *inc_it );

	if ( incoming_objects.size( ) > 0 )
		on_enter( incoming_objects );

	base_physics_object** it	= incoming_objects.begin( );
	base_physics_object** end	= incoming_objects.end( );

	for ( ; it != end ; ++it )
		m_old_objects.push_back( *it );

	// FUNCTION BODY
	// <0x59aa09>|0x000|0x000:'187' buffer_vector<base_physics_object *> incoming_objects(
	// <0x59aa50>|0x047|0x047:'188' base_physics_object** inc_it	= sensed_objects.begin( );
	// <0x59aa58>|0x04f|0x008:'189' base_physics_object** inc_end	= sensed_objects.end( ); 
	// <1>
	// <0x59aaf4>|0x0eb|0x00c:'194'	if ( incoming_objects.size( ) )
	// <0x59ab06>|0x0fd|0x012:'195'		on_enter( incoming_objects );
	// <1>
	// <0x59ab17>|0x10e|0x011:'197'
	// <0x59ab1d>|0x114|0x006:'198'
	// <1>
	// <0x59ab23>|0x11a|0x006:'200'
	// <0x59ab36>|0x12d|0x013:'201'
	// ******
}

// STATE[89.29%|PARTIAL]: Same as `notify_and_add_incoming_objects`.
void collision_sensor::notify_objects_inside( )
{
	buffer_vector<base_physics_object *> objects_inside(  ALLOCA( m_old_objects.size( ) * sizeof( base_physics_object* ) ), m_old_objects.size( ) );
	objects_inside.assign( m_old_objects.begin( ), m_old_objects.end( ) );
	on_inside( objects_inside ); // sushi@TODO: Check where debugger jumps to. In our case this is empty function.

	// FUNCTION BODY
	// <0x59a249>|0x000|0x000:'206'
	// <0x59a28a>|0x041|0x041:'207'
	// <0x59a2ce>|0x085|0x044:'208'
	// ******
}

// STATE[100%|DONE]
void collision_sensor::filter_sensed_objects( buffer_vector<base_physics_object *>& sensed_objects )
{
	sensed_objects.erase(
		std::remove_if( 
			sensed_objects.begin( ),
			sensed_objects.end( ),
			objects_filter_predicate( this )
		),
		sensed_objects.end( )
	);

	std::sort( sensed_objects.begin( ), sensed_objects.end( ) );

	sensed_objects.erase(
		std::unique( sensed_objects.begin( ), sensed_objects.end( ) ),
		sensed_objects.end( )
	);


	// FUNCTION BODY
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <7>
	// <0x59a31f>|0x000|0x000:'220'
	// <1>
	// <0x59a36f>|0x050|0x050:'222'	std::sort( sensed_objects.begin( ), sensed_objects.end( ) );
	// <1>
	// <2>
	// <3>
	// <4>
	// <0x59a390>|0x071|0x021:'227'
	// ******
}

// STATE[100%|DONE]
void collision_sensor::contact_test( base_physics_object* object, physics::contact_test_predicate& predicate )
{
	for ( u32 i = 0 ; i < m_collision_geometries_count ; ++i )
		m_collision_geometries[i]->contact_test( object, predicate );

	// FUNCTION BODY
	// <0x59a969>|0x000|0x000|[1]:'232'
	// <0x59a986>|0x01d|0x01d:'233'
	// ******
}

// STATE[100%|DONE]
bool collision_sensor::contact_test( base_physics_object* object )
{
	for ( u32 i = 0 ; i < m_collision_geometries_count ; ++i )
	{
		if ( m_collision_geometries[i]->contact_test( ) )
			return true;
	}
	return false;

	// FUNCTION BODY
	// <0x59a649>|0x000|0x000|[1]:'238'
	// <1>
	// <0x59a666>|0x01d|0x01d:'240'
	// <0x59a67e>|0x035|0x018:'241'
	// <0x59a682>|0x039|0x004:'242'
	// <0x59a684>|0x03b|0x002:'243'
	// ******
}

// STATE[BLOCKED]: sushi@TODO: protected method, will be checked when children are implemented
void collision_sensor::get_shapes_centers( vectora<float3>& centers ) const
{
	for ( u32 i = 0 ; i < m_collision_geometries_count ; ++i )
		m_collision_geometries[i]->get_shapes_centers( centers );
	
	// FUNCTION BODY
	// <0x59a9b9>|0x000|0x000|[1]:'254'
	// <0x59a9d6>|0x01d|0x01d:'255'
	// ******
}

// STATE[100%|DONE]
void collision_sensor::insert( physics::world* world )
{
	ASSERT( UNKNOWN_EXPRESSION_T( !m_is_active ) );
	m_is_active = true;

	for ( u32 i = 0 ; i < m_collision_geometries_count ; ++i )
		m_collision_geometries[i]->subscribe( world, this );

	// FUNCTION BODY
	// <0x59a849>|0x000|0x000:'260'
	// <0x59a855>|0x00c|0x00c:'261'
	// <1>
	// <0x59a85c>|0x013|0x007|[1]:'263'
	// <0x59a879>|0x030|0x01d:'264'
	// ******
}

// STATE[94.32%|DONE]: `vector::empty` didn't inline.
void collision_sensor::remove( )
{
	ASSERT( UNKNOWN_EXPRESSION_T( m_is_active ) );
	m_is_active = false;

	for ( u32 i = 0 ; i < m_collision_geometries_count ; ++i )
		m_collision_geometries[i]->unsubscribe( this );

	if ( m_old_objects.empty( ) )
		return;

	m_old_objects.erase(
		std::remove_if( 
			m_old_objects.begin( ),
			m_old_objects.end( ),
			remove_loosed_ptrs_predicate
		),
		m_old_objects.end( )
	);

	buffer_vector<base_physics_object *> leaved(  ALLOCA( m_old_objects.size( ) * sizeof( base_physics_object * ) ), m_old_objects.size( ) );
	leaved.assign( m_old_objects.begin( ), m_old_objects.end( ) );
	on_leave( leaved ); // sushi@TODO: Check where debugger jumps to. In our case this is empty function.
	m_old_objects.clear( );

	// FUNCTION BODY
	// <0x59a3f1>|0x000|0x000:'269'
	// <0x59a3fd>|0x00c|0x00c:'270'		m_is_active = false;
	// <1>
	// <0x59a407>|0x016|0x00a|[1]:'272'	for ( u32 i = 0 ; i < m_collision_geometries_count ; ++i )
	// <0x59a427>|0x036|0x020:'273'			m_collision_geometries[i]->unsubscribe( this );
	// <1>
	// <0x59a444>|0x053|0x01d:'275'		if ( m_old_objects.empty( ) )
	// <0x59a467>|0x076|0x023:'276'			return;
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <7>
	// <8>
	// <9>
	// <0x59a46c>|0x07b|0x005:'286'
	// <1>
	// <0x59a526>|0x135|0x0ba:'288' buffer_vector<physi
	// <0x59a58f>|0x19e|0x069:'289'
	// <0x59a5d9>|0x1e8|0x04a:'290'
	// <0x59a5f0>|0x1ff|0x017:'291'
	// ******
}

// STATE[100%|DONE]
collision_geometry* collision_sensor::get_collision_geometry( u32 index )
{
	ASSERT( UNKNOWN_EXPRESSION_T( m_collision_geometries ) );
	return m_collision_geometries[index];

	// FUNCTION BODY
	// <0x59a219>|0x000|0x000:'296'
	// <0x59a225>|0x00c|0x00c:'297'
	// ******
}

} // namespace survarium
