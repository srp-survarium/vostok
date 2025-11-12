////////////////////////////////////////////////////////////////////////////
//	Created 	: 28.08.2025
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "bullet_physics_world.h"
#include <vostok/physics/bullet_utils.h>
#include <vostok/physics/collision_shapes.h>

#include <vostok/physics/engine.h>

namespace vostok {
namespace physics {

/*
// STATE[STUB]
// void `dynamic initializer for 's_physics_max_substeps_cc''()
void `dynamic initializer for 's_physics_max_substeps_cc''( )
{
}

// STATE[STUB]
// void `dynamic initializer for 's_ph_debug_cmd01''()
void `dynamic initializer for 's_ph_debug_cmd01''( )
{
}

// STATE[STUB]
// void `dynamic initializer for 's_ph_debug_cmd02''()
void `dynamic initializer for 's_ph_debug_cmd02''( )
{
}

// STATE[STUB]
// void `dynamic initializer for 's_ph_debug_cmd03''()
void `dynamic initializer for 's_ph_debug_cmd03''( )
{
}

*/

// STATE[STUB]
// void dump_physics_profiler(char const*)
void dump_physics_profiler( pcstr __formal )
{
}

// STATE[STUB]
// void reset_physics_profiler(char const*)
void reset_physics_profiler( pcstr __formal )
{
}

/*
// STATE[STUB]
// void `dynamic initializer for 's_dump_statistics''()
void `dynamic initializer for 's_dump_statistics''( )
{
}

// STATE[STUB]
// void `dynamic atexit destructor for 's_ph_debug_cmd02''()
void `dynamic atexit destructor for 's_ph_debug_cmd02''( )
{
}
*/

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

// STATE[STUB]
btTransform from_vostok( float4x4 const& m )
{
	math::quaternion q_vostok = math::quaternion(m);	// <0x6bd789>|0x000|0x000:'58'
	btQuaternion q0		= from_vostok(q_vostok);		// <0x6bd794>|0x00b|0x00b:'59'
	return btTransform(q0, from_vostok(m.c.xyz()) );	// <0x6bd7a2>|0x019|0x00e:'60'
}

// STATE[STUB]
float4x4 from_bullet( btTransform const& m )
{
	btQuaternion q_bullet	= m.getRotation();												// <0x6bd6df>|0x000|0x000:'65'
	math::quaternion q_vostok = from_bullet( q_bullet );									// <0x6bd6e9>|0x00a|0x00a:'66'
	return create_rotation(q_vostok) * create_translation( from_bullet(m.getOrigin()) );	// <0x6bd6f7>|0x018|0x00e:'67'
}

// STATE[STUB]
// vostok::physics::bullet_physics_world::bullet_physics_world(vostok::memory::base_allocator&, vostok::physics::engine&)
bullet_physics_world::bullet_physics_world( memory::base_allocator& allocator, engine& engine ):
	m_allocator		( allocator ),
	m_engine		( engine ),
	m_world_aabb	( math::create_aabb_min_max( float3( ), float3( ) ) )
{
	// LOCALS
	// memory::base_allocator&         allocator
	// ******

}

// STATE[STUB]
// void vostok::physics::log_cb(char*)
void log_cb( char* text )
{
	// FUNCTION BODY
	// <0x6bf009>|0x000|0x000:'79'
	// ******
}

// STATE[STUB]
// void vostok::physics::bullet_physics_world::initialize()
void bullet_physics_world::initialize( )
{
	// LOCALS
	// btVector3 					worldMax
	// btVector3 					worldMin
	// ******

	// CALL SITE INFO
	// <0x6bf368> -> void <unknown>(btVector3 const&)
	// <0x6bf3cd> -> btOverlappingPairCache* <unknown>()
	// <0x6bf3da> -> void <unknown>(btOverlappingPairCallback*)
	// ******

	// FUNCTION BODY
	// <0x6bf0da>|0x000|0x000:'84'
	// <1>
	// <0x6bf118>|0x03e|0x03e:'86'
	// <1>
	// <2>
	// <3>
	// <4>
	// <0x6bf120>|0x046|0x008:'91'
	// <0x6bf1d0>|0x0f6|0x0b0:'92'
	// <0x6bf1d4>|0x0fa|0x004:'93'
	// <0x6bf1dc>|0x102|0x008:'94'
	// <0x6bf1e4>|0x10a|0x008:'95'
	// <0x6bf217>|0x13d|0x033:'96'
	// <1>
	// <2>
	// <0x6bf230>|0x156|0x019:'99'
	// <0x6bf279>|0x19f|0x049:'100'
	// <0x6bf2a0>|0x1c6|0x027:'101'
	// <1>
	// <0x6bf2a5>|0x1cb|0x005:'103'
	// <0x6bf2ae>|0x1d4|0x009:'104'
	// <0x6bf2dd>|0x203|0x02f:'105'
	// <1>
	// <0x6bf2e6>|0x20c|0x009:'107'
	// <1>
	// <2>
	// <3>
	// <0x6bf302>|0x228|0x01c:'111'
	// <1>
	// <2>
	// <0x6bf331>|0x257|0x02f:'114'
	// <0x6bf36a>|0x290|0x039:'115'
	// <0x6bf38e>|0x2b4|0x024:'116'
	// <0x6bf399>|0x2bf|0x00b:'117'
	// <1>
	// <0x6bf3a5>|0x2cb|0x00c:'119'
	// <0x6bf3c2>|0x2e8|0x01d:'120'
	// <1>
	// <2>
	// <3>
	// <0x6bf3dc>|0x302|0x01a:'124'
	// <1>
	// ******
}

// STATE[STUB]
// void vostok::physics::bullet_physics_world::destroy()
void bullet_physics_world::destroy( )
{
	// CALL SITE INFO
	// <0x6bce21> -> btOverlappingPairCache* <unknown>()
	// <0x6bce2d> -> void <unknown>(btOverlappingPairCallback*)
	// <0x6bce4c> -> void* <unknown>(u32)
	// <0x6bce78> -> void* <unknown>(u32)
	// <0x6bcea4> -> void* <unknown>(u32)
	// <0x6bced0> -> void* <unknown>(u32)
	// <0x6bcefc> -> void* <unknown>(u32)
	// <0x6bcf28> -> void* <unknown>(u32)
	// ******

	// FUNCTION BODY
	// <1>
	// <0x6bce15>|0x000|0x000:'131'
	// <0x6bce2f>|0x01a|0x01a:'132'
	// <0x6bce5b>|0x046|0x02c:'133'
	// <0x6bce87>|0x072|0x02c:'134'
	// <0x6bceb3>|0x09e|0x02c:'135'
	// <0x6bcedf>|0x0ca|0x02c:'136'
	// <0x6bcf0b>|0x0f6|0x02c:'137'
	// <0x6bcf37>|0x122|0x02c:'138'
	// <1>
	// <2>
	// <0x6bcf43>|0x12e|0x00c:'141'
	// <0x6bcf4f>|0x13a|0x00c:'142'
	// ******
}

// STATE[STUB]
// void vostok::physics::bullet_physics_world::set_renderer(btIDebugDraw* const)
void bullet_physics_world::set_renderer( btIDebugDraw* renderer )
{
	// CALL SITE INFO
	// <0x6bc8a8> -> void <unknown>(btIDebugDraw*)
	// ******

}

// STATE[STUB]
// void vostok::physics::bullet_physics_world::draw_object(btCollisionShape* const, btTransform const&, btVector3 const&)
void bullet_physics_world::draw_object( btCollisionShape* shape, btTransform const& transform, btVector3 const& color )
{
	// CALL SITE INFO
	// <0x6bc897> -> void <unknown>(btTransform const&, btCollisionShape const*, btVector3 const&)
	// ******

	// FUNCTION BODY
	// <0x6bc880>|0x000|0x000:'152'
	// ******
}

// STATE[STUB]
// void vostok::physics::bullet_physics_world::on_before_reuse()
void bullet_physics_world::on_before_reuse( )
{
	// FUNCTION BODY
	// <0x6bc870>|0x000|0x000:'157'
	// <0x6bc878>|0x008|0x008:'158'
	// ******
}

// STATE[STUB]
// void vostok::physics::bullet_physics_world::tick(const unsigned int)
void bullet_physics_world::tick( u32 current_time_in_ms )
{
	// CALL SITE INFO
	// <0x6bdef8> -> int <unknown>(float, int, float)
	// ******

	// FUNCTION BODY
	// <1>
	// <0x6bde70>|0x000|0x000:'164'
	// <1>
	// <2>
	// <3>
	// <4>
	// <0x6bde8b>|0x01b|0x01b:'169'
	// <1>
	// <2>
	// <0x6bded0>|0x060|0x045:'172'
	// <1>
	// <2>
	// <3>
	// <0x6bdefa>|0x08a|0x02a:'176'
	// <1>
	// ******
}

// STATE[STUB]
// void vostok::physics::bullet_physics_world::debug_draw_world()
void bullet_physics_world::debug_draw_world( )
{
	// LOCALS
	// btVector3[3] 				object_colors
	// ******

	// CALL SITE INFO
	// <0x6bc993> -> void <unknown>(btTransform const&, btCollisionShape const*, btVector3 const&)
	// ******

	// FUNCTION BODY
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
	// <0x6bc8bd>|0x000|0x000:'195'
	// <0x6bc8c4>|0x007|0x007:'196'
	// <1>
	// <0x6bc8e0>|0x023|0x01c:'198'
	// <1>
	// <2>
	// <0x6bc8e6>|0x029|0x006:'201'
	// <0x6bc8fe>|0x041|0x018:'202'
	// <1>
	// <0x6bc916>|0x059|0x018:'204'
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
	// <0x6bc92e>|0x071|0x018:'223'
	// <1>
	// <0x6bc938>|0x07b|0x00a:'225'
	// <1>
	// <2>
	// <0x6bc93b>|0x07e|0x003:'228'
	// <0x6bc949>|0x08c|0x00e:'229'
	// <1>
	// <0x6bc94b>|0x08e|0x002:'231'
	// <0x6bc959>|0x09c|0x00e:'232'
	// <1>
	// <2>
	// <3>
	// <0x6bc95e>|0x0a1|0x005:'236'
	// <0x6bc96c>|0x0af|0x00e:'237'
	// <1>
	// <0x6bc971>|0x0b4|0x005:'239'
	// <1>
	// <2>
	// <0x6bc978>|0x0bb|0x007:'242'
	// <1>
	// <2>
	// ******
}

// STATE[STUB]
// void vostok::physics::bullet_physics_world::add(vostok::physics::bt_rigid_body_base*, unsigned short, unsigned short)
void bullet_physics_world::add( bt_rigid_body_base* body, u16 filter_group, u16 filter_mask )
{
	// LOCALS
	// btTransform 					trans
	// btVector3 					minAabb
	// btVector3 					maxAabb
	// ******

	// CALL SITE INFO
	// <0x6bd1a9> -> btRigidBody* <unknown>()
	// <0x6bd1b3> -> void <unknown>(btRigidBody*, short, short)
	// <0x6bd1bc> -> btRigidBody* <unknown>()
	// <0x6bd220> -> btRigidBody* <unknown>()
	// <0x6bd23c> -> void <unknown>(btTransform const&, btVector3&, btVector3&) const
	// ******

	// FUNCTION BODY
	// <0x6bd18b>|0x000|0x000:'249'
	// <1>
	// <0x6bd1b5>|0x02a|0x02a:'251'
	// <1>
	// <2>
	// <3>
	// <0x6bd213>|0x088|0x05e:'255'
	// <0x6bd23e>|0x0b3|0x02b:'256'
	// <0x6bd277>|0x0ec|0x039:'257'
	// ******
}

// STATE[STUB]
// void vostok::physics::bullet_physics_world::remove(vostok::physics::bt_rigid_body_base*)
void bullet_physics_world::remove( bt_rigid_body_base* body )
{
	// CALL SITE INFO
	// <0x6bc852> -> btRigidBody* <unknown>()
	// <0x6bc85b> -> void <unknown>(btRigidBody*)
	// ******

	// FUNCTION BODY
	// <0x6bc843>|0x000|0x000:'262'
	// ******
}

// STATE[STUB]
// void vostok::physics::bullet_physics_world::move(vostok::physics::bt_rigid_body_base*, vostok::math::float4x4 const&)
void bullet_physics_world::move( bt_rigid_body_base* body, float4x4 const& new_transform )
{
	// CALL SITE INFO
	// <0x6bc82e> -> void <unknown>(float4x4 const&)
	// ******

	// FUNCTION BODY
	// <0x6bc820>|0x000|0x000:'267'
	// ******
}

// STATE[STUB]
// void vostok::physics::bullet_physics_world::add(vostok::physics::bt_soft_body_rope*)
void bullet_physics_world::add( bt_soft_body_rope* body )
{
	// FUNCTION BODY
	// <0x6bd160>|0x000|0x000:'272'
	// ******
}

// STATE[STUB]
// void vostok::physics::bullet_physics_world::remove(vostok::physics::bt_soft_body_rope*)
void bullet_physics_world::remove( bt_soft_body_rope* body )
{
	// FUNCTION BODY
	// <0x6bcde0>|0x000|0x000:'277'
	// ******
}

// STATE[STUB]
// void vostok::physics::bullet_physics_world::add(vostok::physics::bt_constraint*)
void bullet_physics_world::add( bt_constraint* constraint )
{
	// CALL SITE INFO
	// <0x6bc812> -> void <unknown>(btTypedConstraint*, bool)
	// ******

	// FUNCTION BODY
	// <0x6bc800>|0x000|0x000:'282'
	// ******
}

// STATE[STUB]
// void vostok::physics::bullet_physics_world::remove(vostok::physics::bt_constraint*)
void bullet_physics_world::remove( bt_constraint* constraint )
{
	// CALL SITE INFO
	// <0x6bc7f3> -> void <unknown>(btTypedConstraint*)
	// ******

}

// STATE[STUB]
// void vostok::physics::bullet_physics_world::create_test_scene()
void bullet_physics_world::create_test_scene( )
{
	// FUNCTION BODY
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
	// ******
}

struct closest_ray_result_callback : btCollisionWorld::RayResultCallback {
public:
						closest_ray_result_callback	( btVector3 const& rayFromWorld, btVector3 const& rayToWorld );

