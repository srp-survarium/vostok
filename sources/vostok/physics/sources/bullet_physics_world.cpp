////////////////////////////////////////////////////////////////////////////
//	Created 	: 28.08.2025
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "bullet_physics_world.h"

#include "bullet_include.h"
#include <vostok/console_command.h>
#include <vostok/physics/bullet_utils.h>
#include <vostok/physics/collision_shapes.h>
#include <vostok/physics/engine.h>
#include <vostok/physics/rigid_body_base.h>
#include <vostok/physics/contact_test_predicate.h>

#include "LinearMath/btQuickProf.h"

static u32 s_physics_max_substeps_value = 1;
static console_commands::cc_u32 s_physics_max_substeps_cc (
	"physics_max_substeps",
	s_physics_max_substeps_value,
	0, 100,
	true,
	console_commands::command_type_engine_internal
);

static bool s_debug_draw_walkable = true;
static console_commands::cc_bool s_ph_debug_cmd01 (
	"draw_physics_debug_walkable",
	s_debug_draw_walkable,
	false,
	console_commands::command_type_user_specific
);

static bool s_debug_draw_hittable = true;
static console_commands::cc_bool s_ph_debug_cmd02 (
	"draw_physics_debug_hittable",
	s_debug_draw_hittable,
	false,
	console_commands::command_type_user_specific
);

static bool s_debug_draw_sensor = true;
static console_commands::cc_bool s_ph_debug_cmd03 (
	"draw_physics_debug_sensor",
	s_debug_draw_sensor,
	false,
	console_commands::command_type_user_specific
);

void dump_physics_profiler( pcstr __formal )
{
	CProfileManager::dumpAll( );
}

void reset_physics_profiler( pcstr __formal )
{
	CProfileManager::Reset();
}

static console_commands::cc_delegate s_dump_statistics (
	"dump_physics_profiler", &dump_physics_profiler, false
);

static console_commands::cc_delegate s_reset_statistics (
	"reset_physics_profiler", &reset_physics_profiler, false
);


void* bullet_alloc( u32 size )
{
	return VOSTOK_MALLOC_IMPL( vostok::physics::g_ph_allocator, size, "bullet" );
}

void bullet_free( void* memblock )
{
	return VOSTOK_FREE_IMPL( vostok::physics::g_ph_allocator, memblock );
}

