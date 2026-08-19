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

bt_ghost_object::bt_ghost_object( bt_collision_shape_ptr shape, btPairCachingGhostObject* obj ):
	m_shape			( shape ),
	m_bt_object		( obj )
{
	obj->setUserPointer( this );
}

bt_ghost_object::~bt_ghost_object( )
{
	VOSTOK_DELETE_IMPL( *g_ph_allocator, m_bt_object );
}

void bt_ghost_object::get_overlapping_objects( buffer_vector<base_physics_object *>& result ) const
{
	u32 const size = get_overlapping_objects_count( );

	for ( u32 i = 0 ; i < size ; ++i )
	{
		base_physics_object* user_ptr = static_cast<base_physics_object*>(m_bt_object->getOverlappingObject( i )->getUserPointer( ));
		result.push_back( user_ptr );
	}
}

u32 bt_ghost_object::get_overlapping_objects_count( ) const
{
	return m_bt_object->getNumOverlappingObjects( );
}

u16 bt_ghost_object::get_collision_group( ) const
{
	return m_bt_object->getBroadphaseHandle( )->m_collisionFilterGroup;
}

bt_ghost_object* create_ghost_object( bt_collision_shape_ptr shape, float4x4 const& transform )
{
	btPairCachingGhostObject* ghost = VOSTOK_NEW_IMPL( *g_ph_allocator, btPairCachingGhostObject );

	ghost->setWorldTransform( from_vostok( transform ) );
	ghost->setCollisionShape( shape->get_bt_shape( ) );

	return VOSTOK_NEW_IMPL( *g_ph_allocator, bt_ghost_object )( shape, ghost );
}

void destroy_ghost_object( bt_ghost_object* obj )
{
	bt_collision_shape* shape = obj->m_shape.c_ptr( ); VOSTOK_DELETE_IMPL( *g_ph_allocator, shape );
	VOSTOK_DELETE_IMPL( *g_ph_allocator, obj );
}

void bt_ghost_object::contact_test( world* world, base_physics_object* object, contact_test_predicate& predicate )
{
	static_cast<bullet_physics_world*>(world)->get_bt_internal( )->contactPairTest( m_bt_object, object->get_bt_collision_obect( ), contact_result_callback( &predicate ) );
}

bool bt_ghost_object::contact_test( world* world )
{
	btBroadphasePairArray& bt_pair_array = m_bt_object->getOverlappingPairCache( )->getOverlappingPairArray( );


	s32	pairs_count = bt_pair_array.size( );
	for ( s32 i = 0 ; i < pairs_count ; ++i )
	{
		btBroadphasePair& bt_broadphase_pair = bt_pair_array[i];
		btOverlappingPairCache* bt_pair_cache = static_cast<bullet_physics_world*>(world)->get_bt_internal( )->getBroadphase( )->getOverlappingPairCache( );
		btBroadphasePair* bt_pair = bt_pair_cache->findPair( bt_broadphase_pair.m_pProxy0, bt_broadphase_pair.m_pProxy1 );

		if ( bt_pair )
		{
			if ( bt_pair->m_algorithm )
			{
				btAlignedObjectArray<btPersistentManifold *> manifold_results;
				bt_pair->m_algorithm->getAllContactManifolds( manifold_results );
				for ( s32 j = 0 ; j < manifold_results.size( ) ; ++j )
				{
					if ( manifold_results[j]->getNumContacts( ) )
					{
						return true;
					}
				}
			}
		}
	}

	return false;
}

static void get_non_compound_shapes_centers( btCollisionShape* shape, btTransform const& transform, vectora<float3>& centres_results )
{
	if ( shape->getShapeType( ) != COMPOUND_SHAPE_PROXYTYPE )
	{
		float3 center = from_bullet( transform.getOrigin( ) );
		centres_results.push_back( center );
	}
	else
	{
		btCompoundShape* cshape = static_cast<btCompoundShape*>(shape);
		u32 size = cshape->getNumChildShapes( );
		for ( u32 i = 0 ; i < size ; ++i )
		{
			btTransform shape_transform = transform * cshape->getChildTransform( i );
			get_non_compound_shapes_centers( cshape->getChildShape( i ), shape_transform, centres_results );
		}
	}
}

void bt_ghost_object::non_compound_shapes_centers( vectora<float3>& centres_results ) const
{
	btTransform& transform = m_bt_object->getWorldTransform( );
	get_non_compound_shapes_centers( m_shape->get_bt_shape( ), transform, centres_results );
}

void bt_ghost_object::insert( world* w, u16 group, u16 mask )
{
	static_cast<bullet_physics_world*>(w)->get_bt_internal( )->addCollisionObject( m_bt_object, group, mask );
}

void bt_ghost_object::remove( world* w )
{
	static_cast<bullet_physics_world*>(w)->get_bt_internal( )->removeCollisionObject( m_bt_object );
}

btCollisionObject* bt_ghost_object::get_bt_collision_obect( )
{
	return m_bt_object;
}

void bt_ghost_object::set_transform( float4x4 const& transform )
{
	m_bt_object->setWorldTransform( from_vostok( transform ) );
}

float4x4 bt_ghost_object::get_transform( ) const
{
	return from_bullet( m_bt_object->getWorldTransform( ) );
}

} // namespace physics
} // namespace vostok
