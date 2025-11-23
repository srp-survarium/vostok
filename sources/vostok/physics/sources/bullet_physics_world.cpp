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
#include <vostok/physics/contact_test_predicate.h> // sushi@TODO: Should be private?

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

// STATE[STUB]
void dump_physics_profiler( pcstr __formal )
{
	CProfileManager::dumpAll( );
}

// STATE[STUB]
void reset_physics_profiler( pcstr __formal )
{
	CProfileManager::Reset();
}

// sushi@NOTE: static name conflict. Could be renamed in the future.
static console_commands::cc_delegate s_dump_statistics (
	"dump_physics_profiler", &dump_physics_profiler, false
);

static console_commands::cc_delegate s_reset_statistics (
	"reset_physics_profiler", &reset_physics_profiler, false
);


// STATE[STUB]
void* bullet_alloc( u32 size )
{
	return VOSTOK_MALLOC_IMPL( vostok::physics::g_ph_allocator, size, "bullet" );	// <0x6bc9e0>|0x000|0x000:'45'
}

// STATE[STUB]
void bullet_free( void* memblock )
{
	return VOSTOK_FREE_IMPL( vostok::physics::g_ph_allocator, memblock );	// <0x6bc9c0>|0x000|0x000:'50'
}

namespace vostok {
namespace physics {

// STATE[100%|DONE]
btTransform from_vostok( float4x4 const& m )
{
	math::quaternion q_vostok = math::quaternion(m);	// <0x6bd789>|0x000|0x000:'58'
	btQuaternion q0		= from_vostok(q_vostok);		// <0x6bd794>|0x00b|0x00b:'59'
	return btTransform(q0, from_vostok(m.c.xyz()) );	// <0x6bd7a2>|0x019|0x00e:'60'
}

// STATE[100%|DONE]
float4x4 from_bullet( btTransform const& m )
{
	btQuaternion q_bullet	= m.getRotation();												// <0x6bd6df>|0x000|0x000:'65'
	math::quaternion q_vostok = from_bullet( q_bullet );									// <0x6bd6e9>|0x00a|0x00a:'66'
	return create_rotation(q_vostok) * create_translation( from_bullet(m.getOrigin()) );	// <0x6bd6f7>|0x018|0x00e:'67'
}

// STATE[64%|STUB]
bullet_physics_world::bullet_physics_world( memory::base_allocator& allocator, engine& engine ):
	m_allocator		( allocator ),
	m_engine		( engine ),
	m_world_aabb	( math::create_aabb_min_max(
		float3( math::infinity, math::infinity, math::infinity ),
		float3( math::infinity, math::infinity, math::infinity )
	))
{
}

// STATE[94%|PARTIAL]: Best function to fix logging based on
void log_cb( char* text )
{
	LOG_INFO( text );
}

// STATE[99.35%|DONE]
void bullet_physics_world::initialize( )
{
	btAlignedAllocSetCustom		( bullet_alloc, bullet_free );

	btVector3 worldMin			(-1000,-1000,-1000);
	btVector3 worldMax			(1000,1000,1000);

	//SoftDicsreteDynamicWorld
	m_softBodyWorldInfo						= VOSTOK_NEW_IMPL( m_allocator, btSoftBodyWorldInfo );
	m_softBodyWorldInfo->air_density		= (btScalar)1.2; // <0x6bf1d0>|0x0f6|0x0b0:'92'
	m_softBodyWorldInfo->water_density		= 0;
	m_softBodyWorldInfo->water_offset		= 0;
	m_softBodyWorldInfo->water_normal		= btVector3(0,0,0);
	m_softBodyWorldInfo->m_gravity.setValue	(0,-10,0); // <0x6bf217>|0x13d|0x033:'96'

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

	m_dynamicsWorld->getDispatchInfo().m_enableSPU = false;//true; //? // <0x6bf331>|0x257|0x02f:'114'
	m_dynamicsWorld->setGravity(btVector3(0,-10,0));
	m_softBodyWorldInfo->m_gravity.setValue(0,-10,0);
	m_softBodyWorldInfo->m_sparsesdf.Initialize();
	m_softBodyWorldInfo->m_sparsesdf.Reset();

	m_ghost_pair_callback		= VOSTOK_NEW_IMPL( m_allocator, btGhostPairCallback );
	m_dynamicsWorld->getBroadphase( )->getOverlappingPairCache( )->setInternalGhostPairCallback( m_ghost_pair_callback );

	m_last_frame_delta = 0.0f; // sushi@TODO: Maybe on_before_reuse
	m_last_frame_time = 0.0f;
	CProfileManager::set_log_callback( log_cb );
}

// STATE[99.86%|DONE]: Target used registers slightly differently.
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

// STATE[100%|DONE]
void bullet_physics_world::set_renderer( btIDebugDraw* const renderer )
{
	m_dynamicsWorld->setDebugDrawer( renderer );
}

// STATE[100%|DONE]
void bullet_physics_world::draw_object( btCollisionShape* const shape, btTransform const& transform, btVector3 const& color )
{
	m_dynamicsWorld->debugDrawObject( transform, shape, color );	// <0x6bc880>|0x000|0x000:'152'
}

// STATE[100%|DONE]
void bullet_physics_world::on_before_reuse( )
{
	m_last_frame_delta = 0.0f; // <0x6bc870>|0x000|0x000:'157'
	m_last_frame_time = 0.0f;
}

// STATE[100%|DONE]
void bullet_physics_world::tick( u32 current_time_in_ms )
{
	m_last_frame_delta =
		( current_time_in_ms * math::epsilon_3 - m_last_frame_time ) * 0.1f
		+ m_last_frame_delta * 0.9f;	// <0x6bde70>|0x000|0x000:'164'
	m_last_frame_time = current_time_in_ms * math::epsilon_3;

	s32 max_substeps = math::floor( m_last_frame_delta * 60.0f + 0.5f );
	if ( max_substeps > s_physics_max_substeps_value )
		max_substeps = 1;

	m_dynamicsWorld->stepSimulation( m_last_frame_delta, max_substeps );

	notify_about_contact( );

	m_softBodyWorldInfo->m_sparsesdf.GarbageCollect( );
}

// STATE[STUB] sushi@TODO: Shouldn't be needed for server logic.
void bullet_physics_world::debug_draw_world( )
{
	NOT_IMPLEMENTED( );
}

// STATE[100%|DONE]
void bullet_physics_world::add( bt_rigid_body_base* body, u16 filter_group, u16 filter_mask )
{
	m_dynamicsWorld->addRigidBody( body->get_rigid_body( ), filter_group, filter_mask);	// <0x6bd18b>|0x000|0x000:'249'

	btTransform trans = body->get_rigid_body( )->getWorldTransform( );					// <0x6bd1b5>|0x02a|0x02a:'251'

	btVector3 minAabb;
	btVector3 maxAabb;
	body->get_rigid_body( )->getCollisionShape( )->getAabb( trans, minAabb, maxAabb );	// <0x6bd213>|0x088|0x05e:'255'
	m_world_aabb.modify( from_bullet( minAabb ) );										// <0x6bd23e>|0x0b3|0x02b:'256'
	m_world_aabb.modify( from_bullet( maxAabb ) );										// <0x6bd277>|0x0ec|0x039:'257'
}

// STATE[100%|DONE]
void bullet_physics_world::remove( bt_rigid_body_base* body )
{
	m_dynamicsWorld->removeRigidBody( body->get_rigid_body( ) );
}

// STATE[100%|DONE]
void bullet_physics_world::move( bt_rigid_body_base* body, float4x4 const& new_transform )
{
	body->set_transform( new_transform );
}

// STATE[100%|DONE]: sushi@NOTE: Why filter_group with filter_mask is not passed here.
void bullet_physics_world::add( bt_soft_body_rope* body )
{
	m_dynamicsWorld->addSoftBody( body->m_bt_body ); // <0x6bd160>|0x000|0x000:'272'
}

// STATE[100%|DONE]
void bullet_physics_world::remove( bt_soft_body_rope* body )
{
	m_dynamicsWorld->removeSoftBody( body->m_bt_body ); // <0x6bcde0>|0x000|0x000:'277'
}

// STATE[100%|DONE]
void bullet_physics_world::add( bt_constraint* constraint )
{
	m_dynamicsWorld->addConstraint( constraint->m_bt_typed_constraint ); // <0x6bc800>|0x000|0x000:'282'
}

// STATE[100%|DONE]
void bullet_physics_world::remove( bt_constraint* constraint )
{
	m_dynamicsWorld->removeConstraint( constraint->m_bt_typed_constraint );
}

// STATE[100%|DONE]
void bullet_physics_world::create_test_scene( )
{
	// <1> this is ifdefed original xray impl
	// <26>
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

// STATE[STUB]
closest_ray_result_callback::closest_ray_result_callback( btVector3 const& rayFromWorld, btVector3 const& rayToWorld ) :
	m_rayFromWorld		( rayFromWorld ),
	m_rayToWorld		( rayToWorld ),
	m_triangleIndex		( -1 ),
	m_is_shape_index	( false )
{
}

// STATE[STUB]
float closest_ray_result_callback::addSingleResult( btCollisionWorld::LocalRayResult& rayResult, bool normalInWorldSpace )
{
	m_closestHitFraction = rayResult.m_hitFraction;
	m_collisionObject = rayResult.m_collisionObject;

	if ( rayResult.m_localShapeInfo )
	{
		m_triangleIndex = rayResult.m_localShapeInfo->m_triangleIndex;
		m_is_shape_index = rayResult.m_localShapeInfo->m_is_shape_index; // sushi@TODO: Bullet impl was changed, we need to match it as well
	} else
	{
		m_triangleIndex = -1;
		m_is_shape_index = false;	// <0x36418>|0x032|0x007:'354'
	}

	if ( normalInWorldSpace )		// <0x3641c>|0x036|0x004:'357'
	{
		m_hitNormalWorld = rayResult.m_hitNormalLocal;
	} else
	{
		m_hitNormalWorld = m_collisionObject->getWorldTransform( ) * rayResult.m_hitNormalLocal;
	}

	m_hitPointWorld.setInterpolate3( m_rayFromWorld, m_rayToWorld, rayResult.m_hitFraction );
	return rayResult.m_hitFraction;
}

// STATE[79.93%|PARTIAL]: sushi@NOTE: Lots of instructions reordered. Logic seems to be the same. Don't really care about getting this 100% correct for now.
closest_ray_result bullet_physics_world::ray_test(
	float3 const&		ray_from,
	float3 const&		ray_dir,
	float				ray_length,
	u16					filter_group,
	u16					filter_mask
)
{
	btVector3 from = from_vostok( ray_from );
	btVector3 to = from_vostok( ray_from + ray_dir * ray_length );

	closest_ray_result_callback cb( from, to );
	cb.m_collisionFilterGroup = filter_group;
	cb.m_collisionFilterMask = filter_mask;
	cb.m_flags |= 1 << 1;

	m_dynamicsWorld->rayTest( from, to, cb );

	closest_ray_result result;

	if ( cb.m_collisionObject )
	{
		result.object = static_cast< base_physics_object* >( cb.m_collisionObject->getUserPointer( ) );
		result.hit_point_world = from_bullet( cb.m_hitPointWorld );
		result.hit_normal_world = from_bullet( cb.m_hitNormalWorld );
		result.triangle_index = cb.m_triangleIndex;
		result.is_shape_index = cb.m_is_shape_index;
		result.fraction = cb.m_closestHitFraction;
	}
	return result;
}

// STATE[STUB]
bool bullet_physics_world::recover_from_penetrations(
	bt_collision_shape*		const shape,
	float4x4 const&			transform_initial,
	float4x4&				transform_result,
	u16						filter_group,
	u16						filter_mask
)
{
	// LOCALS
	// btPairCachingGhostObject 	test_ghost_object
	// btVector3 					current_pos
	// btAlignedObjectArray<btPersistentManifold *> manifold_array
	// s32 							i
	// btVector3 					touching_normal
	// btVector3 					delta
	// float 						maxPen
	// float3 						delta_v
	// s32 							j
	// ******

	// CALL SITE INFO
	// <0x6be98e> -> void <unknown>(btCollisionObject*, short, short)
	// <0x6be9b0> -> void <unknown>(btOverlappingPairCache*, btDispatcherInfo const&, btDispatcher*)
	// <0x6be9e7> -> int <unknown>() const
	// <0x6bea66> -> btAlignedObjectArray<btBroadphasePair>& <unknown>()
	// <0x6bea82> -> void <unknown>(btAlignedObjectArray<btPersistentManifold *>&)
	// <0x6beed1> -> int <unknown>() const
	// <0x6bef82> -> void <unknown>(btCollisionObject*)
	// ******

	return false;
	// FUNCTION BODY
	// <0x6be91c>|0x000|0x000:'399'
	// <1>
	// <0x6be92a>|0x00e|0x00e:'401'
	// <1>
	// <2>
	// <3>
	// <4>
	// <0x6be942>|0x026|0x018:'406'
	// <1>
	// <0x6be976>|0x05a|0x034:'408'
	// <1>
	// <0x6be990>|0x074|0x01a:'410'
	// <1>
	// <2>
	// <3>
	// <0x6be998>|0x07c|0x008:'414'
	// <1>
	// <2>
	// <3>
	// <4>
	// <0x6be9b2>|0x096|0x01a:'419'
	// <1>
	// <2>
	// <3>
	// <0x6be9f5>|0x0d9|0x043:'423'
	// <1>
	// <0x6be9f1>|0x0d5|-0x004:'425'
	// <1>
	// <0x6bea6f>|0x153|0x07e:'427'
	// <0x6bea75>|0x159|0x006:'428'
	// <1>
	// <2>
	// <0x6bea84>|0x168|0x00f:'431'
	// <1>
	// <0x6beaad>|0x191|0x029:'433'
	// <0x6beab4>|0x198|0x007:'434'
	// <1>
	// <0x6bead1>|0x1b5|0x01d:'436'
	// <1>
	// <2>
	// <3>
	// <0x6beaf0>|0x1d4|0x01f:'440'
	// <1>
	// <0x6beaf5>|0x1d9|0x005:'442'
	// <1>
	// <0x6beafa>|0x1de|0x005:'444'
	// <1>
	// <2>
	// <0x6beb09>|0x1ed|0x00f:'447'
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <7>
	// <8>
	// <0x6becfd>|0x3e1|0x1f4:'456'
	// <1>
	// <2>
	// <3>
	// <0x6bed03>|0x3e7|0x006:'460'
	// <0x6bee6f>|0x553|0x16c:'461'
	// <1>
	// <2>
	// <0x6beee1>|0x5c5|0x072:'464'
	// <1>
	// <2>
	// <0x6beeea>|0x5ce|0x009:'467'
	// <1>
	// <0x6bef53>|0x637|0x069:'469'
	// <1>
	// <0x6bef72>|0x656|0x01f:'471'
	// <0x6bef84>|0x668|0x012:'472'
	// ******
}

// STATE[STUB]
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
		// STATE[STUB]
		explicit			object_query_callback	( vectora<closest_ray_result>& results, u16 filter_group, u16 filter_mask ) :
								m_results	( results )
		{
			m_collisionFilterGroup = filter_group;
			m_collisionFilterMask = filter_mask;
			m_modify_result_transform.setIdentity( ); // <0x6bd3b0>|0x000|0x000:'488'
		}

		// STATE[STUB]: sushi@TODO: Ghidra scripts cannot generate symbols for this function.
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
				hitNormalWorld = convexResult.m_hitCollisionObject->getWorldTransform( ) * convexResult.m_hitNormalLocal;
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

	btConvexShape* cast_shape;

	if ( shape->get_bt_shape( )->isConvex( ) )
	{
		cast_shape = static_cast< btConvexShape* >( shape->get_bt_shape( ) );
	} else {
		if ( shape->get_bt_shape( )->isCompound( ) )
		{
			// sushi@TODO
		}
		else
		{
			LOG_ERROR( "Unsupported shape passed" );
			return;
		}
	}
	m_dynamicsWorld->convexSweepTest( cast_shape, t1, t2, resultCallback );

	// FUNCTION BODY
	// <1>
	// <..>
	// <35>
	// <0x6bdf1e>|0x000|0x000:'517' btTransform t1 = from_vostok( transform_from );
	// <0x6bdf3b>|0x01d|0x01d:'518' btTransform t2 = from_vostok( transform_to );
	// <0x6bdf47>|0x029|0x00c:'519' object_query_callback resultCallback( results, filter_group, filter_mask );
	// <1>
	// <0x6bdf64>|0x046|0x01d:'521'
	// <1>
	// <0x6be021>|0x103|0x0bd:'523' if ( shape->get_bt_shape( )->isConvex( ) )
	// <1>							{
	// <0x6be029>|0x10b|0x008:'525'
	// <0x6be02b>|0x10d|0x002:'526'	} else {
	// <0x6be030>|0x112|0x005:'527'		if ( shape->get_bt_shape( )->isCompound( ) )
	// <1>								{
	// <2>
	// <0x6be039>|0x11b|0x009:'530'
	// <1>
	// <2>
	// <0x6be03f>|0x121|0x006:'533'
	// <1>
	// <0x6be051>|0x133|0x012:'535'
	// <0x6be443>|0x525|0x3f2:'536'
	// <1>
	// <2>								} else
	// <3>								{
	// <0x6be864>|0x946|0x421:'540'			LOG_ERROR( "Unsupported shape passed" );
	// <1>								}
	// <2>							}
	// <3>
	// <0x6be82f>|0x911|-0x035:'544'
	// ******
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

// STATE[100%|DONE]: sushi@TODO: Noticed that `m_shape_id` was added to bt ray callback
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

	m_dynamicsWorld->rayTest( from, to, cb ); // <0x6bd503>|0x0b2|0x010:'569'

	if ( cb.m_collisionObject )
	{
		s32 size = cb.m_collisionObjects.size( ); // <0x6bd531>|0x0e0|0x00e:'573'
		for ( s32 i = 0 ; i < size ; ++i )
		{
			closest_ray_result ray_result; // sushi@NOTE: One day I will reorder those assignments to the declaration order.
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

//
// sushi@NOTE: All of this is extremely confusing.
// All functions actually have different static consts,
//		which are also unnamed (compiler generated?).
// All functions expect only 4 cases, while this table has 5
//		(6, if you count 0xCC, which is its own share of problems).
// Box, Sphere and Cylinder actually matched properly.
// 0x04 is confusing since it matches many different types.
// 0xCC is even more confusing, since there are 36 types,
//		while they cover only 24.
//

static const u8 s_convert_from_bullet_type[] = {
    0x00, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04,
    0x01, 0x04, 0x02, 0x04, 0x04, 0x03, 0xCC, 0xCC,
    0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC
};

// STATE[STUB] Incorrect generation by Ghidra scripts
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

// STATE[STUB] Incorrect generation by Ghidra scripts.
// Possibly has `from_bullet_shape_type` call inlined?
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

// STATE[42.88%|STUB]: Problems in Ghidra scripts, `from_bullet_shape_type` got inlined also
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
	); // <0x365d9>|0x05c|0x00f:'723'
}

// STATE[STUB]
void bullet_physics_world::contact_pair_test( contact_test_predicate& predicate, btCollisionObject* first_object, btCollisionObject* second_object )
{
	contact_result_callback cb( &predicate );	// <0x6bd373>|0x000|0x000:'732'
	m_dynamicsWorld->contactPairTest( first_object, second_object, cb );
}

// STATE[STUB]
// bool vostok::physics::bullet_physics_world::adjust_foot_transform(vostok::math::float3 const&, vostok::math::float3 const&, vostok::math::float3 const&, float, float, vostok::math::float4x4&)
bool bullet_physics_world::adjust_foot_transform(
	float3 const&		half_size,
	float3 const&		start,
	float3 const&		finish,
	float				rotation_koef0,
	float				__formal,
	float4x4&			transform
)
{
	// LOCALS
	// btCollisionWorld::ClosestConvexResultCallback callback
	// btCapsuleShape 				collision_shape
	// btVector3 					result
	// float3 						normal
	// float 						angle
	// float3 						rotation_axis
	// ******

	// STATICS
	// static bool 					s_ik_change_foot_rotation_value = <0x4c25f0b>;
	// static console_commands::cc_bool s_ik_change_foot_rotation_cc = <0x4c2b0e0>;
	// ******

	return false;
	// FUNCTION BODY
	// <1>
	// <0x6bd80c>|0x000|0x000:'784'
	// <1>
	// <0x6bd85c>|0x050|0x050:'786'
	// <0x6bd886>|0x07a|0x02a:'787'
	// <0x6bd8f7>|0x0eb|0x071:'788'
	// <0x6bd928>|0x11c|0x031:'789'
	// <1>
	// <0x6bd94b>|0x13f|0x023:'791'
	// <1>
	// <0x6bd968>|0x15c|0x01d:'793'
	// <0x6bd98f>|0x183|0x027:'794'
	// <1>
	// <0x6bda23>|0x217|0x094:'796'
	// <1>
	// <0x6bda3d>|0x231|0x01a:'798'
	// <1>
	// <2>
	// <0x6bda64>|0x258|0x027:'801'
	// <0x6bda70>|0x264|0x00c:'802'
	// <0x6bdae2>|0x2d6|0x072:'803'
	// <1>
	// <0x6bdafb>|0x2ef|0x019:'805'
	// <0x6bdb0c>|0x300|0x011:'806'
	// <0x6bdb45>|0x339|0x039:'807'
	// <0x6bdb73>|0x367|0x02e:'808'
	// <0x6bdb83>|0x377|0x010:'809'
	// <1>
	// <0x6bdbae>|0x3a2|0x02b:'811'
	// <0x6bdc2f>|0x423|0x081:'812'
	// <0x6bdc46>|0x43a|0x017:'813'
	// <1>
	// <2>
	// <0x6bdc87>|0x47b|0x041:'816'
	// <1>
	// <2>
	// <0x6bdcb8>|0x4ac|0x031:'819'
	// <1>
	// <2>
	// ******
}

// STATE[95.83%|PARTIAL]: Functionally complete. See comments inside the function for diffs.
void bullet_physics_world::notify_about_contact( )
{
	s32 num_manifold = m_dispatcher->getNumManifolds( );											// <0x6bdced>|0x000|0x000:'826'
	for ( s32 i = 0; i < num_manifold ; ++i )
	{
		btPersistentManifold* manifold = m_dispatcher->getManifoldByIndexInternal( i );
		s32 num_contacts = manifold->getNumContacts( );
		for ( s32 j = 0 ; j < num_contacts ; ++j )													// <0x6bdd24>|0x037|0x006:'831'
		{
			btManifoldPoint& pt = manifold->getContactPoint( j );
			if ( 0.0f <= pt.m_distance1 )															// <0x6bdd34>|0x047|0x010:'834' sushi@NOTE: comiss reversed
				continue;

			base_physics_object* base_obj_a = static_cast< base_physics_object* >(
				static_cast< btPairCachingGhostObject* >( manifold->getBody0( ) )->getUserPointer( )
			);																						// <0x6bdd4a>|0x05d|0x016:'839'
			base_physics_object* base_obj_b = static_cast< base_physics_object* >(
				static_cast< btPairCachingGhostObject* >( manifold->getBody1( ) )->getUserPointer( )
			);																						// <0x6bdd50>|0x063|0x006:'840'

			callbacks_begin_end_pair begin_end	= m_contact_callbacks.equal_range( base_obj_a );
			callbacks_type::iterator it			= begin_end.first;
			callbacks_type::iterator it_end		= begin_end.second;
			for ( ; it != it_end; ++it )
			{
				(*it->second)( base_obj_a, base_obj_b, from_bullet( pt.getPositionWorldOnA( ) ) );	// <0x6bddd4>|0x0e7|0x021:'847' sushi@NOTE: slightly different asm order
			}

			break;
		}
	}
}

// STATE[88.74%|PARTIAL]: Seems like LTCG related issues, but couldn't match the structure also.
void bullet_physics_world::subscribe_on_contact( base_physics_object* object, callback_type* callback )
{
	ASSERT( callback );
	callbacks_begin_end_pair ret	= m_contact_callbacks.equal_range( object );
	callbacks_type::iterator it		= ret.first;
	callbacks_type::iterator it_end	= ret.second;

	for ( ; it != it_end; ++it )
		ASSERT( it->second != callback );

	m_contact_callbacks.insert( callbacks_type::value_type(object, callback) );	// <0x6bcac2>|0x037|0x025:'864'
}

// STATE[77.04%|PARTIAL]: Possibly can be matched further, but I don't think it matters much.
void bullet_physics_world::unsubscribe_from_contact( base_physics_object* object, callback_type* callback )
{
	ASSERT( callback );
	callbacks_begin_end_pair ret	= m_contact_callbacks.equal_range( object );

	callbacks_type::iterator it		= ret.first;
	callbacks_type::iterator it_end	= ret.second;
	for ( ; it != it_end; ++it )
		if ( it->second == callback )
			break;

	ASSERT( it != it_end );

	m_contact_callbacks.erase( it ); // <0x6bca4f>|0x043|0x002:'883'
}

} // namespace physics
} // namespace vostok