namespace vostok {
namespace physics {

btTransform from_vostok( float4x4 const& m )
{
	math::quaternion q_vostok = math::quaternion(m);
	btQuaternion q0		= from_vostok(q_vostok);
	return btTransform(q0, from_vostok(m.c.xyz()) );
}

float4x4 from_bullet( btTransform const& m )
{
	btQuaternion q_bullet	= m.getRotation();
	math::quaternion q_vostok = from_bullet( q_bullet );
	return create_rotation(q_vostok) * create_translation( from_bullet(m.getOrigin()) );
}

bullet_physics_world::bullet_physics_world( memory::base_allocator& allocator, engine& engine ):
	m_allocator		( allocator ),
	m_engine		( engine ),
	m_world_aabb	( math::create_aabb_min_max(
		float3( math::infinity, math::infinity, math::infinity ),
		float3( -math::infinity, -math::infinity, -math::infinity )
	))
{
}

void log_cb( char* text )
{
	LOG_INFO( text );
}

void bullet_physics_world::initialize( )
{
	btAlignedAllocSetCustom		( bullet_alloc, bullet_free );

	btVector3 worldMin			(-1000,-1000,-1000);
	btVector3 worldMax			(1000,1000,1000);

	//SoftDicsreteDynamicWorld
	m_softBodyWorldInfo						= VOSTOK_NEW_IMPL( m_allocator, btSoftBodyWorldInfo );
	m_softBodyWorldInfo->air_density		= (btScalar)1.2;
	m_softBodyWorldInfo->water_density		= 0;
	m_softBodyWorldInfo->water_offset		= 0;
	m_softBodyWorldInfo->water_normal		= btVector3(0,0,0);
	m_softBodyWorldInfo->m_gravity.setValue	(0,-10,0);

	const int maxProxies = 32766;
	m_collisionConfiguration	= VOSTOK_NEW_IMPL( m_allocator, btSoftBodyRigidBodyCollisionConfiguration )( );
	m_dispatcher				= VOSTOK_NEW_IMPL( m_allocator, btCollisionDispatcher )( m_collisionConfiguration );
	btGImpactCollisionAlgorithm::registerAlgorithm( m_dispatcher );

	m_softBodyWorldInfo->m_dispatcher = m_dispatcher;
	m_overlappingPairCache		= VOSTOK_NEW_IMPL( m_allocator, btAxisSweep3 )( worldMin, worldMax, maxProxies );
	m_softBodyWorldInfo->m_broadphase = m_overlappingPairCache;

	m_constraintSolver			= VOSTOK_NEW_IMPL( m_allocator, btSequentialImpulseConstraintSolver )();
	m_dynamicsWorld				= VOSTOK_NEW_IMPL( m_allocator, btSoftRigidDynamicsWorld )(m_dispatcher,
																						m_overlappingPairCache,
																						m_constraintSolver,
																						m_collisionConfiguration );

	m_dynamicsWorld->getDispatchInfo().m_enableSPU = false;//true; //?
	m_dynamicsWorld->setGravity(btVector3(0,-10,0));
	m_softBodyWorldInfo->m_gravity.setValue(0,-10,0);
	m_softBodyWorldInfo->m_sparsesdf.Initialize();
	m_softBodyWorldInfo->m_sparsesdf.Reset();

	m_ghost_pair_callback		= VOSTOK_NEW_IMPL( m_allocator, btGhostPairCallback );
	m_dynamicsWorld->getBroadphase( )->getOverlappingPairCache( )->setInternalGhostPairCallback( m_ghost_pair_callback );

	m_last_frame_delta = 0.0f;
	m_last_frame_time = 0.0f;
	CProfileManager::set_log_callback( log_cb );
}

void bullet_physics_world::destroy( )
{
	m_dynamicsWorld->getBroadphase( )->getOverlappingPairCache( )->setInternalGhostPairCallback( NULL );

	VOSTOK_DELETE_IMPL( m_allocator, m_ghost_pair_callback );
	VOSTOK_DELETE_IMPL( m_allocator, m_dynamicsWorld );
	VOSTOK_DELETE_IMPL( m_allocator, m_constraintSolver );
	VOSTOK_DELETE_IMPL( m_allocator, m_overlappingPairCache );
	VOSTOK_DELETE_IMPL( m_allocator, m_dispatcher );
	VOSTOK_DELETE_IMPL( m_allocator, m_collisionConfiguration );
	VOSTOK_DELETE_IMPL( m_allocator, m_softBodyWorldInfo );

	m_last_frame_delta = 0.0;
	m_last_frame_time = 0.0;
}

void bullet_physics_world::set_renderer( btIDebugDraw* const renderer )
{
	m_dynamicsWorld->setDebugDrawer( renderer );
}

void bullet_physics_world::draw_object( btCollisionShape* const shape, btTransform const& transform, btVector3 const& color )
{
	m_dynamicsWorld->debugDrawObject( transform, shape, color );
}

void bullet_physics_world::on_before_reuse( )
{
	m_last_frame_delta = 0.0f;
	m_last_frame_time = 0.0f;
}

void bullet_physics_world::tick( u32 current_time_in_ms )
{
	m_last_frame_delta =
		( current_time_in_ms * math::epsilon_3 - m_last_frame_time ) * 0.1f
		+ m_last_frame_delta * 0.9f;
	m_last_frame_time = current_time_in_ms * math::epsilon_3;

	s32 max_substeps = math::floor( m_last_frame_delta * 60.0f + 0.5f );
	m_dynamicsWorld->stepSimulation(
		m_last_frame_delta,
		max_substeps > s_physics_max_substeps_value ? 1 : max_substeps
	);

	notify_about_contact( );

	m_softBodyWorldInfo->m_sparsesdf.GarbageCollect( );
}

void bullet_physics_world::debug_draw_world( )
{
	btCollisionObjectArray& objects = m_dynamicsWorld->getCollisionObjectArray( );
	for ( int i = 0 ; i < objects.size( ) ; ++i )
	{
		btCollisionObject* obj = objects[ i ];

		btVector3 object_colors[3];
		object_colors[0] = btVector3( 1, 0, 1 );
		object_colors[1] = btVector3( 1, 1, 0 );
		object_colors[2] = btVector3( 0, 1, 1 );

		s16 group = obj->getBroadphaseHandle( )->m_collisionFilterGroup;
		int color_idx = -1;
		if ( s_debug_draw_walkable && ( group & 6 ) )
			color_idx = 0;
		if ( s_debug_draw_hittable && ( group & 8 ) )
			color_idx = 1;
		if ( s_debug_draw_sensor && ( group & 0x81 ) )
			color_idx = 2;
		else if ( color_idx == -1 )
			continue;

		m_dynamicsWorld->debugDrawObject( obj->getWorldTransform( ), obj->getCollisionShape( ), object_colors[ color_idx ] );
	}
}

void bullet_physics_world::add( bt_rigid_body_base* body, u16 filter_group, u16 filter_mask )
{
	m_dynamicsWorld->addRigidBody( body->get_rigid_body( ), filter_group, filter_mask);

	btTransform trans = body->get_rigid_body( )->getWorldTransform( );

	btVector3 minAabb;
	btVector3 maxAabb;
	body->get_rigid_body( )->getCollisionShape( )->getAabb( trans, minAabb, maxAabb );
	m_world_aabb.modify( from_bullet( minAabb ) );
	m_world_aabb.modify( from_bullet( maxAabb ) );
}

void bullet_physics_world::remove( bt_rigid_body_base* body )
{
	m_dynamicsWorld->removeRigidBody( body->get_rigid_body( ) );
}

void bullet_physics_world::move( bt_rigid_body_base* body, float4x4 const& new_transform )
{
	body->set_transform( new_transform );
}

void bullet_physics_world::add( bt_soft_body_rope* body )
{
	m_dynamicsWorld->addSoftBody( body->m_bt_body );
}

void bullet_physics_world::remove( bt_soft_body_rope* body )
{
	m_dynamicsWorld->removeSoftBody( body->m_bt_body );
}

void bullet_physics_world::add( bt_constraint* constraint )
{
	m_dynamicsWorld->addConstraint( constraint->m_bt_typed_constraint );
}

void bullet_physics_world::remove( bt_constraint* constraint )
{
	m_dynamicsWorld->removeConstraint( constraint->m_bt_typed_constraint );
}

void bullet_physics_world::create_test_scene( )
{
}

struct closest_ray_result_callback : btCollisionWorld::RayResultCallback {
public:
						closest_ray_result_callback	( btVector3 const& rayFromWorld, btVector3 const& rayToWorld );

