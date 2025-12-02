////////////////////////////////////////////////////////////////////////////
//	Created 	: 05.10.2025
////////////////////////////////////////////////////////////////////////////

#ifndef VOSTOK_PHYSICS_CONTACT_TEST_PREDICATE_H_INCLUDED
#define VOSTOK_PHYSICS_CONTACT_TEST_PREDICATE_H_INCLUDED

#include <vostok/collision/primitives.h>
#include <vostok/physics/sources/bullet_include.h>

namespace vostok {
namespace physics {
// sushi@TODO: Should be a private header. Not really?
// sushi@TODO: Primitive type moved from collision to physics
struct contact_test_predicate {
public:
	virtual	float		add_single_result		(
							void*				arg_0,
							collision::primitive_type		arg_1,
							float4x4 const&		arg_2,
							float3 const&		arg_3,
							collision::primitive_type		arg_4,
							float4x4 const&		arg_5,
							float3 const&		arg_6
						) = 0;
}; // struct contact_test_predicate

STATIC_SIZE_ASSERT(contact_test_predicate, 0x4);


struct contact_result_callback : btCollisionWorld::ContactResultCallback {
public:
	inline				contact_result_callback( contact_test_predicate* predicate ) : m_predicate( predicate ) { }

	// sushi@TODO: The impl is in `bullet_physics_world.cpp`
	virtual	btScalar	addSingleResult			(
							btManifoldPoint&			cp,
							btCollisionObject const*	colObj0,
							s32							partId0,
							s32							index0,
							btCollisionObject const*	colObj1,
							s32							partId1,
							s32							index1
						) override;

public:
	/* 0x0000 */	/* btCollisionWorld::ContactResultCallback */
	/* 0x0008 */	contact_test_predicate*		m_predicate;
}; // struct contact_result_callback

STATIC_SIZE_ASSERT(contact_result_callback, 0xC);

} // namespace physics
} // namespace vostok

#endif // #ifndef VOSTOK_PHYSICS_CONTACT_TEST_PREDICATE_H_INCLUDED