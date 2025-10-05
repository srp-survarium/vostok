////////////////////////////////////////////////////////////////////////////
//	Created 	: 05.10.2025
////////////////////////////////////////////////////////////////////////////

#ifndef VOSTOK_PHYSICS_2_CONTACT_TEST_PREDICATE_H_INCLUDED
#define VOSTOK_PHYSICS_2_CONTACT_TEST_PREDICATE_H_INCLUDED

#include <vostok/collision/primitives.h>

namespace vostok {
namespace physics_2 {

/* vostok::physics::contact_test_predicate */

//////////////////////////
//     DEFINITIONS      //
//////////////////////////

struct contact_test_predicate  {
public:
	virtual float add_single_result	(
										void*,
										collision::primitive_type,
										float4x4 const&,
										float3 const&,
										collision::primitive_type,
										float4x4 const&,
										float3 const&)				= 0;

}; // struct contact_test_predicate

namespace {
	typedef char size_assert[
		sizeof(contact_test_predicate) == 0x4 ? 1 : -1
	];
}

} // namespace physics
} // namespace vostok

#endif // #ifndef VOSTOK_PHYSICS_2_CONTACT_TEST_PREDICATE_H_INCLUDED