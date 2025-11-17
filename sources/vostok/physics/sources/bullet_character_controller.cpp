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

static float s_cc_max_allowed_penetration_value			= 0.04f;
static console_commands::cc_float s_cc_max_allowed_penetration_cc			( "cc_max_allowed_penetration", s_cc_max_allowed_penetration_value, 0.0f, 1.0f, false, console_commands::command_type_engine_internal );

static bool  s_cc_prevent_step_bouncing_value			= true;
static console_commands::cc_bool  s_cc_prevent_step_bouncing_cc				( "cc_prevent_step_bouncing", s_cc_prevent_step_bouncing_value, false, console_commands::command_type_engine_internal );

static float s_character_sliping_speed_multiplier_value = 20.0f;
static console_commands::cc_float s_character_sliping_speed_multiplier_cc	( "cc_sliping_speed_multiplier", s_character_sliping_speed_multiplier_value, 0.01f, 100.0f, true, console_commands::command_type_engine_internal );

static float s_step_height								= 0.6f;
static console_commands::cc_float s_step_height_command						( "character_controller_step_height", s_step_height, 0.0f, 2.0f, true, console_commands::command_type_engine_internal );

u16 const*	g_game_material_groups;
s32			g_game_materials_count;

static bool	logging	= false;


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

// STATE[STUB]: sushi@NOTE: This function is used in `survarium` module.
void setup_game_material_groups( u16 const* game_material_groups, u16 game_materials_count )
{
	g_game_material_groups = game_material_groups;
	g_game_materials_count = game_materials_count;	// <0x584530>|0x000|0x000:'201'
}

class character_move_test_callback : public btCollisionWorld::ClosestConvexResultCallback , public boost::noncopyable {
public:
	// STATE[STUB]
						character_move_test_callback	( btCollisionObject* self, btVector3 const& up_vector, float minSlopeDot ) :
							ClosestConvexResultCallback	( btVector3( 0.0f, 0.0f, 0.0f ), btVector3( 0.0f, 0.0f, 0.0f ) ),
							m_up_vector					( up_vector ),
							m_self						( self ),
							m_minSlopeDot				( minSlopeDot ) {}

	// STATE[STUB]
	// sushi@NOTE: Understand this a bit better
	virtual	float		addSingleResult					( btCollisionWorld::LocalConvexResult& convexResult, bool normalInWorldSpace ) override
	{
		if ( convexResult.m_hitCollisionObject == m_self )
			return 2.0f;

		btVector3 hit_normal = normalInWorldSpace
			? convexResult.m_hitNormalLocal
			: convexResult.m_hitCollisionObject->getWorldTransform( ) * convexResult.m_hitNormalLocal;

		if ( m_minSlopeDot <= m_up_vector.dot( hit_normal ) )
			return btCollisionWorld::ClosestConvexResultCallback::addSingleResult( convexResult, normalInWorldSpace );
		else
			return 0.0f;

		// FUNCTION BODY
		// <0xde4c6>|0x000|0x000:'238'
		// <0xde4d6>|0x010|0x010:'239'
		// <1>
		// <2>
		// <0xde4e3>|0x01d|0x00d:'242'
		// <1>
		// <0xde4ea>|0x024|0x007:'244'
		// <0xde4fa>|0x034|0x010:'245'
		// <1>
		// <2>
		// <0xde4ff>|0x039|0x005:'248'
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
		// <0xde5a3>|0x0dd|0x0a4:'262'
		// <0xde5ce>|0x108|0x02b:'263'
		// <0xde5dc>|0x116|0x00e:'264'
		// <1>
		// <0xde5e5>|0x11f|0x009:'266'
		// ******
	}


private:
	/* 0x0000 */	/* btCollisionWorld::ClosestConvexResultCallback */
	/* 0x0060 */	/* boost::noncopyable */
	/* 0x0060 */	btVector3				m_up_vector;
	/* 0x0070 */	btCollisionObject*		m_self;
	/* 0x0074 */	float					m_minSlopeDot;
}; // class character_move_test_callback

STATIC_SIZE_ASSERT(character_move_test_callback, 0x80);