	virtual	float		addSingleResult				( btCollisionWorld::LocalRayResult& rayResult, bool normalInWorldSpace ) override;

private:
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
// vostok::physics::closest_ray_result_callback::closest_ray_result_callback(btVector3 const&, btVector3 const&)
closest_ray_result_callback::closest_ray_result_callback( btVector3 const& rayFromWorld, btVector3 const& rayToWorld )
{
}

// STATE[STUB]
// float vostok::physics::closest_ray_result_callback::addSingleResult(btCollisionWorld::LocalRayResult&, bool)
float closest_ray_result_callback::addSingleResult( btCollisionWorld::LocalRayResult& rayResult, bool normalInWorldSpace )
{
	return 0.0f;
	// FUNCTION BODY
	// <1>
	// <2>
	// <3>
	// <0x363e6>|0x000|0x000:'344'
	// <0x363f2>|0x00c|0x00c:'345'
	// <1>
	// <0x363f7>|0x011|0x005:'347'
	// <1>
	// <0x363fd>|0x017|0x006:'349'
	// <0x36406>|0x020|0x009:'350'
	// <0x3640f>|0x029|0x009:'351'
	// <1>
	// <0x36411>|0x02b|0x002:'353'
	// <0x36418>|0x032|0x007:'354'
	// <1>
	// <2>
	// <0x3641c>|0x036|0x004:'357'
	// <1>
	// <0x36422>|0x03c|0x006:'359'
	// <0x36431>|0x04b|0x00f:'360'
	// <1>
	// <2>
	// <0x36436>|0x050|0x005:'363'
	// <1>
	// <0x364e2>|0x0fc|0x0ac:'365'
	// <0x36543>|0x15d|0x061:'366'
	// ******
}

// STATE[STUB]
// vostok::physics::closest_ray_result vostok::physics::bullet_physics_world::ray_test(vostok::math::float3 const&, vostok::math::float3 const&, const float, unsigned short, unsigned short)
closest_ray_result bullet_physics_world::ray_test(
	float3 const&		ray_from,
	float3 const&		ray_dir,
	float				ray_length,
	u16					filter_group,
	u16					filter_mask
)
{
	return closest_ray_result();
	// LOCALS
	// closest_ray_result_callback 	cb
	// btVector3 					from
	// btVector3 					to
	// ******

	// CALL SITE INFO
	// <0x6bcd05> -> void <unknown>(btVector3 const&, btVector3 const&, btCollisionWorld::RayResultCallback&) const
	// ******

	// FUNCTION BODY
	// <0x6bcc3c>|0x000|0x000:'372'
	// <0x6bcc55>|0x019|0x019:'373'
	// <1>
	// <0x6bcc93>|0x057|0x03e:'375'
	// <1>
	// <2>
	// <3>
	// <4>
	// <0x6bccd7>|0x09b|0x044:'380'
	// <1>
	// <0x6bcce9>|0x0ad|0x012:'382'
	// <1>
	// <0x6bcd07>|0x0cb|0x01e:'384'
	// <0x6bcd0a>|0x0ce|0x003:'385'
	// <1>
	// <0x6bcd2f>|0x0f3|0x025:'387'
	// <0x6bcd41>|0x105|0x012:'388'
	// <1>
	// <2>
	// <0x6bcd7c>|0x140|0x03b:'391'
	// <0x6bcdc8>|0x18c|0x04c:'392'
	// <1>
	// <2>
	// ******
}

// STATE[STUB]
// bool vostok::physics::bullet_physics_world::recover_from_penetrations(vostok::physics::bt_collision_shape* const, vostok::math::float4x4 const&, vostok::math::float4x4&, unsigned short, unsigned short)
bool bullet_physics_world::recover_from_penetrations(
	bt_collision_shape*		shape,
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
// void vostok::physics::bullet_physics_world::object_query(vostok::physics::bt_collision_shape* const, vostok::math::float4x4 const&, vostok::math::float4x4 const&, vostok::vectora<vostok::physics::closest_ray_result>&, unsigned short, unsigned short)
void bullet_physics_world::object_query(
	bt_collision_shape*				shape,
	float4x4 const&					transform_from,
	float4x4 const&					transform_to,
	vectora<closest_ray_result>&	results,
	u16								filter_group,
	u16								filter_mask
)
{
	// LOCALS
	// bullet_physics_world::object_query::__l2::object_query_callback resultCallback
	// btTransform 					t1
	// btTransform 					t2
	// ******

	// TYPEDEFS
	// typedef
	// 	bullet_physics_world::object_query::__l2::object_query_callback
	// 	bullet_physics_world::object_query::__l2::object_query_callback;

	// ******

	// FUNCTION BODY
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
	// <0x6bdf1e>|0x000|0x000:'517'
	// <0x6bdf3b>|0x01d|0x01d:'518'
	// <0x6bdf47>|0x029|0x00c:'519'
	// <1>
	// <0x6bdf64>|0x046|0x01d:'521'
	// <1>
	// <0x6be021>|0x103|0x0bd:'523'
	// <1>
	// <0x6be029>|0x10b|0x008:'525'
	// <0x6be02b>|0x10d|0x002:'526'
	// <0x6be030>|0x112|0x005:'527'
	// <1>
	// <2>
	// <0x6be039>|0x11b|0x009:'530'
	// <1>
	// <2>
	// <0x6be03f>|0x121|0x006:'533'
	// <1>
	// <0x6be051>|0x133|0x012:'535'
	// <0x6be443>|0x525|0x3f2:'536'
	// <1>
	// <2>
	// <3>
	// <0x6be864>|0x946|0x421:'540'
	// <1>
	// <2>
	// <3>
	// <0x6be82f>|0x911|-0x035:'544'
	// ******
}
/*
// STATE[STUB]
// `vostok::physics::bullet_physics_world::object_query'::`2'::object_query_callback::object_query_callback(vostok::vectora<vostok::physics::closest_ray_result>&, const unsigned short, const unsigned short)
`vostok::physics::bullet_physics_world::object_query'::`2'::object_query_callback::object_query_callback( vostok::vectora<vostok::physics::closest_ray_result>& results, u16 group, u16 mask )
{
	// FUNCTION BODY
	// <1>
	// <2>
	// <0x6bd3b0>|0x000|0x000:'488'
	// ******
}

// STATE[STUB]
// float `vostok::physics::bullet_physics_world::object_query'::`2'::object_query_callback::addSingleResult(btCollisionWorld::LocalConvexResult&, bool)
float `vostok::physics::bullet_physics_world::object_query'::`2'::object_query_callback::addSingleResult( btCollisionWorld::LocalConvexResult& convexResult, bool normalInWorldSpace )
{
	// LOCALS
	// btVector3 					hitNormalWorld
	// vostok::physics::closest_ray_result query_result
	// ******

	return 0.0f;
	// FUNCTION BODY
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
	// <0x6bcf69>|0x000|0x000:'503'
	// <1>
	// <2>
	// <3>
	// <4>
	// <0x6bd0e8>|0x17f|0x17f:'508'
	// <1>
	// <0x6bd14b>|0x1e2|0x063:'510'
	// ******
}
*/

// STATE[STUB]
// void vostok::physics::bullet_physics_world::ray_query(vostok::math::float3 const&, vostok::math::float3 const&, const float, vostok::vectora<vostok::physics::closest_ray_result>&, unsigned short, unsigned short)
void bullet_physics_world::ray_query(
	float3 const&					ray_from,
	float3 const&					ray_dir,
	float							ray_length,
	vectora<closest_ray_result>&	results,
	u16								filter_group,
	u16								filter_mask
)
{
	// LOCALS
	// btCollisionWorld::AllHitsRayResultCallback cb
	// btVector3 					from
	// btVector3 					to
	// s32 							size
	// closest_ray_result 			ray_result
	// ******

	// CALL SITE INFO
	// <0x6bd521> -> void <unknown>(btVector3 const&, btVector3 const&, btCollisionWorld::RayResultCallback&) const
	// ******

	// FUNCTION BODY
	// <0x6bd451>|0x000|0x000:'560'
	// <0x6bd44c>|-0x005|-0x005:'561'
	// <0x6bd44f>|-0x002|0x003:'562'
	// <0x6bd4eb>|0x09a|0x09c:'563'
	// <0x6bd4ef>|0x09e|0x004:'564'
	// <1>
	// <2>
	// <0x6bd4f3>|0x0a2|0x004:'567'
	// <1>
	// <0x6bd503>|0x0b2|0x010:'569'
	// <1>
	// <0x6bd523>|0x0d2|0x020:'571'
	// <1>
	// <0x6bd531>|0x0e0|0x00e:'573'
	// <0x6bd538>|0x0e7|0x007:'574'
	// <1>
	// <2>
	// <0x6bd561>|0x110|0x029:'577'
	// <1>
	// <2>
	// <3>
	// <4>
	// <0x6bd591>|0x140|0x030:'582'
	// <0x6bd608>|0x1b7|0x077:'583'
	// <1>
	// <2>
	// <3>
	// <0x6bd693>|0x242|0x08b:'587'
	// <1>
	// ******
}

// STATE[STUB]
// vostok::physics::primitive_type vostok::physics::from_bullet_shape_type(int)
collision::primitive_type from_bullet_shape_type( s32 type )
{
	return collision::primitive_box;
	// STATICS
	// static <NoType> 				 = <0x6bc7b8>;
	// static <NoType> 				 = <0x6bc7a4>;
	// ******

	// OTHER SYMBOLS
	// Label(LabelSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x6ab78e }, flags: ProcedureFlags { nofpo: false, int: false, far: false, never: false, notreached: false, cust_call: false, noinline: false, optdbginfo: false }, name: RawString("$LN5") })
	// Label(LabelSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x6ab794 }, flags: ProcedureFlags { nofpo: false, int: false, far: false, never: false, notreached: false, cust_call: false, noinline: false, optdbginfo: false }, name: RawString("$LN4") })
	// Label(LabelSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x6ab797 }, flags: ProcedureFlags { nofpo: false, int: false, far: false, never: false, notreached: false, cust_call: false, noinline: false, optdbginfo: false }, name: RawString("$LN3") })
	// Label(LabelSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x6ab79d }, flags: ProcedureFlags { nofpo: false, int: false, far: false, never: false, notreached: false, cust_call: false, noinline: false, optdbginfo: false }, name: RawString("$LN2") })
	// ******

	// FUNCTION BODY
	// <0x6bc780>|0x000|0x000:'654'
	// <1>
	// <0x6bc78e>|0x00e|0x00e:'656'
	// <0x6bc794>|0x014|0x006:'657'
	// <0x6bc797>|0x017|0x003:'658'
	// <0x6bc79d>|0x01d|0x006:'659'
	// <1>
	// <2>
	// ******
}

// STATE[STUB]
// vostok::math::float3 vostok::physics::dimensions_from_bullet_shape(btCollisionShape const*)
float3 dimensions_from_bullet_shape( btCollisionShape const* bullet_shape )
{
	// LOCALS
	// float3 						half_extents
	// float3 						radius
	// float3 						half_extents
	// float3 						dimensions
	// ******

	// STATICS
	// static <NoType> 				 = <0x6bcc18>;
	// static <NoType> 				 = <0x6bcc04>;
	// ******

	// OTHER SYMBOLS
	// Label(LabelSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x6abb24 }, flags: ProcedureFlags { nofpo: false, int: false, far: false, never: false, notreached: false, cust_call: false, noinline: false, optdbginfo: false }, name: RawString("$LN5") })
	// Label(LabelSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x6abb4a }, flags: ProcedureFlags { nofpo: false, int: false, far: false, never: false, notreached: false, cust_call: false, noinline: false, optdbginfo: false }, name: RawString("$LN4") })
	// Label(LabelSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x6abb7e }, flags: ProcedureFlags { nofpo: false, int: false, far: false, never: false, notreached: false, cust_call: false, noinline: false, optdbginfo: false }, name: RawString("$LN3") })
	// Label(LabelSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x6abbbb }, flags: ProcedureFlags { nofpo: false, int: false, far: false, never: false, notreached: false, cust_call: false, noinline: false, optdbginfo: false }, name: RawString("$LN2") })
	// ******

	return vostok::math::float3(1., 1., 1.);
	// FUNCTION BODY
	// <0x6bcb10>|0x000|0x000:'667'
	// <1>
	// <2>
	// <3>
	// <4>
	// <0x6bcb24>|0x014|0x014:'672'
	// <0x6bcb45>|0x035|0x021:'673'
	// <1>
	// <2>
	// <3>
	// <4>
	// <0x6bcb4a>|0x03a|0x005:'678'
	// <0x6bcb62>|0x052|0x018:'679'
	// <1>
	// <2>
	// <3>
	// <4>
	// <0x6bcb7e>|0x06e|0x01c:'684'
	// <0x6bcba5>|0x095|0x027:'685'
	// <1>
	// <2>
	// <3>
	// <4>
	// <0x6bcbbb>|0x0ab|0x016:'690'
	// <0x6bcb78>|0x068|-0x043:'691'
	// <1>
	// <2>
	// <3>
	// ******
}

// STATE[STUB]
// float vostok::physics::contact_result_callback::addSingleResult(btManifoldPoint&, btCollisionObject const*, int, int, btCollisionObject const*, int, int)
float contact_result_callback::addSingleResult(
	btManifoldPoint&			__formal1,
	btCollisionObject const*	colObj0,
	s32							__formal2,
	s32							__formal3,
	btCollisionObject const*	colObj1,
	s32							__formal4,
	s32							__formal5
)
{
	// LOCALS
	// float4x4 					shape_1_transform
	// float3 						shape_0_dim
	// float4x4 					shape_0_transform
	// float3 						shape_1_dim
	// ******

	// CALL SITE INFO
	// <0x3661b> -> float <unknown>(void*, primitive_type, float4x4 const&, float3 const&, primitive_type, float4x4 const&, float3 const&)
	// ******

	return 0.0f;
	// FUNCTION BODY
	// <0x3657d>|0x000|0x000:'707'
	// <0x3658f>|0x012|0x012:'708'
	// <0x3659f>|0x022|0x010:'709'
	// <1>
	// <0x365ae>|0x031|0x00f:'711'
	// <0x365be>|0x041|0x010:'712'
	// <0x365ca>|0x04d|0x00c:'713'
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <7>
	// <8>
	// <9>
	// <0x365d9>|0x05c|0x00f:'723'
	// ******
}

// STATE[STUB]
// void vostok::physics::bullet_physics_world::contact_pair_test(vostok::physics::contact_test_predicate&, btCollisionObject*, btCollisionObject*)
void bullet_physics_world::contact_pair_test( contact_test_predicate& predicate, btCollisionObject* first_object, btCollisionObject* second_object )
{
	// LOCALS
	// contact_result_callback 		cb
	// ******

	// FUNCTION BODY
	// <0x6bd373>|0x000|0x000:'732'
	// <0x6bd380>|0x00d|0x00d:'733'
	// ******
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

// STATE[STUB]
// void vostok::physics::bullet_physics_world::notify_about_contact()
void bullet_physics_world::notify_about_contact( )
{
	// LOCALS
	// s32 							num_manifolds
	// s32 							i
	// base_physics_object* 		base_obj_a
	// base_physics_object* 		base_obj_b
	// ******

	// CALL SITE INFO
	// <0x6bdcfa> -> int <unknown>() const
	// <0x6bdd1c> -> btPersistentManifold* <unknown>(int)
	// <0x6bde3a> -> <unknown>
	// ******

	// FUNCTION BODY
	// <0x6bdced>|0x000|0x000:'826'
	// <0x6bdcfc>|0x00f|0x00f:'827'
	// <1>
	// <0x6bdd13>|0x026|0x017:'829'
	// <0x6bdd1e>|0x031|0x00b:'830'
	// <0x6bdd24>|0x037|0x006:'831'
	// <1>
	// <2>
	// <0x6bdd34>|0x047|0x010:'834'
	// <1>
	// <2>
	// <3>
	// <4>
	// <0x6bdd4a>|0x05d|0x016:'839'
	// <0x6bdd50>|0x063|0x006:'840'
	// <1>
	// <2>
	// <3>
	// <0x6bdd62>|0x075|0x012:'844'
	// <1>
	// <0x6bddb3>|0x0c6|0x051:'846'
	// <0x6bddd4>|0x0e7|0x021:'847'
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// ******
}

// STATE[STUB]
// void vostok::physics::bullet_physics_world::subscribe_on_contact(vostok::physics::base_physics_object*, boost::function<void __cdecl(vostok::physics::base_physics_object *,vostok::physics::base_physics_object *,vostok::math::float3 const &)>*)
void bullet_physics_world::subscribe_on_contact( base_physics_object* object, boost::function<void(base_physics_object *,base_physics_object *,float3 const &)>* callback )
{
	// LOCALS
	// std::pair<std::priv::_Rb_tree_iterator<std::pair<base_physics_object * const,boost::function<void(base_physics_object *,base_physics_object *,float3 const &)> *>,std::priv::_MultimapTraitsT<std::pair<base_physics_object * const,boost::function<void(base_physics_object *,base_physics_object *,float3 const &)> *> > >,std::priv::_Rb_tree_iterator<std::pair<base_physics_object * const,boost::function<void(base_physics_object *,base_physics_object *,float3 const &)> *>,std::priv::_MultimapTraitsT<std::pair<base_physics_object * const,boost::function<void(base_physics_object *,base_physics_object *,float3 const &)> *> > > > ret
	// ******

	// TYPEDEFS
	// typedef
	// 	std::priv::_Rb_tree_iterator<std::pair<base_physics_object * const,boost::function<void(base_physics_object *,base_physics_object *,float3 const &)> *>,std::priv::_MultimapTraitsT<std::pair<base_physics_object * const,boost::function<void(base_physics_object *,base_physics_object *,float3 const &)> *> > >
	// 	callbacks_iterator;

	// ******

	// FUNCTION BODY
	// <1>
	// <0x6bca8b>|0x000|0x000:'858'
	// <1>
	// <2>
	// <0x6bca9d>|0x012|0x012:'861'
	// <1>
	// <2>
	// <0x6bcac2>|0x037|0x025:'864'
	// ******
}

// STATE[STUB]
// void vostok::physics::bullet_physics_world::unsubscribe_from_contact(vostok::physics::base_physics_object*, boost::function<void __cdecl(vostok::physics::base_physics_object *,vostok::physics::base_physics_object *,vostok::math::float3 const &)>*)
void bullet_physics_world::unsubscribe_from_contact( base_physics_object* object, boost::function<void(base_physics_object *,base_physics_object *,float3 const &)>* callback )
{
	// LOCALS
	// std::pair<std::priv::_Rb_tree_iterator<std::pair<base_physics_object * const,boost::function<void(base_physics_object *,base_physics_object *,float3 const &)> *>,std::priv::_MultimapTraitsT<std::pair<base_physics_object * const,boost::function<void(base_physics_object *,base_physics_object *,float3 const &)> *> > >,std::priv::_Rb_tree_iterator<std::pair<base_physics_object * const,boost::function<void(base_physics_object *,base_physics_object *,float3 const &)> *>,std::priv::_MultimapTraitsT<std::pair<base_physics_object * const,boost::function<void(base_physics_object *,base_physics_object *,float3 const &)> *> > > > ret
	// ******

	// TYPEDEFS
	// typedef
	// 	std::priv::_Rb_tree_iterator<std::pair<base_physics_object * const,boost::function<void(base_physics_object *,base_physics_object *,float3 const &)> *>,std::priv::_MultimapTraitsT<std::pair<base_physics_object * const,boost::function<void(base_physics_object *,base_physics_object *,float3 const &)> *> > >
	// 	callbacks_iterator;

	// ******

	// FUNCTION BODY
	// <1>
	// <0x6bca0c>|0x000|0x000:'870'
	// <1>
	// <2>
	// <0x6bca1e>|0x012|0x012:'873'
	// <1>
	// <0x6bca31>|0x025|0x013:'875'
	// <1>
	// <0x6bca4d>|0x041|0x01c:'877'
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <0x6bca4f>|0x043|0x002:'883'
	// ******
}

} // namespace physics
} // namespace vostok