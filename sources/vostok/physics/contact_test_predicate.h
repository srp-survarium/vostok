////////////////////////////////////////////////////////////////////////////
//	Created 	: 05.10.2025
////////////////////////////////////////////////////////////////////////////

#ifndef VOSTOK_PHYSICS_CONTACT_TEST_PREDICATE_H_INCLUDED
#define VOSTOK_PHYSICS_CONTACT_TEST_PREDICATE_H_INCLUDED

#include <vostok/collision/primitives.h>

namespace vostok {
namespace physics {

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

} // namespace physics
} // namespace vostok

#endif // #ifndef VOSTOK_PHYSICS_CONTACT_TEST_PREDICATE_H_INCLUDED