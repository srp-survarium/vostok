////////////////////////////////////////////////////////////////////////////
//	Created 	: 05.10.2025
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "bullet_character_controller.h"

#include "bullet_include.h"
#include "LinearMath/btQuickProf.h"
#include <vostok/console_command.h>

namespace vostok {
namespace physics {

static float s_step_height								= 0.6f;
static console_commands::cc_float s_step_height_command						( "character_controller_step_height", s_step_height, 0.0f, 2.0f, true, console_commands::command_type_engine_internal );

static float s_character_sliping_speed_multiplier_value = 20.0f;
static console_commands::cc_float s_character_sliping_speed_multiplier_cc	( "cc_sliping_speed_multiplier", s_character_sliping_speed_multiplier_value, 0.01f, 100.0f, true, console_commands::command_type_engine_internal );

static float s_cc_max_allowed_penetration_value			= 0.04f;
static console_commands::cc_float s_cc_max_allowed_penetration_cc			( "cc_max_allowed_penetration", s_cc_max_allowed_penetration_value, 0.0f, 1.0f, false, console_commands::command_type_engine_internal );

static bool  s_cc_prevent_step_bouncing_value			= true;
static console_commands::cc_bool  s_cc_prevent_step_bouncing_cc				( "cc_prevent_step_bouncing", s_cc_prevent_step_bouncing_value, false, console_commands::command_type_engine_internal );

static u32 s_cc_smooth_positions_count_value			= 3;
static console_commands::cc_u32 s_cc_smooth_positions_count_cc				( "cc_smooth_positions_count", s_cc_smooth_positions_count_value, 1, 100, true, console_commands::command_type_engine_internal );


u16 const*	g_game_material_groups;
s32			g_game_materials_count;

static bool	logging	= false;

const btVector3 bullet_character_controller::m_up_vector = btVector3( 0.0f, 1.0f, 0.0f );

static btVector3 getNormalizedVector( btVector3 const& v )
{
	btVector3 result = v.normalized( );
	if ( result.length( ) < SIMD_EPSILON )
		result.setZero( );
	return result;
}

btVector3 computeReflectionDirection( btVector3 const& direction, btVector3 const& normal )
{
	return direction - 2 * normal.dot( direction ) * normal;
}

btVector3 parallelComponent( btVector3 const& direction, btVector3 const& normal )
{
	return direction.dot( normal ) * normal;
}

btVector3 perpindicularComponent( btVector3 const& direction, btVector3 const& normal )
{
	return direction - normal.dot( direction ) * normal;
}

void setup_game_material_groups( u16 const* game_material_groups, u16 game_materials_count )
{
	g_game_material_groups = game_material_groups;
	g_game_materials_count = game_materials_count;
}

class character_move_test_callback : public btCollisionWorld::ClosestConvexResultCallback , public boost::noncopyable {
public:
						character_move_test_callback	( btCollisionObject* self, btVector3 const& up_vector, const float minSlopeDot ) :
							ClosestConvexResultCallback	( btVector3( 0.0f, 0.0f, 0.0f ), btVector3( 0.0f, 0.0f, 0.0f ) ),
							m_up_vector					( up_vector ),
							m_self						( self ),
							m_minSlopeDot				( minSlopeDot ) {}