// STATE[STUB]
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
	m_collision_filter_group	( 4 ), // collisionFilterGroup ),	// LTCG'ed to 4
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

// STATE[STUB]
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
	float				tangentMag, // definitely unused
	float				normalMag   // possibly used for compiled out check, see original bullet impl
)
{
	BT_PROFILE("updateTargetPositionBasedOnCollision"); // <0x585470>|0x000|0x000:'319'

	btVector3 result = m_current_pos;
	btVector3 movementDirection = target_pos - m_current_pos;

	float movement_length = movementDirection.length( );
	if ( movement_length > FLT_EPSILON )
	{
		movementDirection.normalize( ); // sushi@NOTE: As far as I understand, normalizing movement gives more precise math, but isn't required

		btVector3 reflectDir = computeReflectionDirection( movementDirection, hitNormal );
		reflectDir.normalize( );

		btVector3 newMovementDirection = perpindicularComponent( reflectDir, hitNormal );

		// <0x585677>|0x207|0x01e:'343' // sushi@NOTE: Based on the `vostok_structure` this makes more sense, but when I do that assembly breaks in other places.
		// newMovementDirection *= movement_length;
		// result += newMovementDirection;
		result += newMovementDirection * movement_length;
	}

	return result;
}

// STATE[STUB]
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

// STATE[STUB]
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
		if ( -m_max_fall_speed >= fall_speed )
			m_vertical_velocity = -m_max_fall_speed;
		else if ( m_jump_speed < fall_speed )
			m_vertical_velocity = m_jump_speed;
		else
			m_vertical_velocity = fall_speed;
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