	virtual	float		addSingleResult				( btCollisionWorld::LocalRayResult& rayResult, bool normalInWorldSpace ) override;

public:
	/* 0x0000 */	/* btCollisionWorld::RayResultCallback */
	/* 0x0020 */	btVector3		m_rayFromWorld;
	/* 0x0030 */	btVector3		m_rayToWorld;
	/* 0x0040 */	btVector3		m_hitNormalWorld;
	/* 0x0050 */	btVector3		m_hitPointWorld;
	/* 0x0060 */	s32				m_triangleIndex;
	/* 0x0064 */	bool			m_is_shape_index;
}; // struct closest_ray_result_callback

STATIC_SIZE_ASSERT(closest_ray_result_callback, 0x70);

closest_ray_result_callback::closest_ray_result_callback( btVector3 const& rayFromWorld, btVector3 const& rayToWorld ) :
	m_rayFromWorld		( rayFromWorld ),
	m_rayToWorld		( rayToWorld ),
	m_triangleIndex		( -1 ),
	m_is_shape_index	( false )
{
}

float closest_ray_result_callback::addSingleResult( btCollisionWorld::LocalRayResult& rayResult, bool normalInWorldSpace )
{
	m_closestHitFraction = rayResult.m_hitFraction;
	m_collisionObject = rayResult.m_collisionObject;

	if ( rayResult.m_localShapeInfo )
	{
		m_triangleIndex = rayResult.m_localShapeInfo->m_triangleIndex;
		m_is_shape_index = rayResult.m_localShapeInfo->m_is_shape_index;
	} else
	{
		m_triangleIndex = -1;
		m_is_shape_index = false;
	}

	if ( normalInWorldSpace )
	{
		m_hitNormalWorld = rayResult.m_hitNormalLocal;
	} else
	{
		m_hitNormalWorld = m_collisionObject->getWorldTransform( ).getBasis( ) * rayResult.m_hitNormalLocal;
	}

	m_hitPointWorld.setInterpolate3( m_rayFromWorld, m_rayToWorld, rayResult.m_hitFraction );
	return rayResult.m_hitFraction;
}

closest_ray_result bullet_physics_world::ray_test(
	float3 const&		ray_from,
	float3 const&		ray_dir,
	float const			ray_length,
	u16					filter_group,
	u16					filter_mask
)
{
	btVector3 from = from_vostok( ray_from );
	btVector3 to = from_vostok( ray_from + ray_dir * ray_length );

	closest_ray_result_callback cb( from, to );
	cb.m_collisionFilterGroup = filter_group; cb.m_collisionFilterMask = filter_mask; cb.m_flags |= 1 << 1;

	m_dynamicsWorld->rayTest( from, to, cb );

	closest_ray_result result;

	if ( cb.m_collisionObject )
	{
		result.hit_point_world = from_bullet( cb.m_hitPointWorld );
		result.object = static_cast< base_physics_object* >( cb.m_collisionObject->getUserPointer( ) );
		result.triangle_index = cb.m_triangleIndex;
		result.is_shape_index = cb.m_is_shape_index;
		result.hit_normal_world = from_bullet( cb.m_hitNormalWorld );
		result.fraction = cb.m_closestHitFraction;
	}
	return result;
}

bool bullet_physics_world::recover_from_penetrations(
	bt_collision_shape*		const shape,
	float4x4 const&			transform_initial,
	float4x4&				transform_result,
	u16						filter_group,
	u16						filter_mask
)
{
	btCollisionShape* bt_shape = shape->get_bt_shape( );
	btPairCachingGhostObject test_ghost_object;
	test_ghost_object.setCollisionShape( bt_shape );
	test_ghost_object.setCollisionFlags( btCollisionObject::CF_CHARACTER_OBJECT );
	test_ghost_object.setWorldTransform( from_vostok( transform_initial ) );

	m_dynamicsWorld->addCollisionObject( &test_ghost_object, filter_group, filter_mask );

	for ( s32 i = 3 ; i != 0 ; --i )
	{
		m_dynamicsWorld->getDispatcher( )->dispatchAllCollisionPairs(
			test_ghost_object.getOverlappingPairCache( ),
			m_dynamicsWorld->getDispatchInfo( ),
			m_dynamicsWorld->getDispatcher( )
		);

		btVector3 current_pos = test_ghost_object.getWorldTransform( ).getOrigin( );
		btManifoldArray manifold_array;


		for ( s32 j = 0 ; j < test_ghost_object.getOverlappingPairCache( )->getNumOverlappingPairs( ) ; ++j )
		{
			float maxPen = 0.0f;
			btVector3 touching_normal; // sushi@NOTE: Should be initialized,but isn't in target

			manifold_array.resize( 0 );

			btBroadphasePair& pair = test_ghost_object.getOverlappingPairCache( )->getOverlappingPairArray( )[j];

			if ( pair.m_algorithm )
				pair.m_algorithm->getAllContactManifolds( manifold_array );

			for ( s32 i = 0 ; i < manifold_array.size( ) ; ++i )
			{
				btPersistentManifold* manifold = manifold_array[i];
				float directionSign = manifold->getBody0( ) == &test_ghost_object ? -1.0f : 1.0f;

				for ( s32 j = 0 ; j < manifold->getNumContacts( ) ; ++j )
				{
					btManifoldPoint& contact = manifold->getContactPoint( j );
					btScalar dist = contact.getDistance( );
					if ( dist < 0.0f )
					{
						if ( dist < maxPen && contact.m_normalWorldOnB.y( ) > 0.0f ) // sushi@NOTE: They forgot direction sign?
						{
							maxPen = dist;
							touching_normal = contact.m_normalWorldOnB * directionSign;
						}
					}
				}
			}
			btVector3 delta = touching_normal * maxPen;
			float3 delta_v = from_bullet( touching_normal * maxPen ) ;

			LOG_INFO(
				"recover from %x:%x delta %.3f %.3f %.3f",
				pair.m_pProxy0->m_clientObject,
				pair.m_pProxy1->m_clientObject,
				delta_v.x,
				delta_v.y,
				delta_v.z
			);
			current_pos += delta;

		}
		btTransform newTrans = test_ghost_object.getWorldTransform( );
		newTrans.setOrigin( current_pos );
		test_ghost_object.setWorldTransform( newTrans );
	}

	transform_result = from_bullet( test_ghost_object.getWorldTransform( ) );

	m_dynamicsWorld->removeCollisionObject( &test_ghost_object );
	return true;
}

void bullet_physics_world::object_query(
	bt_collision_shape*				const shape,
	float4x4 const&					transform_from,
	float4x4 const&					transform_to,
	vectora<closest_ray_result>&	results,
	u16								filter_group,
	u16								filter_mask
)
{
	struct object_query_callback : public btCollisionWorld::ConvexResultCallback , public boost::noncopyable {
	public:
		explicit			object_query_callback	( vectora<closest_ray_result>& results, u16 const filter_group, u16 const filter_mask ) :
								m_results	( results )
		{
			m_collisionFilterGroup = filter_group;
			m_collisionFilterMask = filter_mask;
			m_modify_result_transform.setIdentity( );
		}

		virtual	float		addSingleResult			( btCollisionWorld::LocalConvexResult& convexResult, bool normalInWorldSpace ) override
		{
			closest_ray_result query_result;
			query_result.hit_point_world = from_bullet( m_modify_result_transform * convexResult.m_hitPointLocal );

			btVector3 hitNormalWorld;
			if ( normalInWorldSpace )
			{
				hitNormalWorld = convexResult.m_hitNormalLocal;
			} else
			{
				hitNormalWorld = convexResult.m_hitCollisionObject->getWorldTransform( ).getBasis( ) * convexResult.m_hitNormalLocal;
			}
			query_result.hit_normal_world = from_bullet( hitNormalWorld );

			query_result.triangle_index = convexResult.m_localShapeInfo->m_triangleIndex;
			query_result.is_shape_index = convexResult.m_localShapeInfo->m_is_shape_index;
			query_result.fraction = convexResult.m_hitFraction;

			m_results.push_back( query_result );

			return convexResult.m_hitFraction;
		}

	public:
		/* 0x000c */	vectora<closest_ray_result>&	m_results;
		/* 0x0010 */	btTransform						m_modify_result_transform;
	};

	btTransform t1 = from_vostok( transform_from );
	btTransform t2 = from_vostok( transform_to );
	object_query_callback resultCallback( results, filter_group, filter_mask );

	btCollisionShape* bt_shape = shape->get_bt_shape( );
	btConvexShape* cast_shape;

	if ( bt_shape->isConvex( ) )
	{
		cast_shape = static_cast< btConvexShape* >( bt_shape );
	} else {
		if ( bt_shape->isCompound( ) )
		{
			btCompoundShape* comp_shape = static_cast< btCompoundShape* >( bt_shape );
			cast_shape = static_cast< btConvexShape* >( comp_shape->getChildShape( 0 ) );
			btTransform& child_transform = comp_shape->getChildTransform( 0 );
			resultCallback.m_modify_result_transform = child_transform.inverse( );
			t1 = t1 * child_transform;
			t2 = t2 * child_transform;

		}
		else
		{
			LOG_ERROR( "Unsupported shape passed" );
			return;
		}
	}
	m_dynamicsWorld->convexSweepTest( cast_shape, t1, t2, resultCallback );
}

struct distance_predicate {
public:
	explicit		distance_predicate	( float3 const& from ) : m_from( from ) { }

