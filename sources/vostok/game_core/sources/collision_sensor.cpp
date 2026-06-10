////////////////////////////////////////////////////////////////////////////
//	Created 	: 03.11.2025
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include <vostok/game_core/collision_sensor.h>

#include <vostok/game_core/base_project.h>
#include <vostok/game_core/collision_geometry.h>
#include <vostok/physics/base_physics_object.h>
#include <vostok/physics/contact_test_predicate.h>
#include <vostok/physics/world.h>

using physics::base_physics_object;
using configs::binary_config_value;

namespace survarium {

// STATE[82.21%|PARTIAL]: target CALLS the implicit link_resolver base ctor; base inlines its vtable store.
collision_sensor::collision_sensor( ) :
	link_resolver					( ),
	m_collision_geometries			( NULL ),
	m_collision_geometries_count	( 0 ),
	m_is_active						( false )

{
	// STRUCTURE DIFF: target 0 stmts / base 0 stmts (init-list only; no diverging rows)
	// VERDICT: STRUCTURE MATCH (init-list shape ok) - target emits one `call link_resolver::
	// link_resolver` (implicit base ctor) where base inlines the vtable-only store; decided by
	// the base-class decl, not this body. LTCG/ICF, non-steerable.
}

// STATE[100%|DONE]
collision_sensor::~collision_sensor( )
{
	VOSTOK_FREE_IMPL( g_allocator, m_collision_geometries );
}

// STATE[100%|DONE]
void collision_sensor::load( binary_config_value const& cfg )
{
	binary_config_value collision_table = cfg["collision_geometries"];
	m_collision_geometries_count = collision_table.size( );
	ASSERT( UNKNOWN_EXPRESSION );

	m_collision_geometries = (collision_geometry**)VOSTOK_MALLOC_IMPL( g_allocator, sizeof( collision_geometry* ) * m_collision_geometries_count, "collision_geometries" );
}

// STATE[100%|DONE]
void collision_sensor::resolve_links( base_project* p, binary_config_value cfg )
{
	binary_config_value collision_table = cfg["collision_geometries"];
	for ( u32 i = 0 ; i < m_collision_geometries_count ; ++i )
	{
		pcstr geom_name = collision_table[i]["name"];
		m_collision_geometries[i] = static_cast_checked<collision_geometry*>( p->get_object_by_name( geom_name ) );
	}
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

// STATE[97.60%|PARTIAL]: buffer_vector(ALLOCA,...) ctor inline-vs-call, see notify_and_add_incoming_objects.
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

	// STRUCTURE DIFF: target 25 stmts / base 25 stmts
	// SIZE +0x11 | 113 | buffer_vector<base_physics_object *> all_sensed_objects(  ALLOCA( ... ), objects_count );
	// SIZE +0x11 | 120 | buffer_vector<base_physics_object *> sensed_objects(  ALLOCA( ... ), all_sensed_objects_count );
	// VERDICT: STRUCTURE MATCH (shape ok) - both rows are the buffer_vector(ALLOCA,count) ctor
	// inline-vs-call shape (see notify_and_add_incoming_objects). LTCG, non-steerable.
}

// STATE[100%|DONE]
bool collision_sensor::is_filter_passed( base_physics_object* object ) const
{
	return object != NULL;
}

struct left_objects_predicate {
	inline explicit	left_objects_predicate	(
						buffer_vector<base_physics_object* > const&	current_objects,
						buffer_vector<base_physics_object *>&		objects_to_delete
					) :
					m_current_objects	( &current_objects ),
					m_objects_to_delete	( &objects_to_delete ) {}