// STATE[78%|STUB]
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
	float shape_y = m_shape_offset.y( );
	btManifoldArray manifold_array;

	for ( s32 i = 0 ; i < m_ghost_object->getOverlappingPairCache( )->getNumOverlappingPairs( ) ; ++i )
	{
		manifold_array.clear( );
		btBroadphasePair& pair = m_ghost_object->getOverlappingPairCache( )->getOverlappingPairArray( )[i];

		if ( pair.m_algorithm )
			pair.m_algorithm->getAllContactManifolds( manifold_array );

		for ( s32 j = 0 ; j < manifold_array.size( ) ; ++j )
		{
			btPersistentManifold* manifold = manifold_array[j];
			bool isFirstBody = manifold->getBody0( ) == m_ghost_object;
			float normalSign = isFirstBody ? -1.0f : 1.0f;

			for ( s32 k = 0 ; k < manifold->getNumContacts( ) ; ++k )
			{
				btManifoldPoint& contact = manifold->getContactPoint( k );

				if ( contact.getDistance( ) < 0.0f )
				{
					if ( maxPen > contact.getDistance( ) )
					{
						maxPen = contact.getDistance( );
						btVector3 pos_on_shape = isFirstBody ? contact.m_localPointA : contact.m_localPointB;

						float weight = math::pow( ( shape_y - math::abs( pos_on_shape.y( ) ) ) / shape_y, 3 );

						btVector3 displacement = contact.m_normalWorldOnB * normalSign * contact.getDistance( );
						btVector3 weighedDisplacement = btVector3( displacement.x( ) * weight, displacement.y( ) * ( 1 - weight ) , displacement.z( ) * weight );
						m_current_pos += weighedDisplacement;
					}
				}
			}
		}
	}

	btTransform world = m_ghost_object->getWorldTransform( );
	world.setOrigin( m_current_pos );
	m_ghost_object->setWorldTransform( world );
	return math::abs( maxPen );

	// FUNCTION BODY
	// <0x58506d>|0x000|0x000:'429' BT_PROFILE("recover_from_penetration")
	// <1>
	// <2>
	// <3>
	// <0x5850a6>|0x039|0x039:'433' m_collision_world->getDispatcher( )->dispatchAllCollisionPairs(
	// <1>
	// <0x5850c5>|0x058|0x01f:'435' m_ghost_object->getWorldTransform( ).getOrigin( );
	// <1>
	// <2>
	// <0x5850df>|0x072|0x01a:'438' float maxPen = 0.0f;
	// <1>
	// <0x5850e8>|0x07b|0x009:'440' float shape_y = m_shape_offset.y( );
	// <1>
	// <0x585108>|0x09b|0x020:'442' for ( s32 i = 0 ; i < m_ghost_object->getOverlappingPairCache( )->getNumOverlappingPairs( ) ; ++i )
	// <1>
	// <0x585131>|0x0c4|0x029:'444' manifold_array.clear( )
	// <1>
	// <0x585192>|0x125|0x061:'446' btBroadphasePair& pair = m_ghost_ob
	// <1>
	// <0x5851b0>|0x143|0x01e:'448' if ( pair.m_algorithm )
	// <0x5851b5>|0x148|0x005:'449' pair.m_algorithm->getAllContactManifolds( manifold_array );
	// <1>
	// <2>
	// <0x5851c6>|0x159|0x011:'452' for ( s32 j = 0 ; j < manifold_a
	// <1>
	// <0x5851f0>|0x183|0x02a:'454' 	btPersistentManifold* manifold = manifold_array[j];
	// <0x5851f7>|0x18a|0x007:'455' void* body0 = manifold->getB
	// <0x585206>|0x199|0x00f:'456' float sign = body0 == m_
	// <1>
	// <0x585217>|0x1aa|0x011:'458' for ( s32 k = 0 ; k < manifold->
	// <1>
	// <2>
	// <3>
	// <0x585230>|0x1c3|0x019:'462' ????
	// <1>
	// <0x585235>|0x1c8|0x005:'464' if ( contact.getDistance( ) < 0.0f && maxPen > contact.getDistance( ) )
	// <1>
	// <2>
	// <3>
	// <4>
	// <0x585241>|0x1d4|0x00c:'469' ????? maxPen > contact.getDistance( ) )
	// <1>
	// <0x585250>|0x1e3|0x00f:'471' maxPen = curPen;
	// <1>
	// <0x585256>|0x1e9|0x006:'473' btVector3 pos_on_shape = isFirstBody ? contact.m_localPointA : contact.m_localPoint
	// <1>
	// <2>
	// <3>
	// <0x585264>|0x1f7|0x00e:'477' mov
	// <0x5852bd>|0x250|0x059:'478' mul
	// <1>
	// <0x5852dc>|0x26f|0x01f:'480' m_current_pos += btVector3( displaceme
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <0x58535a>|0x2ed|0x07e:'486'
	// <0x58537e>|0x311|0x024:'487'
	// <0x585388>|0x31b|0x00a:'488'
	// <0x58538d>|0x320|0x005:'489'
	// ******
}

// STATE[STUB]
void bullet_character_controller::step_up( bool change_shape_size, btVector3& pos_up_correction )
{
	float radius = m_current_shape_dim.x;
	float height = m_current_shape_dim.y;

    float step_height = ( height - radius ) - s_step_height;
    if ( step_height < 0.0f )
        step_height = 0.0f;

	// if ( change_shape_size )
	setup_shape_dim( float2( radius, radius + step_height ) );


	pos_up_correction = btVector3(
		0.0f,
		( height - ( radius + step_height ) ) * 0.5f,
		0.0f
	);
	m_current_pos += pos_up_correction;
	m_current_step_offset = pos_up_correction.y( );
}

