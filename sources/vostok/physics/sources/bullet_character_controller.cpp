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


u16 const*	g_game_material_groups;
s32			g_game_materials_count;

static bool	logging	= false;

const btVector3 bullet_character_controller::m_up_vector = btVector3( 0.0f, 1.0f, 0.0f );

// STATE[100%|DONE]
static btVector3 getNormalizedVector( btVector3 const& v )
{
	btVector3 result = v.normalized( );		// <0x584e69>|0x000|0x000:'61'
	if ( result.length( ) < SIMD_EPSILON )	// <0x584ed2>|0x069|0x069:'62'
		result.setZero( );
	return result;							// <0x584f14>|0x0ab|0x042:'65'
}

// STATE[100%|DONE]
btVector3 computeReflectionDirection( btVector3 const& direction, btVector3 const& normal )
{
	return direction - 2 * normal.dot( direction ) * normal;	// <0x584ae6>|0x000|0x000:'75'
}

// STATE[100%|DONE]: The structure doesn't match
btVector3 parallelComponent( btVector3 const& direction, btVector3 const& normal )
{
	return direction.dot( normal ) * normal;
}

// STATE[100%|DONE]
btVector3 perpindicularComponent( btVector3 const& direction, btVector3 const& normal )
{
	return direction - normal.dot( direction ) * normal;	// <0x584a66>|0x000|0x000:'92'
}

// STATE[100%|DONE]: sushi@NOTE: This function is used in `survarium` module.
void setup_game_material_groups( u16 const* game_material_groups, u16 game_materials_count )
{
	g_game_material_groups = game_material_groups;
	g_game_materials_count = game_materials_count;	// <0x584530>|0x000|0x000:'201'
}

class character_move_test_callback : public btCollisionWorld::ClosestConvexResultCallback , public boost::noncopyable {
public:
	// STATE[100%|DONE]
						character_move_test_callback	( btCollisionObject* self, btVector3 const& up_vector, float minSlopeDot ) :
							ClosestConvexResultCallback	( btVector3( 0.0f, 0.0f, 0.0f ), btVector3( 0.0f, 0.0f, 0.0f ) ),
							m_up_vector					( up_vector ),
							m_self						( self ),
							m_minSlopeDot				( minSlopeDot ) {}

	// STATE[100%|DONE]
	virtual	float		addSingleResult					( btCollisionWorld::LocalConvexResult& convexResult, bool normalInWorldSpace ) override
	{
		if ( convexResult.m_hitCollisionObject == m_self )
			return 2.0f; // sushi@NOTE: In bullet this was 1.0f. Why?

		btVector3 hitNormalWorld;
		if (normalInWorldSpace)
		{
			hitNormalWorld = convexResult.m_hitNormalLocal;
		} else
		{	// sushi@NOTE: I did the transform with origin as well. Does this matter? Understand this better
			///need to transform normal into worldspace
			hitNormalWorld = convexResult.m_hitCollisionObject->getWorldTransform().getBasis()*convexResult.m_hitNormalLocal;
		}

		btScalar dotUp = m_up_vector.dot(hitNormalWorld);
		if (dotUp < m_minSlopeDot) {
			return btScalar(0.0);  // sushi@NOTE: In bullet this was 1.0f. Why?
		}
		return ClosestConvexResultCallback::addSingleResult (convexResult, normalInWorldSpace);
	}


private:
	/* 0x0000 */	/* btCollisionWorld::ClosestConvexResultCallback */
	/* 0x0060 */	/* boost::noncopyable */
	/* 0x0060 */	btVector3				m_up_vector;
	/* 0x0070 */	btCollisionObject*		m_self;
	/* 0x0074 */	float					m_minSlopeDot;
}; // class character_move_test_callback

STATIC_SIZE_ASSERT(character_move_test_callback, 0x80);

// STATE[91.29%|DONE]: Target writes to local stack zero and then clear that local stack. Possibly LTCG artifacts
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
	m_collision_filter_group	( 4 ), // collisionFilterGroup ),	// LTCG'ed to 4 sushi@TODO, recover
	m_collision_filter_mask		( 2 ), // collisionFilterMask ),	// LTCG'ed to 2
	m_max_slope_in_radians		( math::pi_d3 ),
	m_max_slope_angle_cos		( cosf( math::pi_d3 ) ),
	m_gravity					( 29.4f ),							// 29.400002
	m_was_on_ground				( false ),
	m_jumping					( false ),
	m_useGhostObjectSweepTest	( true ),
	m_walk_vector_applied		( false ),
	m_on_steep_slope			( false ),
	m_has_updates				( false ),
	m_positions					( )
{
	setup_crouch_state( false ); // <0x584e4b>|0x000|0x000:'310'
}