	inline	bool	operator()			( closest_ray_result const& lhs, closest_ray_result const& rhs ) const
	{
		return ( lhs.hit_point_world - m_from ).squared_length( ) < ( rhs.hit_point_world - m_from ).squared_length( );
	}

public:
	/* 0x0000 */	float3		m_from;
}; // struct distance_predicate

STATIC_SIZE_ASSERT(distance_predicate, 0xC);

void bullet_physics_world::ray_query(
	float3 const&					ray_from,
	float3 const&					ray_dir,
	float							ray_length,
	vectora<closest_ray_result>&	results,
	u16								filter_group,
	u16								filter_mask
)
{
	btVector3 from = from_vostok( ray_from );
	btVector3 to = from_vostok( ray_from + ray_dir * ray_length );
	btCollisionWorld::AllHitsRayResultCallback cb( from, to );
	cb.m_collisionFilterGroup = filter_group;
	cb.m_collisionFilterMask = filter_mask;
	cb.m_flags |= 1 << 1;

	m_dynamicsWorld->rayTest( from, to, cb );

	if ( cb.m_collisionObject )
	{
		s32 size = cb.m_collisionObjects.size( );
		for ( s32 i = 0 ; i < size ; ++i )
		{
			closest_ray_result ray_result;
			ray_result.hit_point_world = from_bullet( cb.m_hitPointWorld[i] );
			ray_result.object = static_cast< base_physics_object* >( cb.m_collisionObjects[i]->getUserPointer( ) );
			ray_result.triangle_index = cb.m_triangleIndex[i];
			ray_result.is_shape_index = cb.m_is_shape_index[i];
			ray_result.hit_normal_world = from_bullet( cb.m_hitNormalWorld[i] );
			ray_result.fraction = cb.m_closestHitFraction;
			results.push_back( ray_result );
		}

		std::sort( results.begin( ), results.end( ), distance_predicate( ray_from ) );
	}
}

static const u8 s_convert_from_bullet_type[] = {
    0x00, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04,
    0x01, 0x04, 0x02, 0x04, 0x04, 0x03, 0xCC, 0xCC,
    0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC
};

static collision::primitive_type from_bullet_shape_type( s32 type )
{
	switch ( s_convert_from_bullet_type[ type ] )
	{
		case 0:	return collision::primitive_box;
		case 1:	return collision::primitive_sphere;
		case 3:	return collision::primitive_cylinder;
		case 2:	return collision::primitive_capsule;
		default: NODEFAULT( );
	}
}

static float3 dimensions_from_bullet_shape( btCollisionShape const* bullet_shape )
{
	switch ( s_convert_from_bullet_type[ bullet_shape->getShapeType( ) ] )
	{
		case 0:
		{
			btBoxShape const* bt_shape = static_cast< btBoxShape const* >( bullet_shape );
			float3 half_extents = from_bullet( bt_shape->getHalfExtentsWithoutMargin( ) );
			return half_extents;
		}
		case 1:
		{
			btSphereShape const* bt_shape = static_cast< btSphereShape const* >( bullet_shape );
			float3 radius( bt_shape->getRadius( ), bt_shape->getRadius( ), 0.0f );
			return radius;
		}
		case 3:
		{
			btCylinderShape const* bt_shape = static_cast< btCylinderShape const* >( bullet_shape );
			float3 half_extents = from_bullet( bt_shape->getHalfExtentsWithoutMargin( ) );
			return half_extents;
		}
		case 2:
		{
			btCapsuleShape const* bt_shape = static_cast< btCapsuleShape const* >( bullet_shape );
			float3 dimensions( bt_shape->getHalfHeight( ), bt_shape->getRadius( ), 0.0f );
			return dimensions;
		}
		default: NODEFAULT( );
	}
}

float contact_result_callback::addSingleResult(
	btManifoldPoint&			cp,
	btCollisionObject const*	colObj0,
	s32							partId0,
	s32							index0,
	btCollisionObject const*	colObj1,
	s32							partId1,
	s32							index1
)
{
	s32 shape_0_type = colObj0->getCollisionShape( )->getShapeType( );
	float4x4 shape_0_transform = from_bullet( colObj0->getWorldTransform( ) );
	float3 shape_0_dim = dimensions_from_bullet_shape( colObj0->getCollisionShape( ) );

	s32 shape_1_type = colObj1->getCollisionShape( )->getShapeType( );
	float4x4 shape_1_transform = from_bullet( colObj1->getWorldTransform( ) );
	float3 shape_1_dim = dimensions_from_bullet_shape( colObj1->getCollisionShape( ) );

	return m_predicate->add_single_result(
		colObj0->getCollisionShape( )->getUserPointer( ),
		from_bullet_shape_type( shape_0_type ),
		shape_0_transform,
		shape_0_dim,
		from_bullet_shape_type( shape_1_type ),
		shape_1_transform,
		shape_1_dim
	);
}

void bullet_physics_world::contact_pair_test( contact_test_predicate& predicate, btCollisionObject* first_object, btCollisionObject* second_object )
{
	contact_result_callback cb( &predicate );
	m_dynamicsWorld->contactPairTest( first_object, second_object, cb );
}

bool bullet_physics_world::adjust_foot_transform(
	float3 const&		half_size,
	float3 const&		start,
	float3 const&		finish,
	float				rotation_koef0,
	float				__formal,
	float4x4&			transform
)
{
	static bool s_ik_change_foot_rotation_value = false;
	static console_commands::cc_bool s_ik_change_foot_rotation_cc( "ik_change_foot_rotation", s_ik_change_foot_rotation_value, false, console_commands::command_type_engine_internal );

	btCapsuleShape collision_shape( half_size.x, half_size.y );
	btQuaternion q = from_vostok( transform ).getRotation( );
	btVector3 btStart = from_vostok( start );
	btVector3 btEnd	= from_vostok( finish );

	btCollisionWorld::ClosestConvexResultCallback callback( btStart, btEnd );
	callback.m_collisionFilterGroup = 0x24;
	callback.m_collisionFilterMask = 0x2;
	m_dynamicsWorld->convexSweepTest( &collision_shape, btTransform( q, btStart ), btTransform( q, btEnd ), callback );

	if ( callback.hasHit( ) )
	{
		if ( math::abs( callback.m_closestHitFraction ) >= math::epsilon_5 )
		{
			btVector3 result;
			result.setInterpolate3( btStart, btEnd, callback.m_closestHitFraction );

			if ( s_ik_change_foot_rotation_value )
			{ // collision normal and the object�s forward direction
				float3 normal = from_bullet( callback.m_hitNormalWorld );
				const float angle = math::acos( math::clamp_r( normal.dot_product( -transform.k.xyz( ) ), -1.0f, 1.0f ) ) * rotation_koef0;
				if ( math::abs( angle ) >= math::epsilon_5 )
				{
					float3 rotation_axis = ( -transform.k.xyz( ) ).cross_product( normal ).normalize( );
					transform = math::mul4x3( transform, math::create_rotation( rotation_axis, angle ) );
				}
			}
			transform.c.xyz( ) = from_bullet( result );
		}
	}
	return callback.hasHit( );
}

void bullet_physics_world::notify_about_contact( )
{
	const s32 num_manifolds = m_dispatcher->getNumManifolds( );
	for ( s32 i = 0; i < num_manifolds ; ++i )
	{
		btPersistentManifold* manifold = m_dispatcher->getManifoldByIndexInternal( i );
		for ( s32 j = 0 ; j < manifold->getNumContacts( ) ; ++j )
		{
			if ( 0.0f <= manifold->getContactPoint( j ).m_distance1 )
				continue;

			base_physics_object* base_obj_a = static_cast< base_physics_object* >(
				static_cast< btPairCachingGhostObject* >( manifold->getBody0( ) )->getUserPointer( )
			);
			base_physics_object* base_obj_b = static_cast< base_physics_object* >(
				static_cast< btPairCachingGhostObject* >( manifold->getBody1( ) )->getUserPointer( )
			);

			callbacks_begin_end_pair begin_end = m_contact_callbacks.equal_range( base_obj_a );
			for ( ; begin_end.first != begin_end.second ; ++begin_end.first )
				(*begin_end.first->second)( base_obj_a, base_obj_b, from_bullet( manifold->getContactPoint( j ).getPositionWorldOnA( ) ) );

			break;
		}
	}
}

void bullet_physics_world::subscribe_on_contact( base_physics_object* object, callback_type* callback )
{
	callbacks_begin_end_pair ret = m_contact_callbacks.equal_range( object );


	for ( ; ret.first != ret.second; ++ret.first )
		ASSERT( ret.first->second != callback );

	m_contact_callbacks.insert( callbacks_type::value_type( object, callback ) );
}

void bullet_physics_world::unsubscribe_from_contact( base_physics_object* object, callback_type* callback )
{
	callbacks_begin_end_pair ret = m_contact_callbacks.equal_range( object );


	callbacks_type::iterator it = m_contact_callbacks.end( );

	for ( ; ret.first != ret.second; ++ret.first )
	{
		if ( ret.first->second == callback ) { it = ret.first; break; }
	}

	ASSERT(
		it != m_contact_callbacks.end( )
	);
	m_contact_callbacks.erase( it );
}

} // namespace physics
} // namespace vostok