// STATE[STUB]
void bullet_character_controller::step_forward_and_strafe( btVector3 const& walkMove )
{

	updateTargetPositionBasedOnCollision( walkMove, walkMove, 10, 10 );
	// LOCALS
	// btVector3 					target_pos
	// btTransform 					start
	// btTransform 					end
	// float 						fraction
	// character_move_test_callback callback
	// btVector3 					sweepDirNegative
	// btVector3 					currentDir
	// ******

	// FUNCTION BODY
	// <0x585786>|0x000|0x000:'553'
	// <1>
	// <2>
	// <0x5857c7>|0x041|0x041:'556'
	// <1>
	// <0x5857ca>|0x044|0x003:'558'
	// <1>
	// <2>
	// <3>
	// <0x5857dc>|0x056|0x012:'562'
	// <0x585852>|0x0cc|0x076:'563'
	// <0x585940>|0x1ba|0x0ee:'564'
	// <1>
	// <0x585946>|0x1c0|0x006:'566'
	// <1>
	// <0x585962>|0x1dc|0x01c:'568'
	// <1>
	// <0x58596d>|0x1e7|0x00b:'570'
	// <1>
	// <2>
	// <3>
	// <0x585972>|0x1ec|0x005:'574'
	// <1>
	// <2>
	// <3>
	// <0x5859f1>|0x26b|0x07f:'578'
	// <1>
	// <0x585a18>|0x292|0x027:'580'
	// <0x585a40>|0x2ba|0x028:'581'
	// <1>
	// <0x585a42>|0x2bc|0x002:'583'
	// <1>
	// <2>
	// <0x585a71>|0x2eb|0x02f:'586'
	// <1>
	// <0x585a8a>|0x304|0x019:'588'
	// <1>
	// <2>
	// <3>
	// <4>
	// <0x585a9b>|0x315|0x011:'593'
	// <0x585ac5>|0x33f|0x02a:'594'
	// <0x585aec>|0x366|0x027:'595'
	// <0x585b12>|0x38c|0x026:'596'
	// <1>
	// <0x585b27>|0x3a1|0x015:'598'
	// <1>
	// ******
}