// STATE[83%|DONE]: LTCG for `this` in destructor.
bullet_character_controller::~bullet_character_controller( )
{
}

// STATE[99.80%|PARTIAL]
// The implementation is based on `btKinematicCharacterController::updateTargetPositionBasedOnCollision`.
// Further matches might come from updating `computeReflectionDirection`, possibly, even though it matches 100%, when inlined, it doesn't.
// Can also be because of `normalMag` check that was compiled out by LTCG.
btVector3 bullet_character_controller::updateTargetPositionBasedOnCollision(
	btVector3 const&	hitNormal,
	btVector3 const&	target_pos,
	float				tangentMag, // unused
	float				normalMag
)
{
	BT_PROFILE("updateTargetPositionBasedOnCollision"); // <0x585470>|0x000|0x000:'319'

	btVector3 result = m_current_pos;
	btVector3 movementDirection = target_pos - m_current_pos;

	float movement_length = movementDirection.length( );
	if ( movement_length > FLT_EPSILON )
	{
		movementDirection.normalize( ); // sushi@NOTE: As far as I understand, normalizing movement gives more precise math, but isn't required

		btVector3 reflectDir = computeReflectionDirection( movementDirection, hitNormal ); // reflection here is not needed
		reflectDir.normalize( );

		btVector3 perpindicularDir  = perpindicularComponent( reflectDir, hitNormal );

		// if (normalMag != 0.0) // sushi@NOTE: While LTCG should get rid of it, this reduces the match
		{
			// btVector3 perpComponent = perpindicularDir * btScalar ( normalMag * movement_length );
			result +=  perpindicularDir * btScalar ( normalMag * movement_length );
		}

		// <0x585677>|0x207|0x01e:'343' // sushi@NOTE: Based on the `vostok_structure` this makes more sense, but when I do that assembly breaks in other places.
	}
	return result;
}

// STATE[96%|DONE]: Logging is still not 100% matched
void bullet_character_controller::updateAction( btCollisionWorld* collisionWorld, float deltaTime )
{
	BT_PROFILE("updateAction1");																				// <0x5863f9>|0x000|0x000:'356'
	if ( logging )																								// <0x58643a>|0x041|0x041:'357'
		LOG_INFO( "updateAction Start  %f %f %f", m_current_pos.x( ), m_current_pos.y( ), m_current_pos.z( ) );	// <0x586446>|0x04d|0x00c:'359'

	pre_step( deltaTime );																						// <0x586534>|0x13b|0x0ee:'362'
	if ( logging )																								// <0x586539>|0x140|0x005:'363'
		LOG_INFO( "updateAction Middle %f %f %f", m_current_pos.x( ), m_current_pos.y( ), m_current_pos.z( ) );	// <0x586546>|0x14d|0x00d:'364'

	player_step( deltaTime );																					// <0x58663a>|0x241|0x0f4:'366'
	if ( logging )																								// <0x586648>|0x24f|0x00e:'367'
		LOG_INFO( "updateAction Finish %f %f %f", m_current_pos.x( ), m_current_pos.y( ), m_current_pos.z( ) );	// <0x586655>|0x25c|0x00d:'368'

	m_walk_vector.setZero( );																					// <0x58674e>|0x355|0x0f9:'369'
}

