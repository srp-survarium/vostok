////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#ifndef TEST_OBJECTS_IN_SHAPE_PREDICATE_H_INCLUDED
#define TEST_OBJECTS_IN_SHAPE_PREDICATE_H_INCLUDED

/* INCLUDES */
struct vostok::physics::contact_test_predicate;
class vostok::physics::primitive_type;

namespace survarium {

struct test_objects_in_shape_predicate : public physics::contact_test_predicate {
	inline				test_objects_in_shape_predicate	( ) { /* no source */ }

	virtual	float		add_single_result				(
							void*						arg_0,
							physics::primitive_type		arg_1,
							float4x4 const&				arg_2,
							float3 const&				arg_3,
							physics::primitive_type		arg_4,
							float4x4 const&				arg_5,
							float3 const&				arg_6
						) override { /* no source */ }

public:
	/* 0x0000 */	/* physics::contact_test_predicate */
	/* 0x0004 */	bool	m_in_shape;
}; // struct test_objects_in_shape_predicate

STATIC_SIZE_ASSERT(test_objects_in_shape_predicate, 0x8);

} // namespace survarium

#endif // #ifndef TEST_OBJECTS_IN_SHAPE_PREDICATE_H_INCLUDED