// STATE[STUB]
void bullet_character_controller::step_down( float dt, bool change_size_only, btVector3 const& pos_up_correction )
{
	// LOCALS
	// character_move_test_callback callback
	// btTransform 					start
	// btTransform 					finish
	// ******

	// CALL SITE INFO
	// <0x5861dc> -> float <unknown>() const
	// ******

	// FUNCTION BODY
	// <0x585cdc>|0x000|0x000:'627'
	// <1>
	// <2>
	// <0x585d1c>|0x040|0x040:'630'
	// <0x585d1f>|0x043|0x003:'631'
	// <1>
	// <0x585d43>|0x067|0x024:'633'
	// <1>
	// <0x585dcd>|0x0f1|0x08a:'635'
	// <0x585de3>|0x107|0x016:'636'
	// <1>
	// <2>
	// <3>
	// <0x585de6>|0x10a|0x003:'640'
	// <1>
	// <2>
	// <3>
	// <0x585de9>|0x10d|0x003:'644'
	// <1>
	// <2>
	// <3>
	// <0x585eca>|0x1ee|0x0e1:'648'
	// <1>
	// <0x585eef>|0x213|0x025:'650'
	// <0x585f19>|0x23d|0x02a:'651'
	// <1>
	// <0x585f1b>|0x23f|0x002:'653'
	// <1>
	// <2>
	// <0x585f44>|0x268|0x029:'656'
	// <1>
	// <0x585f5a>|0x27e|0x016:'658'
	// <0x585f6c>|0x290|0x012:'659'
	// <1>
	// <0x585fc0>|0x2e4|0x054:'661'
	// <1>
	// <0x586039>|0x35d|0x079:'663'
	// <0x5860ae>|0x3d2|0x075:'664'
	// <0x5860c2>|0x3e6|0x014:'665'
	// <0x5860c9>|0x3ed|0x007:'666'
	// <0x5860d0>|0x3f4|0x007:'667'
	// <1>
	// <0x5860d5>|0x3f9|0x005:'669'
	// <1>
	// <2>
	// <0x586188>|0x4ac|0x0b3:'672'
	// <1>
	// <2>
	// <0x58618d>|0x4b1|0x005:'675'
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
	// <17>
	// <18>
	// <19>
	// <20>
	// <21>
	// <22>
	// <23>
	// <24>
	// <25>
	// <26>
	// <27>
	// <28>
	// <29>
	// <30>
	// <31>
	// <32>
	// <33>
	// <34>
	// <35>
	// <36>
	// <37>
	// <38>
	// <39>
	// <40>
	// <41>
	// <42>
	// <43>
	// <44>
	// <0x5861a9>|0x4cd|0x01c:'720'
	// <0x5861d5>|0x4f9|0x02c:'721'
	// <0x5861e1>|0x505|0x00c:'722'
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

// STATE[STUB]
void bullet_character_controller::pre_step( float dt )
{
	BT_PROFILE("pre_step");														// <0x5856c0>|0x000|0x000:'739'
	m_pre_step_position = m_ghost_object->getWorldTransform( ).getOrigin( );	// <0x5856f7>|0x037|0x037:'740'

	s32 i = 0;
	if ( recover_from_penetration( ) > 0.05f )
		for ( ; recover_from_penetration( ) > 0.05f ; )
		{
			if ( ++i > 3 )
				break;
		}

	m_current_pos = m_ghost_object->getWorldTransform( ).getOrigin( );			// <0x58573c>|0x07c|0x015:'757'

	// FUNCTION BODY
	// <0x5856c0>|0x000|0x000:'739'
	// <0x5856f7>|0x037|0x037:'740'
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <7>
	// <0x58570f>|0x04f|0x018:'748'
	// <1>
	// <0x585726>|0x066|0x017:'750'
	// <0x585727>|0x067|0x001:'751'
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <0x58573c>|0x07c|0x015:'757'
	// <1>
	// ******
}

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

// STATE[56%|PARTIAL]: Just cannot match it
void bullet_character_controller::setup_crouch_state( bool crouch )
{
	btVector3 prev_shape_offset = m_shape_offset;												// <0x584809>|0x000|0x000:'1122'
	m_in_crouch = crouch;
    float shape_y;
    if ( crouch )
    {
    	m_current_shape_dim = m_crouch_shape_dim;
        m_shape.setImplicitShapeDimensions( btVector3( m_current_shape_dim.x * 0.5, ( m_current_shape_dim.y - m_current_shape_dim.x ) * 0.5, m_current_shape_dim.x * 0.5 ) );
        shape_y = m_crouch_shape_dim.y;
    }
    else
    {
        m_current_shape_dim = m_stand_shape_dim;
        m_shape.setImplicitShapeDimensions( btVector3( m_current_shape_dim.x * 0.5, ( m_current_shape_dim.y - m_current_shape_dim.x ) * 0.5, m_current_shape_dim.x * 0.5 ) );
        shape_y = m_stand_shape_dim.y;
    }

	m_shape_offset = btVector3( 0.0f, shape_y, 0.0f );

	btVector3 orign = m_ghost_object->getWorldTransform( ).getOrigin( ) - ( prev_shape_offset - m_shape_offset );
	m_ghost_object->getWorldTransform( ).setOrigin( orign );
	m_ghost_object->setInterpolationWorldTransform(  m_ghost_object->getWorldTransform( ) );	// <0x58499c>|0x193|0x016:'1136'

	m_ghost_object->setCollisionShape( &m_shape );												// <0x5849aa>|0x1a1|0x00e:'1138'

	if ( m_collision_world )																	// <0x5849be>|0x1b5|0x014:'1140'
		m_ghost_object->getOverlappingPairCache( )->cleanProxyFromPairs(
			m_ghost_object->getBroadphaseHandle( ),
			m_collision_world->getDispatcher( )													// <0x5849c5>|0x1bc|0x007:'1141'
		);

    /* Structure */

    // <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <0x584809>|0x000|0x000:'1122'
	// <0x584896>|0x08d|0x08d:'1123'
	// <0x58489e>|0x095|0x008:'1124'
	// <1>
	// <0x5848a0>|0x097|0x002:'1126'
	// <0x5848ac>|0x0a3|0x00c:'1127'
	// <0x5848fc>|0x0f3|0x050:'1128'
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <0x58490f>|0x106|0x013:'1134' m_shape_offset = btVector3( 0.0f, shape_y, 0.0f );
	// <0x584986>|0x17d|0x077:'1135'
	// <0x58499c>|0x193|0x016:'1136'
	// <1>
	// <0x5849aa>|0x1a1|0x00e:'1138'
	// <1>
	// <0x5849be>|0x1b5|0x014:'1140'
	// <0x5849c5>|0x1bc|0x007:'1141'
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
}

// STATE[94%|DONE]: Target also kept ecx
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