// STATE[99%|DONE]: LTCG for `step_forward_and_strafe`. Might get fixed after it is implemented properly.
void bullet_character_controller::player_step( float dt )
{
	// static bool use_shape_size = <0x10000>;

	BT_PROFILE("player_step");

	m_has_updates = true;
	m_was_on_ground = on_ground( );

	if ( m_jumping )
		m_vertical_velocity = m_walk_vector.y( ) / dt;
	else
	{  // sushi@NOTE: Why are we always falling
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

// STATE[96.92%|PARTIAL]
float bullet_character_controller::recover_from_penetration( )
{
	BT_PROFILE("recover_from_penetration"); // <0x58506d>|0x000|0x000:'429'
	m_collision_world->getDispatcher( )->dispatchAllCollisionPairs(
		m_ghost_object->getOverlappingPairCache( ),
		m_collision_world->getDispatchInfo( ),
		m_collision_world->getDispatcher( )
	);

	m_current_pos = m_ghost_object->getWorldTransform( ).getOrigin( );

	float maxPen = 0.0f;
	float shape_y = math::abs( m_shape_offset.y( ) );
	btManifoldArray manifold_array;

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
						float weight = math::pow( pos_on_shape_y, 3 ); // on decomp.me the fix was to manually unroll `pow`. Sadly it didn't help here

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

// STATE[100%|DONE]
void bullet_character_controller::step_up( bool change_shape_size, btVector3& pos_up_correction )
{
	float new_cylinder_height = math::max( 0.0f, ( m_current_shape_dim.y - m_current_shape_dim.x ) - s_step_height );

	float new_full_height = m_current_shape_dim.x + new_cylinder_height;
	setup_shape_dim( float2( m_current_shape_dim.x, new_full_height ) );

	pos_up_correction.setValue( 0.0f, ( m_current_shape_dim.y - new_full_height ) * 0.5f, 0.0f );

    m_current_pos += pos_up_correction;
	m_current_step_offset = pos_up_correction.y( );

	/* This one is way easier to read, but doesn't match exactly

	// float diameter = m_current_shape_dim.x;
	// float full_height = m_current_shape_dim.y;
	// float new_cylinder_height = ( full_height - diameter ) - s_step_height;
	// new_cylinder_height = math::max( 0.0f, new_cylinder_height );
	
	float new_full_height = diameter + new_cylinder_height;
	setup_shape_dim( float2( diameter, new_full_height ) );

	pos_up_correction.setValue( 0.0f,  ( full_height - new_full_height ) * 0.5f, 0.0f );

    m_current_pos += pos_up_correction;
	m_current_step_offset = pos_up_correction.y( );
	*/
}

// STATE[98.71%|DONE]: LTCG for s_cc_max_allowed_penetration_value
void bullet_character_controller::step_forward_and_strafe( btVector3 const& walkMove )
{
	BT_PROFILE("step_forward_and_strafe");

	btTransform start, end;
	btVector3 target_pos = m_current_pos + walkMove;

	start.setIdentity ();
	end.setIdentity ();

	btScalar fraction = 1.0;
	btScalar distance2 = (m_current_pos-target_pos).length2();

	if ( distance2 < FLT_EPSILON ) // check is done in a different order
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
//				printf("currentDir: don't normalize a zero vector\n");
				break;
			}

		} else {
			// we moved whole way
			m_current_pos = target_pos;
		}
	}
}

