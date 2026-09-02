// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef BULLET_CHARACTER_CONTROLLER_H_INCLUDED
#define BULLET_CHARACTER_CONTROLLER_H_INCLUDED

#include <BulletDynamics/Dynamics/btActionInterface.h>
#include <BulletCollision/CollisionShapes/btCapsuleShape.h>

class btDynamicsWorld;
class btPairCachingGhostObject;

namespace vostok {
namespace physics {

class bullet_character_controller : public btActionInterface, boost::noncopyable {
public:
											bullet_character_controller		(
												btPairCachingGhostObject*	ghost_object,
												float2 const&				stand_shape_dim,
												float2 const&				crouch_shape_dim,
												s16							collisionFilterGroup,
												s16							collisionFilterMask
											);
	virtual									~bullet_character_controller	( );

	virtual	void							updateAction					( btCollisionWorld* collisionWorld, float deltaTime ) override;
	// Empty virtual override; IS instantiated (vtable slot, body = `ret 4`, ICF-folded
	// in both binaries). The empty body is faithful — not a missing reconstruction.
	virtual	void							debugDraw						( btIDebugDraw* arg_0 ) override { /* no source */ }

			void							set_desired_walk_vector			( btVector3 const& walk_vector );

			bool							on_ground						( ) const;

			void							insert							( btDynamicsWorld* world );
			void							remove							( btDynamicsWorld* world );

			btTransform						get_transform					( );
			void							set_transform					( btTransform const& transform );

			void							set_crouch						( bool crouch );
			bool							can_stand						( );
	inline	bool							has_updates						( ) const { return m_has_updates; }

			void							jump							( );
			void							end_jump						( );
			bool							can_jump						( ) const;

	// STATE[REMOVED]: no caller (the get_gravity/is_inserted grep hits are on unrelated
	// classes game_core::bullet_manager / victory_item_core); absent from both binaries.
	inline	float							get_gravity						( ) const { /* no source */ }
	inline	bool							is_inserted						( ) const { /* no source */ } // STATE[REMOVED]

private:
	inline	btPairCachingGhostObject*		get_active_ghost_object			( ) { return m_ghost_object; }

			void							player_step						( float dt );
			void							pre_step						( float dt );

	// STATE[REMOVED]: not inlined into pre_step (verified: pre_step only loops
	// recover_from_penetration); no caller; absent from both binaries.
	inline	void							prevent_max_slope_moving_prestep( const float dt ) { /* no source */ }

			float							recover_from_penetration		( );


			void							step_up							( bool change_shape_size, btVector3& pos_up_correction );
			void							step_forward_and_strafe			( btVector3 const& walkMove );
			void							step_down						( float dt, bool change_size_only, btVector3 const& pos_up_correction );

			btVector3						updateTargetPositionBasedOnCollision(
												btVector3 const&	hitNormal,
												btVector3 const&	target_pos,
												float				tangentMag = 0.0f,
												float				normalMag = 1.0f
											);
	// STATE[REMOVED]: the "most likely used in player_step" hypothesis is DISPROVEN — the
	// target player_step (0x576220) calls only step_up/step_forward_and_strafe/step_down/
	// setWorldTransform, none of these. These private inlines have no out-of-line body and no
	// inline site in any shipped function; absent from both binaries. Empty stubs correct.
	inline	bool							in_crouch						( ) const { return m_in_crouch; }
	inline	void							prevent_step_bouncing			( ) { /* no source */ } // STATE[REMOVED]
	inline	bool							can_overstep_obstacle			( btVector3 const& arg_0, btVector3 const& arg_1 ) { /* no source */ } // STATE[REMOVED]
	inline	bool							has_support_to_overstep_obstacle( ) { /* no source */ } // STATE[REMOVED]
	inline	void							updata_slide_vector				( btVector3 const& arg_0, const float arg_1 ) { /* no source */ } // STATE[REMOVED]

	inline	u32								get_contacts_count				( ) { /* no source */ } // STATE[REMOVED]

			void							setup_crouch_state				( bool crouch );
			void							setup_shape_dim					( float2 const& shape_dim );


	static const	btVector3							m_up_vector;

public:
	/* 0x0000 */	/* btActionInterface */
	/* 0x0004 */	/* boost::noncopyable */
	// target has this private too, but character_controller.cpp activate/deactivate read it directly
	/* 0x0004 */	btDynamicsWorld*					m_collision_world;
private:
	/* 0x0010 */	btVector3							m_walk_vector;
	/* 0x0020 */	btVector3							m_normalizedDirection;
	/* 0x0030 */	btVector3							m_current_pos;
	/* 0x0040 */	btVector3							m_pre_step_position;
	/* 0x0050 */	float								m_current_step_offset;
	/* 0x0060 */	btVector3							m_shape_offset;
	/* 0x0070 */	float2								m_current_shape_dim;
	/* 0x0078 */	float2								m_stand_shape_dim;
	/* 0x0080 */	float2								m_crouch_shape_dim;
	/* 0x0088 */	btPairCachingGhostObject*			m_ghost_object;
	/* 0x0090 */	btCapsuleShape						m_shape;
	/* 0x00e0 */	bool								m_in_crouch;
	/* 0x00e2 */	const s16								m_collision_filter_group;
	/* 0x00e4 */	const s16								m_collision_filter_mask;
	/* 0x00e8 */	float								m_vertical_velocity;
	/* 0x00ec */	const float							m_max_fall_speed;
	/* 0x00f0 */	const float							m_jump_speed;
	/* 0x00f4 */	const float							m_max_slope_in_radians;
	/* 0x00f8 */	const float							m_max_slope_angle_cos;
	/* 0x00fc */	const float							m_gravity;
	/* 0x0100 */	bool								m_was_on_ground;
	/* 0x0101 */	bool								m_jumping;
	/* 0x0102 */	const bool							m_useGhostObjectSweepTest;
	/* 0x0103 */	bool								m_walk_vector_applied;
	/* 0x0104 */	bool								m_on_steep_slope;
	/* 0x0105 */	bool								m_has_updates;
	/* 0x0108 */	std::list< btVector3 >				m_positions;
}; // class bullet_character_controller

STATIC_SIZE_ASSERT(bullet_character_controller, 0x110);

btVector3	computeReflectionDirection	( btVector3 const& direction, btVector3 const& normal );
btVector3	parallelComponent			( btVector3 const& direction, btVector3 const& normal );
btVector3	perpindicularComponent		( btVector3 const& direction, btVector3 const& normal );

} // namespace physics
} // namespace vostok

#endif // #ifndef BULLET_CHARACTER_CONTROLLER_H_INCLUDED
