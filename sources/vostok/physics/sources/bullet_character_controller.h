////////////////////////////////////////////////////////////////////////////
//	Created 	: 28.08.2025
////////////////////////////////////////////////////////////////////////////

#ifndef BULLET_CHARACTER_CONTROLLER_H_INCLUDED
#define BULLET_CHARACTER_CONTROLLER_H_INCLUDED

#include <BulletDynamics/Dynamics/btActionInterface.h>
#include <BulletCollision/CollisionShapes/btCapsuleShape.h>

namespace vostok {
namespace physics {

class btPairCachingGhostObject;
class btDynamicsWorld;


/*
// STATE[STUB]
// void* vostok::physics::character_move_test_callback::`scalar deleting destructor'(unsigned int)
void* character_move_test_callback::`scalar deleting destructor'( )
{
}
*/

class bullet_character_controller : btActionInterface, boost::noncopyable {
public:
	bullet_character_controller( btPairCachingGhostObject* ghost_object, float2 const& stand_shape_dim, float2 const& crouch_shape_dim );
	virtual ~bullet_character_controller( );

	virtual void			updateAction				( btCollisionWorld* collisionWorld, float deltaTime ) override;
	virtual void			debugDraw					( btIDebugDraw* debugDrawer ) /* no source */ override;

	void					set_desired_walk_vector		( btVector3 const& walk_vector );

	bool					on_ground					( ) const;
	void					insert						( btDynamicsWorld* world );
	void					remove						( btDynamicsWorld* world );

	btTransform				get_transform				( );
	void					set_transform				( btTransform const& transform );

	void					set_crouch					( bool crouch );
	bool					can_stand					( );
	bool					has_updates					( ) const /* no source */;

	void					jump						( );
	void					end_jump					( );
	bool					can_jump					( ) const;

	float					get_gravity					( ) const /* no source */;
	bool					is_inserted					( ) const /* no source */;

	btPairCachingGhostObject* get_active_ghost_object	( ) /* no source */;

	void					player_step					( float dt );
	void					pre_step					( float dt );

	void					prevent_max_slope_moving_prestep	(float) /* no source */;
	float					recover_from_penetration			( );

	void					step_up						( bool change_shape_size, btVector3& pos_up_correction );
	void					step_forward_and_strafe		( btVector3 const& walkMove );
	void					step_down					( float dt, bool change_size_only, btVector3 const& pos_up_correction );

	btVector3				updateTargetPositionBasedOnCollision( btVector3 const& hitNormal, btVector3 const& target_pos, float __formal, float normalMag );

	bool					in_crouch							( ) const /* no source */;
	void					prevent_step_bouncing				( ) /* no source */;
	bool					can_overstep_obstacle				( btVector3 const&, btVector3 const& ) /* no source */;
	bool					has_support_to_overstep_obstacle	() /* no source */;
	void					updata_slide_vector					( btVector3 const&, float ) /* no source */;

	u32						get_contacts_count			( ) /* no source */;

	void					setup_crouch_state			( bool crouch );
	void					setup_shape_dim				( float2 const& shape_dim );

private:
	/* offset 0x0000 */ /* fields for btActionInterface */
	/* offset 0x0004 */ /* fields for boost::noncopyable */
	/* offset 0x0004 */ btDynamicsWorld*                    m_collision_world;
	/* offset 0x0010 */ btVector3                           m_walk_vector;
	/* offset 0x0020 */ btVector3                           m_normalizedDirection;
	/* offset 0x0030 */ btVector3                           m_current_pos;
	/* offset 0x0040 */ btVector3                           m_pre_step_position;
	/* offset 0x0050 */ float                               m_current_step_offset;
	/* offset 0x0060 */ btVector3                           m_shape_offset;
	/* offset 0x0070 */ float2                              m_current_shape_dim;
	/* offset 0x0078 */ float2                              m_stand_shape_dim;
	/* offset 0x0080 */ float2                              m_crouch_shape_dim;
	/* offset 0x0088 */ btPairCachingGhostObject*           m_ghost_object;
	/* offset 0x0090 */ btCapsuleShape                      m_shape;
	/* offset 0x00e0 */ bool                                m_in_crouch;
	/* offset 0x00e2 */ s16                                 m_collision_filter_group;
	/* offset 0x00e4 */ s16                                 m_collision_filter_mask;
	/* offset 0x00e8 */ float                               m_vertical_velocity;
	/* offset 0x00ec */ float                               m_max_fall_speed;
	/* offset 0x00f0 */ float                               m_jump_speed;
	/* offset 0x00f4 */ float                               m_max_slope_in_radians;
	/* offset 0x00f8 */ float                               m_max_slope_angle_cos;
	/* offset 0x00fc */ float                               m_gravity;
	/* offset 0x0100 */ bool                                m_was_on_ground;
	/* offset 0x0101 */ bool                                m_jumping;
	/* offset 0x0102 */ bool                                m_useGhostObjectSweepTest;
	/* offset 0x0103 */ bool                                m_walk_vector_applied;
	/* offset 0x0104 */ bool                                m_on_steep_slope;
	/* offset 0x0105 */ bool                                m_has_updates;
	/* offset 0x0108 */ std::list<btVector3,std::allocator<btVector3> >  m_positions;
}; // class bullet_character_controller

namespace {
	typedef char size_assert[
		sizeof(bullet_character_controller) == 0x110 ? 1 : -1
	];
}

} // namespace physics
} // namespace vostok

#endif // #ifndef BULLET_CHARACTER_CONTROLLER_H_INCLUDED