	virtual	float		addSingleResult					( btCollisionWorld::LocalConvexResult& convexResult, bool normalInWorldSpace ) override
	{
		if ( convexResult.m_hitCollisionObject == m_self )
			return 2.0f;

		btVector3 hitNormalWorld;
		if (normalInWorldSpace)
		{
			hitNormalWorld = convexResult.m_hitNormalLocal;
		} else
		{
			///need to transform normal into worldspace
			hitNormalWorld = convexResult.m_hitCollisionObject->getWorldTransform().getBasis()*convexResult.m_hitNormalLocal;
		}

		btScalar dotUp = m_up_vector.dot(hitNormalWorld);
		if (dotUp < m_minSlopeDot) {
			return btScalar(0.0);
		}
		return ClosestConvexResultCallback::addSingleResult (convexResult, normalInWorldSpace);
	}


private:
	/* 0x0000 */	/* btCollisionWorld::ClosestConvexResultCallback */
	/* 0x0060 */	/* boost::noncopyable */
	/* 0x0060 */	const btVector3			m_up_vector;
	/* 0x0070 */	btCollisionObject*		m_self;
	/* 0x0074 */	const float				m_minSlopeDot;
}; // class character_move_test_callback

STATIC_SIZE_ASSERT(character_move_test_callback, 0x80);

bullet_character_controller::bullet_character_controller(
	btPairCachingGhostObject*	ghost_object,
	float2 const&				stand_shape_dim,
	float2 const&				crouch_shape_dim,
	s16							collisionFilterGroup,
	s16							collisionFilterMask
) :
	m_collision_world			( NULL ),
	m_walk_vector				( btVector3( 0.0f, 0.0f, 0.0f ) ),
	m_normalizedDirection		( btVector3( 0.0f, 0.0f, 0.0f ) ),
	m_current_pos				( btVector3( 0.0f, 0.0f, 0.0f ) ),
	m_pre_step_position			( btVector3( 0.0f, 0.0f, 0.0f ) ),
	m_current_step_offset		( 0.0f ),
	m_current_shape_dim			( float2( math::SNaN, math::SNaN ) ),
	m_stand_shape_dim			( stand_shape_dim ),
	m_crouch_shape_dim			( crouch_shape_dim ),
	m_ghost_object				( ghost_object ),