// STATE[93.50%|PARTIAL]: Failed to match this further. See comments as to why.
// * There were 44 commented out lines, possibly with some future logic.
// * btKinematicCharacterController has a slightly different order for transforms and `finish_pos`.
void bullet_character_controller::step_down( float dt, bool change_size_only, btVector3 const& pos_up_correction )
{
	BT_PROFILE("step_down");

	btTransform start;
	start.setIdentity( );
	start.setOrigin( m_current_pos );

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
		// sushi@NOTE: Even though target has it, seems like the transform here is not needed in general,
		// since `ghost_object` isn't supposed to rotate anyway. So the same y coordinates can be used.
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

			// sushi@NOTE: There is no code here. Which means the player will be stuck in air when the fall is too high?
		}
	} else
		m_current_pos = finish.getOrigin( );

	m_current_pos += pos_up_correction;
	m_current_pos.setY( m_current_pos.getY( ) + m_shape.getMargin( ) );
	setup_crouch_state( m_in_crouch );

	// FUNCTION BODY
	// <0x585cdc>|0x000|0x000:'627' BT_PROFILE("step_down");
	// <1>
	// <2>
	// <0x585d1c>|0x040|0x040:'630' <xor>
	// <0x585d1f>|0x043|0x003:'631' btTransform start( btMatrix3x3::getIdentity( ), m_current_pos );
	// <1>
	// <0x585d43>|0x067|0x024:'633' float step_height = m_vertical_velocity >= 0 ? 0.0f : - m_vertical_velocity * dt;
	// <1>
	// <0x585dcd>|0x0f1|0x08a:'635' if ( s_step_height > step_height && m_was_on_ground )
	// <0x585de3>|0x107|0x016:'636'		step_height = s_step_height;
	// <1>
	// <2>
	// <3>
	// <0x585de6>|0x10a|0x003:'640' btVector3 finish_pos = m_current_pos - m_up_vector * ( m_current_step_offset + pos_up_correction.getY( ) + step_height );
	// <1>
	// <2>
	// <3>
	// <0x585de9>|0x10d|0x003:'644' character_move_test_callback callback( m_ghost_object, m_up_vector, m_max_slope_angle_cos );
	// <1>
	// <2>
	// <3>
	// <0x585eca>|0x1ee|0x0e1:'648' if ( m_useGhostObjectSweepTest )
	// <1>							{
	// <0x585eef>|0x213|0x025:'650'		m_ghost_object->convexSweepTest( m_shape, start, end, callback );
	// <0x585f19>|0x23d|0x02a:'651' } else
	// <1>
	// <0x585f1b>|0x23f|0x002:'653'		m_collision_world->convexSweepTest( m_shape, start, end, callback, s_cc_max_allowed_penetration_value );
	// <1>
	// <2>
	// <0x585f44>|0x268|0x029:'656' if ( callback.m_closestHitFraction < 1.0f )
	// <1>							{
	// <0x585f5a>|0x27e|0x016:'658'		btTransform worldTransform = m_ghost_object->getWorldTransform( ).inverse( );
	// <0x585f6c>|0x290|0x012:'659'
	// <1>
	// <0x585fc0>|0x2e4|0x054:'661'		if ( hitPointWorld.dot( s_step_height ) < s_step_height )
	// <1>								{
	// <0x586039>|0x35d|0x079:'663'			m_current_pos = ( 1.0f - callback.m_closestHitFraction ) * start.getOrigin( ) - end.getOrigin( );
	// <0x5860ae>|0x3d2|0x075:'664'			m_vertical_velocity = 0.0;
	// <0x5860c2>|0x3e6|0x014:'665'			m_jumping = 0;
	// <0x5860c9>|0x3ed|0x007:'666'			m_on_steep_slope = 0;
	// <0x5860d0>|0x3f4|0x007:'667'		} else
	// <1>
	// <0x5860d5>|0x3f9|0x005:'669'			LOG_INFO( "dddd" );
	// <1>
	// <2>
	// <0x586188>|0x4ac|0x0b3:'672'
	// <1>
	// <2>							} else
	// <0x58618d>|0x4b1|0x005:'675'		m_current_pos = end.getOrigin( );
	// <1>

	// <44>
	// <0x5861a9>|0x4cd|0x01c:'720' m_current_pos += pos_up_correction;
	// <0x5861d5>|0x4f9|0x02c:'721' m_current_pos.getY( ) += m_shape.getMargin( );
	// <0x5861e1>|0x505|0x00c:'722' setup_crouch_state( m_in_crouch );
	// <1>
	// ******
}

// STATE[100%|DONE]
void bullet_character_controller::set_desired_walk_vector( btVector3 const& walk_vector )
{
	m_has_updates = false;	// <0x584fde>|0x000|0x000:'728'
	m_walk_vector = walk_vector;

	if ( !m_walk_vector.isZero( ) )
		m_normalizedDirection = getNormalizedVector( m_walk_vector );

	m_walk_vector_applied = false;
}

// STATE[100%|DONE]
void bullet_character_controller::pre_step( float dt )
{
	BT_PROFILE("pre_step");														// <0x5856c0>|0x000|0x000:'739'
	m_pre_step_position = m_ghost_object->getWorldTransform( ).getOrigin( );	// <0x5856f7>|0x037|0x037:'740'

	for ( s32 i = 0 ; recover_from_penetration( ) > 0.05f ; )
	{
		if ( ++i > 3 )
			break;
	}

	m_current_pos = m_ghost_object->getWorldTransform( ).getOrigin( );			// <0x58573c>|0x07c|0x015:'757'
}

// sushi@NOTE: 300 empty lines

// STATE[100%|DONE]
bool bullet_character_controller::can_jump( ) const
{
	return !in_crouch( ) && on_ground( ) && !m_on_steep_slope; // <0x5845f1>|0x000|0x000:'1077'
}

// STATE[100%|DONE]
void bullet_character_controller::jump( )
{
	if ( can_jump( ) )			// <0x5849f1>|0x000|0x000:'1082'
	{
		m_jumping = true;
		m_positions.clear( );	// <0x584a2c>|0x03b|0x03b:'1089'
	}
}

// STATE[100%|DONE]
void bullet_character_controller::end_jump( )
{
	m_jumping = false; // <0x584520>|0x000|0x000:'1094'
}

// STATE[100%|DONE]
bool bullet_character_controller::on_ground( ) const
{
	return math::abs( m_vertical_velocity ) < math::epsilon_3; // <0x5845b1>|0x000|0x000:'1105'
}