			// STATE[100%|DONE]: `bool const` (per the target PDB locals) drops the
			// xor-before-sete; 97.67 -> 100.
			bool	operator()				( base_physics_object* obj ) const
			{
				bool const result = std::find( m_current_objects->begin( ), m_current_objects->end( ), obj ) == m_current_objects->end( );
				if ( !result )
					return false;

				m_objects_to_delete->push_back( obj );
				return true;
			}

public:
	/* 0x0000 */	buffer_vector< base_physics_object* > const*	m_current_objects;
	/* 0x0004 */	buffer_vector< base_physics_object* >*			m_objects_to_delete;
}; // struct left_objects_predicate

STATIC_SIZE_ASSERT(left_objects_predicate, 0x8);

// STATE[99.82%|PARTIAL]: target's frame is 0xc bigger, pushing spill slots to disp32; was
// mislabeled 100%|DONE.
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

	// STRUCTURE DIFF: target 4 stmts / base 4 stmts
	// SIZE -0x9  | 174 | buffer_vector<base_physics_object *> objects_to_delete(  ALLOCA( ... ), m_old_objects.size( ) );
	// SIZE -0x12 | 183 | );
	// SIZE -0x6  | 186 | 	 on_leave( objects_to_delete );
	// VERDICT: STRUCTURE MATCH (shape ok) - the slices are instruction-for-instruction identical;
	// every byte of delta is disp8-vs-disp32 on the this/buffer spills (target [ebp-8Ch]/[ebp-90h]
	// vs base [ebp-80h]/[ebp-84h]: target's frame is 0xc bigger). Frame-allocation noise,
	// non-steerable.
}

// STATE[92.62%|PARTIAL]: buffer_vector(ALLOCA,count) ctor inline-vs-call (extra size()/fill_uninitialized).
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

	// STRUCTURE DIFF: target 12 stmts / base 12 stmts
	// SIZE +0x1c | 202 | buffer_vector<base_physics_object *> incoming_objects( ALLOCA( ... ), sensed_objects.size( ) );
	// VERDICT: STRUCTURE MATCH (shape ok) - sole row is the buffer_vector(ALLOCA,count) ctor
	// inline-vs-call. LTCG, non-steerable.
}

// STATE[89.29%|PARTIAL]: buffer_vector(ALLOCA,count) ctor inline-vs-call, same as notify_and_add_incoming_objects.
void collision_sensor::notify_objects_inside( )
{
	buffer_vector<base_physics_object *> objects_inside(  ALLOCA( m_old_objects.size( ) * sizeof( base_physics_object* ) ), m_old_objects.size( ) );
	objects_inside.assign( m_old_objects.begin( ), m_old_objects.end( ) );
	on_inside( objects_inside ); // sushi@TODO: Check where debugger jumps to. In our case this is empty function.

	// STRUCTURE DIFF: target 3 stmts / base 3 stmts
	// SIZE +0x19 | 228 | buffer_vector<base_physics_object *> objects_inside(  ALLOCA( ... ), m_old_objects.size( ) );
	// VERDICT: STRUCTURE MATCH (shape ok) - sole row is the buffer_vector(ALLOCA,count) ctor
	// inline-vs-call. LTCG, non-steerable.
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
}

// STATE[100%|DONE]
void collision_sensor::contact_test( base_physics_object* object, physics::contact_test_predicate& predicate )
{
	for ( u32 i = 0 ; i < m_collision_geometries_count ; ++i )
		m_collision_geometries[i]->contact_test( object, predicate );
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
}

// STATE[93.08%|PARTIAL]: target INLINES `m_old_objects.empty()`; our base keeps it an out-of-line call.
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

	// STRUCTURE DIFF: target 11 stmts / base 11 stmts
	// SIZE -0xe | 308 | if ( m_old_objects.empty( ) )
	// VERDICT: STRUCTURE MATCH (shape ok) - sole row: target INLINES empty() (begin==end +
	// sete/movzx/test), base CALLs it out-of-line. Reverse-direction inline-vs-call - not
	// steerable from source (empty() IS the faithful spelling). The 94.32->93.08 drift was
	// cross-unit LTCG churn, not a regression in this body.
}

// STATE[100%|DONE]
collision_geometry* collision_sensor::get_collision_geometry( u32 index )
{
	ASSERT( UNKNOWN_EXPRESSION_T( m_collision_geometries ) );
	return m_collision_geometries[index];
}

} // namespace survarium