	m_shape						( btCapsuleShape( 1.0f, 1.0f ) ),
	m_vertical_velocity			( 0.0f ),
	m_max_fall_speed			( 55.0f ),
	m_jump_speed				( 10.0f ),
	m_in_crouch					( false ),
	m_collision_filter_group	( collisionFilterGroup ),
	m_collision_filter_mask		( collisionFilterMask ),
	m_max_slope_in_radians		( math::pi_d3 ),
	m_max_slope_angle_cos		( cosf( math::pi_d3 ) ),
	m_gravity					( 29.4f ),
	m_was_on_ground				( false ),
	m_jumping					( false ),
	m_useGhostObjectSweepTest	( true ),
	m_walk_vector_applied		( false ),
	m_on_steep_slope			( false ),
	m_has_updates				( false ),
	m_positions					( )
{
	setup_crouch_state( false );
}

bullet_character_controller::~bullet_character_controller( )
{
}

btVector3 bullet_character_controller::updateTargetPositionBasedOnCollision(
	btVector3 const&	hitNormal,
	btVector3 const&	target_pos,
	float				tangentMag, // unused
	float				normalMag
)
{
	BT_PROFILE("updateTargetPositionBasedOnCollision");

	btVector3 result = m_current_pos;
	btVector3 movementDirection = target_pos - m_current_pos;

	float movement_length = movementDirection.length( );
	if ( movement_length > FLT_EPSILON )
	{
		movementDirection.normalize( );

		btVector3 reflectDir = computeReflectionDirection( movementDirection, hitNormal );
		reflectDir.normalize( );

		btVector3 perpindicularDir  = perpindicularComponent( reflectDir, hitNormal );

		{
			btVector3 perpComponent = perpindicularDir * btScalar ( normalMag * movement_length );
			result += perpComponent;
		}
	}
	return result;
}

void bullet_character_controller::updateAction( btCollisionWorld* collisionWorld, float deltaTime )
{
	BT_PROFILE("updateAction1");
	if ( logging )
		LOG_INFO( "updateAction Start  %f %f %f", m_current_pos.x( ), m_current_pos.y( ), m_current_pos.z( ) );

	pre_step( deltaTime );
	if ( logging )
		LOG_INFO( "updateAction Middle %f %f %f", m_current_pos.x( ), m_current_pos.y( ), m_current_pos.z( ) );

	player_step( deltaTime );
	if ( logging )
		LOG_INFO( "updateAction Finish %f %f %f", m_current_pos.x( ), m_current_pos.y( ), m_current_pos.z( ) );

	m_walk_vector.setZero( );
}

void bullet_character_controller::player_step( float dt )
{
	BT_PROFILE("player_step");

	m_has_updates = true;
	m_was_on_ground = on_ground( );

	if ( m_jumping )
		m_vertical_velocity = m_walk_vector.y( ) / dt;
	else
	{
		float fall_speed = m_vertical_velocity - m_gravity * dt;
		m_vertical_velocity = math::clamp_r( fall_speed, -m_max_fall_speed, m_jump_speed );
	}

	btVector3 step_up_correction;
	step_up_correction.setZero( );

	if ( !m_jumping )
		step_up( true, step_up_correction );

	if ( !m_walk_vector_applied ) {
		step_forward_and_strafe( m_walk_vector );
		m_walk_vector_applied = true;
	}

	if ( !m_jumping )
		step_down( dt, true, step_up_correction );

	btTransform new_transform = m_ghost_object->getWorldTransform( );
	new_transform.setOrigin( m_current_pos );
	m_ghost_object->setWorldTransform( new_transform );
}

float bullet_character_controller::recover_from_penetration( )
{
	BT_PROFILE("recover_from_penetration");
	m_collision_world->getDispatcher( )->dispatchAllCollisionPairs(
		m_ghost_object->getOverlappingPairCache( ),
		m_collision_world->getDispatchInfo( ),
		m_collision_world->getDispatcher( )
	);

	m_current_pos = m_ghost_object->getWorldTransform( ).getOrigin( );

	float maxPen = 0.0f;
	float shape_y = math::abs( m_shape_offset.y( ) );	btManifoldArray manifold_array;

	for ( s32 i = 0 ; i < m_ghost_object->getOverlappingPairCache( )->getNumOverlappingPairs( ) ; ++i )
	{
		manifold_array.resize( 0 );
		btBroadphasePair& pair = m_ghost_object->getOverlappingPairCache( )->getOverlappingPairArray( )[i];

		if ( pair.m_algorithm )
			pair.m_algorithm->getAllContactManifolds( manifold_array );

		for ( s32 j = 0 ; j < manifold_array.size( ) ; ++j )
		{
			btPersistentManifold* manifold = manifold_array[j];
			bool isFirstBody = manifold->getBody0( ) == m_ghost_object;
			float directionSign = isFirstBody ? -1.0f : 1.0f;

			for ( s32 k = 0 ; k < manifold->getNumContacts( ) ; ++k )
			{
				btManifoldPoint& contact = manifold->getContactPoint( k );

				btScalar dist = contact.getDistance( );

				if ( dist < 0.0f )
				{
					if ( dist < maxPen )
					{
						maxPen = dist;
						btVector3 pos_on_shape = isFirstBody ? contact.m_localPointA : contact.m_localPointB;

                        float pos_on_shape_y = ( shape_y - math::abs( pos_on_shape.y( ) ) ) / shape_y;
						float weight = pos_on_shape_y * pos_on_shape_y * pos_on_shape_y;

						btVector3 displacement = contact.m_normalWorldOnB * directionSign * dist;
						btVector3 weighedDisplacement = displacement * btVector3( weight, 1.0f - weight, weight );
						m_current_pos += weighedDisplacement;
					}
				}
			}
		}
	}

	btTransform newTrans = m_ghost_object->getWorldTransform( );
	newTrans.setOrigin( m_current_pos );
	m_ghost_object->setWorldTransform( newTrans );
	return math::abs( maxPen );
}

void bullet_character_controller::step_up( bool change_shape_size, btVector3& pos_up_correction )
{
	float new_cylinder_height = math::max( 0.0f, ( m_current_shape_dim.y - m_current_shape_dim.x ) - s_step_height );

	float new_full_height = m_current_shape_dim.x + new_cylinder_height;
	setup_shape_dim( float2( m_current_shape_dim.x, new_full_height ) );

	pos_up_correction.setValue( 0.0f, ( m_current_shape_dim.y - new_full_height ) * 0.5f, 0.0f );

    m_current_pos += pos_up_correction;
	m_current_step_offset = pos_up_correction.y( );

}

void bullet_character_controller::step_forward_and_strafe( btVector3 const& walkMove )
{
	BT_PROFILE("step_forward_and_strafe");

	btTransform start, end;
	btVector3 target_pos = m_current_pos + walkMove;

	start.setIdentity ();
	end.setIdentity ();

	btScalar fraction = 1.0;
	btScalar distance2 = (m_current_pos-target_pos).length2();

	if ( distance2 < FLT_EPSILON )
		return;

	int maxIter = 10;

	while (fraction > btScalar(0.01) && maxIter-- > 0)
	{
		start.setOrigin (m_current_pos);
		end.setOrigin (target_pos);
		btVector3 sweepDirNegative(m_current_pos - target_pos);

		character_move_test_callback callback( m_ghost_object, sweepDirNegative, 0.0 );
		callback.m_collisionFilterGroup = m_collision_filter_group;
		callback.m_collisionFilterMask = m_collision_filter_mask;

		if ( m_useGhostObjectSweepTest )
		{
			m_ghost_object->convexSweepTest (&m_shape, start, end, callback, s_cc_max_allowed_penetration_value );
		} else
		{
			m_collision_world->convexSweepTest (&m_shape, start, end, callback, s_cc_max_allowed_penetration_value );
		}

		fraction -= callback.m_closestHitFraction;

		if ( callback.hasHit( ) )
		{
			// we moved only a fraction
			btScalar hitDistance;
			hitDistance = (callback.m_hitPointWorld - m_current_pos).length();

			target_pos = updateTargetPositionBasedOnCollision( callback.m_hitNormalWorld, target_pos );
			btVector3 currentDir = target_pos - m_current_pos;
			distance2 = currentDir.length2();
			if (distance2 > SIMD_EPSILON)
			{
				currentDir.normalize();
				/* See Quake2: "If velocity is against original velocity, stop ead to avoid tiny oscilations in sloping corners." */
				if (currentDir.dot(m_normalizedDirection) <= btScalar(0.0))
				{
					break;
				}
			} else
			{
				break;
			}

		} else {
			// we moved whole way
			m_current_pos = target_pos;
		}
	}
}

void bullet_character_controller::step_down( float dt, bool change_size_only, btVector3 const& pos_up_correction )
{
	BT_PROFILE("step_down");

	btTransform start;
	start.setIdentity( );	start.setOrigin( m_current_pos );

	float step_height = m_vertical_velocity < 0.f ? -m_vertical_velocity * dt : 0.f;
	if ( s_step_height > step_height && m_was_on_ground )
		step_height = s_step_height;

	btVector3 finish_pos = m_current_pos - m_up_vector * ( m_current_step_offset + pos_up_correction.getY( ) + step_height );
	btTransform finish;
	finish.setIdentity( );
	finish.setOrigin( finish_pos );

	character_move_test_callback callback( m_ghost_object, m_up_vector, m_max_slope_angle_cos );
	callback.m_collisionFilterGroup = m_collision_filter_group;
	callback.m_collisionFilterMask = m_collision_filter_mask;

	if ( m_useGhostObjectSweepTest )
	{
		m_ghost_object->convexSweepTest( &m_shape, start, finish, callback, s_cc_max_allowed_penetration_value );
	} else
	{
		m_collision_world->convexSweepTest( &m_shape, start, finish, callback, s_cc_max_allowed_penetration_value );
	}

	if ( callback.hasHit( ) )
	{
		btTransform worldTransformInv = m_ghost_object->getWorldTransform( ).inverse( );
		btVector3 hitPointLocal = worldTransformInv * callback.m_hitPointWorld;

		if ( hitPointLocal.dot( m_up_vector ) <= s_step_height )
		{
			m_current_pos.setInterpolate3( start.getOrigin( ), finish.getOrigin( ), callback.m_closestHitFraction );
			m_vertical_velocity = 0.0f;
			m_jumping = false;
			m_on_steep_slope = false;
		} else
		{
			LOG_INFO( "dddd" );
		}
	} else
		m_current_pos = finish.getOrigin( );

	m_current_pos += pos_up_correction;
	m_current_pos.setY( m_current_pos.getY( ) + m_shape.getMargin( ) );
	setup_crouch_state( m_in_crouch );
}

void bullet_character_controller::set_desired_walk_vector( btVector3 const& walk_vector )
{
	m_has_updates = false;
	m_walk_vector = walk_vector;

	if ( !m_walk_vector.isZero( ) )
		m_normalizedDirection = getNormalizedVector( m_walk_vector );

	m_walk_vector_applied = false;
}

void bullet_character_controller::pre_step( float dt )
{
	BT_PROFILE("pre_step");
	m_pre_step_position = m_ghost_object->getWorldTransform( ).getOrigin( );

	for ( s32 i = 0 ; recover_from_penetration( ) > 0.05f ; )
	{
		++i;
		if ( i > 3 )
			break;
	}

	m_current_pos = m_ghost_object->getWorldTransform( ).getOrigin( );
}

bool bullet_character_controller::can_jump( ) const
{
	return !in_crouch( ) && on_ground( ) && !m_on_steep_slope;
}

void bullet_character_controller::jump( )
{
	if ( can_jump( ) )
	{
		m_jumping = true;
		m_positions.clear( );
	}
}

void bullet_character_controller::end_jump( )
{
	m_jumping = false;
}

bool bullet_character_controller::on_ground( ) const
{
	return math::abs( m_vertical_velocity ) < math::epsilon_3;
}

// x = capsule diameter
// y = full capsule height
void bullet_character_controller::setup_shape_dim( float2 const& shape_dim )
{
	m_shape.setImplicitShapeDimensions(
		btVector3(
			shape_dim.x * 0.5f,
			( shape_dim.y - shape_dim.x ) * 0.5f,
			shape_dim.x * 0.5f
		)
	);
}

void bullet_character_controller::setup_crouch_state( bool crouch )
{
	btVector3 prev_shape_offset = m_shape_offset;
	m_in_crouch = crouch;
    float shape_y;
    if ( crouch )
    {
    	m_current_shape_dim = m_crouch_shape_dim;
		setup_shape_dim( m_current_shape_dim );
        shape_y = m_crouch_shape_dim.y;
    }
    else
    {
        m_current_shape_dim = m_stand_shape_dim;
		setup_shape_dim( m_current_shape_dim );
        shape_y = m_stand_shape_dim.y;
    }
	m_shape_offset.setZero( );
	m_shape_offset.setY( shape_y * 0.5f );

	btVector3 orign = m_ghost_object->getWorldTransform( ).getOrigin( );
	orign -= prev_shape_offset - m_shape_offset;
	m_ghost_object->getWorldTransform( ).setOrigin( orign );
	m_ghost_object->setInterpolationWorldTransform(  m_ghost_object->getWorldTransform( ) );

	m_ghost_object->setCollisionShape( &m_shape );

	if ( m_collision_world )
		m_ghost_object->getOverlappingPairCache( )->cleanProxyFromPairs(
			m_ghost_object->getBroadphaseHandle( ),
			m_collision_world->getDispatcher( )
		);
}

void bullet_character_controller::insert( btDynamicsWorld* world )
{
	m_collision_world = world;
	setup_crouch_state( false );
	m_collision_world->addCollisionObject( m_ghost_object, m_collision_filter_group, m_collision_filter_mask );
	m_collision_world->addAction( this );
	m_positions.clear( );
}

void bullet_character_controller::remove( btDynamicsWorld* world )
{
	m_collision_world->removeAction( this );
	m_collision_world->removeCollisionObject( m_ghost_object );
	m_positions.clear( );

	m_ghost_object->getOverlappingPairs( ).clear( );
	m_collision_world = NULL;
}

btTransform bullet_character_controller::get_transform( )
{
	btTransform transform = m_ghost_object->getWorldTransform( );
	transform.setOrigin( transform.getOrigin( ) - m_shape_offset );
	return transform;
}

void bullet_character_controller::set_transform( btTransform const& transform )
{
	m_ghost_object->setWorldTransform( btTransform( transform.getRotation( ), transform.getOrigin( ) + m_shape_offset ) );
	m_ghost_object->setInterpolationWorldTransform( m_ghost_object->getWorldTransform( ) );
}

void bullet_character_controller::set_crouch( bool crouch )
{
	if ( crouch == m_in_crouch )
		return;

	if ( crouch )
	{
		setup_crouch_state( true );
	}
	else
	{
		setup_crouch_state( false );
	}

	m_positions.clear( );
}

bool bullet_character_controller::can_stand( )
{
	return true;
}

} // namespace physics
} // namespace vostok
