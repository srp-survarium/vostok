## Physics
If you got close in matching, but the loads of vectors are out of order, this most likely means something got inlined.

## Matching dept


### Overview

`btVector3::lerp` vs `btVector3::setInterpolate3` - The second one builds in-place and was used by VG.
Their impl is also slightly different, so you can figure out which one is used based on that also.


### Modules
* animated_model_instance_cook.cpp
* animated_rigid_body.cpp
* bullet_character_controller.cpp
* bullet_physics_world.cpp
* character_controller.cpp
* collision_shape_cook.cpp
* collision_shapes.cpp
* physics_entry_point.cpp

### Modules (Done)
* ghost_object.cpp
* rigid_body_construction_info.cpp
* static_rigid_body.cpp


// FUNCTION BODY
// <1>
// <0x6bd80c>|0x000|0x000:'784' static console_commands::cc_bool s_ik_change_foot_rotation_cc
// <1>
// <0x6bd85c>|0x050|0x050:'786' btCapsuleShape collision_shape( half_size.x, half_size.y )
// <0x6bd886>|0x07a|0x02a:'787' btQuaternion q = from_vostok( transform ).getRotation( );
// <0x6bd8f7>|0x0eb|0x071:'788' btVector3 btStart = from_vostok( start );
// <0x6bd928>|0x11c|0x031:'789' btVector3 btEnd	= from_vostok( finish );
// <1>
// <0x6bd94b>|0x13f|0x023:'791' btCollisionWorld::ClosestConvexResultCallback callback( btStart, btEnd );
// <1>
// <0x6bd968>|0x15c|0x01d:'793' callback.m_collisionFilterMask = 0x2;
// <0x6bd98f>|0x183|0x027:'794' m_dynamicsWorld->convexSweepTest( &collisi
// <1>
// <0x6bda23>|0x217|0x094:'796' if ( callback.hasHit( ) )
// <1>							{
// <0x6bda3d>|0x231|0x01a:'798'		if ( math::abs( callback.m_closestHitFraction ) >= math::epsilon_5 )
// <1>								{
// <2>									btVector3 result;
// <0x6bda64>|0x258|0x027:'801'			result.setInterpolate3( btStart, btEnd, callback.m_closestHitFraction );
// <0x6bda70>|0x264|0x00c:'802'
// <0x6bdae2>|0x2d6|0x072:'803'			if ( s_ik_change_foot_rotation_value )
// <1>									{
// <0x6bdafb>|0x2ef|0x019:'805'				float3 normal = from_bullet( callback.m_hitNormalWorld );
// <0x6bdb0c>|0x300|0x011:'806'				float value = normal.dot_product( -transform.k.xyz( ) );
// <0x6bdb45>|0x339|0x039:'807'				math::clamp( value, -1.0f, 1.0f )
// <0x6bdb73>|0x367|0x02e:'808'				float angle = math::acos( value ) * rotation_koef0;
// <0x6bdb83>|0x377|0x010:'809'				if ( math::abs( angle ) >= math::epsilon_5 )
// <1>										{
// <0x6bdbae>|0x3a2|0x02b:'811'
// <0x6bdc2f>|0x423|0x081:'812'
// <0x6bdc46>|0x43a|0x017:'813'
// <1>										}
// <2>									}
// <0x6bdc87>|0x47b|0x041:'816'
// <1>								}
// <2>							}
// <0x6bdcb8>|0x4ac|0x031:'819' return callback.hasHit( );
// <1>
// <2>
// ******
