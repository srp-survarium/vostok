////////////////////////////////////////////////////////////////////////////
//	Created 	: 05.10.2025
////////////////////////////////////////////////////////////////////////////

#ifndef VOSTOK_PHYSICS_CONTACT_TEST_PREDICATE_H_INCLUDED
#define VOSTOK_PHYSICS_CONTACT_TEST_PREDICATE_H_INCLUDED

#include <vostok/physics/enums/primitive_type.h>
#include <vostok/physics/sources/bullet_include.h>

namespace vostok {
namespace physics {

struct contact_test_predicate {
public:
	virtual	float		add_single_result		(
							void*						user_data,
							primitive_type				first_shape_type,
							float4x4 const&				first_shape_transform,
							float3 const&				first_shape_dimension,
							primitive_type				second_shape_type,
							float4x4 const&				second_shape_transform,
							float3 const&				second_shape_dimension
						) = 0;
}; // struct contact_test_predicate

STATIC_SIZE_ASSERT(contact_test_predicate, 0x4);

// sushi@TODO: Might be private
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