// STATE[100%|DONE]
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
	); // <0x584549>|0x000|0x000:'1111'
}

// STATE[92%|PARTIAL]: The closest I was able to get. The assembly is the same except for a few instructions reshuffled.
void bullet_character_controller::setup_crouch_state( bool crouch )
{
	btVector3 prev_shape_offset = m_shape_offset;												// <0x584809>|0x000|0x000:'1122'
	m_in_crouch = crouch;
    float shape_y;
    if ( crouch )
    {
    	m_current_shape_dim = m_crouch_shape_dim;
		setup_shape_dim( m_current_shape_dim );		// up until this point no breakpoints in structure
        shape_y = m_crouch_shape_dim.y;
    }												// this empty line is the most confusing, since it doesn't match structure
    else											// while everything else (including this `else`) does
    {
        m_current_shape_dim = m_stand_shape_dim;
		setup_shape_dim( m_current_shape_dim );
        shape_y = m_stand_shape_dim.y;
    }
	m_shape_offset.setZero( ); // Creating a new btVector and assigning it results in new stack variables.`
	m_shape_offset.setY( shape_y * 0.5f );

	btVector3 orign = m_ghost_object->getWorldTransform( ).getOrigin( );
	orign -= prev_shape_offset - m_shape_offset;
	m_ghost_object->getWorldTransform( ).setOrigin( orign );
	m_ghost_object->setInterpolationWorldTransform(  m_ghost_object->getWorldTransform( ) );	// <0x58499c>|0x193|0x016:'1136'

	m_ghost_object->setCollisionShape( &m_shape );												// <0x5849aa>|0x1a1|0x00e:'1138'

	if ( m_collision_world )																	// <0x5849be>|0x1b5|0x014:'1140'
		m_ghost_object->getOverlappingPairCache( )->cleanProxyFromPairs(
			m_ghost_object->getBroadphaseHandle( ),
			m_collision_world->getDispatcher( )													// <0x5849c5>|0x1bc|0x007:'1141'
		);
}

// STATE[94%|PARTIAL]: Target preserves `ecx` by pushing it in prologue.
void bullet_character_controller::insert( btDynamicsWorld* world )
{
	m_collision_world = world; // <0x584bb4>|0x000|0x000:'1152'
	setup_crouch_state( false );
	m_collision_world->addCollisionObject( m_ghost_object, m_collision_filter_group, m_collision_filter_mask );
	m_collision_world->addAction( this );
	m_positions.clear( );
}

// STATE[100%|DONE]
void bullet_character_controller::remove( btDynamicsWorld* world )
{
	// ASSERT( m_collision_world == world )? Why do we even need to pass world
	m_collision_world->removeAction( this );					// <0x584766>|0x000|0x000:'1165'
	m_collision_world->removeCollisionObject( m_ghost_object );	// <0x584773>|0x00d|0x00d:'1166'
	m_positions.clear( );										// <0x584784>|0x01e|0x011:'1167'

	m_ghost_object->getOverlappingPairs( ).clear( );			// <0x5847ac>|0x046|0x028:'1171'
	m_collision_world = NULL;									// <0x5847eb>|0x085|0x03f:'1173'
}

// STATE[100%|DONE]
btTransform bullet_character_controller::get_transform( )
{
	btTransform transform = m_ghost_object->getWorldTransform( );	// <0x5846a9>|0x000|0x000:'1178'
	transform.setOrigin( transform.getOrigin( ) - m_shape_offset );	// <0x584701>|0x058|0x058:'1180'
	return transform;
}

// STATE[100%|DONE]
void bullet_character_controller::set_transform( btTransform const& transform )
{
	m_ghost_object->setWorldTransform( btTransform( transform.getRotation( ), transform.getOrigin( ) + m_shape_offset ) );	// <0x584f4c>|0x000|0x000:'1186'
	m_ghost_object->setInterpolationWorldTransform( m_ghost_object->getWorldTransform( ) );									// <0x584fb7>|0x06b|0x06b:'1187'
}

// STATE[100%|DONE]
void bullet_character_controller::set_crouch( bool crouch )
{
	if ( crouch == m_in_crouch ) // <0x584b6a>|0x000|0x000:'1192'
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

// STATE[100%|DONE]: The actual logic was commented out
bool bullet_character_controller::can_stand( )
{
	return true; // <0x584510>|0x000|0x000:'1208'
}

} // namespace physics
} // namespace vostok
