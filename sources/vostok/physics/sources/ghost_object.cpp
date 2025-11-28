////////////////////////////////////////////////////////////////////////////
//	Created 	: 28.08.2025
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include <vostok/physics/ghost_object.h>

#include "bullet_include.h"
#include "bullet_physics_world.h"
#include <vostok/physics/bullet_utils.h>
#include <vostok/physics/contact_test_predicate.h>

namespace vostok {
namespace physics {

// STATE[98%|DONE]: Instructions are ordered slightly differently
bt_ghost_object::bt_ghost_object( bt_collision_shape_ptr shape, btPairCachingGhostObject* obj ):
	m_shape			( shape ),
	m_bt_object		( obj )
{
	obj->setUserPointer( this );	// <0x583e86>|0x000|0x000:'23'
}

// STATE[64%|DONE]: ~loose_ptr_base didn't inline. In theory might be something different
bt_ghost_object::~bt_ghost_object( )
{
	VOSTOK_DELETE_IMPL( *g_ph_allocator, m_bt_object ); // <0x583d97>|0x000|0x000:'28'
}

// STATE[48%|DONE] For some reason `push_back` was not inlined in target, it also had asserts inside, which didn't compile out.
// All of this doesn't make much sense, since this module is compiled with optimizations, and `push_back` is a header-only inline function.
// There are more functions like this.
void bt_ghost_object::get_overlapping_objects( buffer_vector<base_physics_object *>& result ) const
{
	u32 const size = get_overlapping_objects_count( );																					// <0x583875>|0x000|0x000:'33'

	for ( u32 i = 0 ; i < size ; ++i )																									// <0x583885>|0x010|0x010:'35'
	{
		base_physics_object* user_ptr = static_cast<base_physics_object*>(m_bt_object->getOverlappingObject( i )->getUserPointer( ));	// <0x583890>|0x01b|0x00b:'37'
		result.push_back( user_ptr );																									// <0x5838a2>|0x02d|0x012:'38'
	}
}

// STATE[100%|DONE]
u32 bt_ghost_object::get_overlapping_objects_count( ) const
{
	return m_bt_object->getNumOverlappingObjects( );	// <0x583860>|0x000|0x000:'44'
}

// STATE[100%|DONE]
u16 bt_ghost_object::get_collision_group( ) const
{
	// sushi@TODO: ASSERT?
	return m_bt_object->getBroadphaseHandle( )->m_collisionFilterGroup;	// <0x583740>|0x000|0x000:'50'
}

// STATE[95%|DONE] LTCG for intrusive pointer
bt_ghost_object* create_ghost_object( bt_collision_shape_ptr shape, float4x4 const& transform )
{
	btPairCachingGhostObject* ghost = VOSTOK_NEW_IMPL( *g_ph_allocator, btPairCachingGhostObject );	// <0x583f26>|0x000|0x000:'55'

	ghost->setWorldTransform( from_vostok( transform ) );											// <0x583f4f>|0x029|0x029:'57'
	ghost->setCollisionShape( shape->get_bt_shape( ) );												// <0x583f62>|0x03c|0x013:'58'

	return VOSTOK_NEW_IMPL( *g_ph_allocator, bt_ghost_object )( shape, ghost );						// <0x583f7a>|0x054|0x018:'60'
}

// STATE[100%|DONE]
void destroy_ghost_object( bt_ghost_object* obj )
{
	bt_collision_shape* shape = obj->m_shape.c_ptr(); // sushi@TODO: Understand and document why the object is destroyed outside of `resource_ptr`.
	VOSTOK_DELETE_IMPL( *g_ph_allocator, shape );	// <0x583ec1>|0x000|0x000:'66'
	VOSTOK_DELETE_IMPL( *g_ph_allocator, obj );		// <0x583ef3>|0x032|0x032:'67'
}

// STATE[100%|DONE]
void bt_ghost_object::contact_test( world* world, base_physics_object* object, contact_test_predicate& predicate )
{
	btCollisionObject* bt_test_object = object->get_bt_collision_obect( );
	static_cast<bullet_physics_world*>(world)->get_bt_internal( )->contactPairTest( m_bt_object, bt_test_object, contact_result_callback( &predicate ) ); // <0x583cc3>|0x000|0x000:'73'
}

// STATE[99%|DONE]: eax and edx were swapped for no apparent reason. Possibly we can match everything closer if we adhere to the target structure more diligently
bool bt_ghost_object::contact_test( world* world )
{
	btBroadphasePairArray& bt_pair_array = m_bt_object->getOverlappingPairCache( )->getOverlappingPairArray( );


	s32	pairs_count = bt_pair_array.size( );
	for ( s32 i = 0 ; i < pairs_count ; ++i )
	{
		btBroadphasePair& bt_broadphase_pair = bt_pair_array[i];
		btOverlappingPairCache* bt_pair_cache = static_cast<bullet_physics_world*>(world)->get_bt_internal( )->getBroadphase( )->getOverlappingPairCache( );	// <0x583780>|0x02d|0x014:'86'
		btBroadphasePair* bt_pair = bt_pair_cache->findPair( bt_broadphase_pair.m_pProxy0, bt_broadphase_pair.m_pProxy1 );

		if ( bt_pair )																																			// <0x5837a6>|0x053|0x026:'87'
		{
			if ( bt_pair->m_algorithm )																															// <0x5837aa>|0x057|0x004:'90'
			{
				btAlignedObjectArray<btPersistentManifold *> manifold_results;
				bt_pair->m_algorithm->getAllContactManifolds( manifold_results );																				// <0x5837af>|0x05c|0x005:'93'
				for ( s32 j = 0 ; j < manifold_results.size( ) ; ++j )																							// <0x5837d1>|0x07e|0x022:'94'
				{
					if ( manifold_results[j]->getNumContacts( ) )																								// <0x5837e0>|0x08d|0x00f:'96'
					{
						return true;
					}
				}
			}
		}
	}

	return false;
}

// STATE[100%|DONE]
static void get_non_compound_shapes_centers( btCollisionShape* shape, btTransform const& transform, vectora<float3>& centres_results )
{
	if ( shape->getShapeType( ) != COMPOUND_SHAPE_PROXYTYPE )													// <0x5838c6>|0x000|0x000:'134'
	{																											// <1>
		float3 center = from_bullet( transform.getOrigin( ) );													// <0x5838d8>|0x012|0x012:'136'
		centres_results.push_back( center );																	// <0x5838e0>|0x01a|0x008:'137'
	}																											// <1>
	else																										// <2>
	{																											// <3>
		btCompoundShape* cshape = static_cast<btCompoundShape*>(shape);											// <4>
		u32 size = cshape->getNumChildShapes( );																// <0x583940>|0x07a|0x060:'142'
		for ( u32 i = 0 ; i < size ; ++i )																		// <0x583943>|0x07d|0x003:'143'
		{																										// <1>
			btTransform shape_transform = transform * cshape->getChildTransform( i );							// <0x58394b>|0x085|0x008:'145'
			get_non_compound_shapes_centers( cshape->getChildShape( i ), shape_transform, centres_results );	// <0x583c68>|0x3a2|0x31d:'146'
		}																										// <1>
	}
}

// STATE[36%|DONE]: sushi@TODO: The problem with `operator->` somehow not being inlined on Master Gold with debug usage.
void bt_ghost_object::non_compound_shapes_centers( vectora<float3>& centres_results ) const
{
	btTransform& transform = m_bt_object->getWorldTransform( );									// <0x583d31>|0x000|0x000:'152'
	// bt_collision_shape* shape = m_shape.c_ptr( );											// <0x583d34>|0x003|0x003:'153' sushi@NOTE: Why not `m_shape->get_bt_shape( );`
	get_non_compound_shapes_centers( m_shape->get_bt_shape( ), transform, centres_results );	// <0x583d3f>|0x00e|0x00b:'154'
																								// <1>
}

// STATE[100%|DONE]: sushi@NOTE: Doesn't match function body, also might be some other type of cast
void bt_ghost_object::insert( world* w, u16 group, u16 mask )
{
	static_cast<bullet_physics_world*>(w)->get_bt_internal( )->addCollisionObject( m_bt_object, group, mask );

	// FUNCTION BODY
	// <1>
	// <2>
	// <3>
	// ******
}

// STATE[100%|DONE]
void bt_ghost_object::remove( world* w )
{
	static_cast<bullet_physics_world*>(w)->get_bt_internal( )->removeCollisionObject( m_bt_object );

	// FUNCTION BODY
	// <1>
	// ******
}

// STATE[100%|DONE]
btCollisionObject* bt_ghost_object::get_bt_collision_obect( )
{
	return m_bt_object;	// <0x5836f0>|0x000|0x000:'174'
}

// STATE[100%|DONE]
void bt_ghost_object::set_transform( float4x4 const& transform )
{
	m_bt_object->setWorldTransform( from_vostok( transform ) );	// <0x583d6a>|0x000|0x000:'179'
}

// STATE[100%|DONE]
float4x4 bt_ghost_object::get_transform( ) const
{
	return from_bullet( m_bt_object->getWorldTransform( ) );	// <0x583d10>|0x000|0x000:'184'
}

} // namespace physics
} // namespace vostok