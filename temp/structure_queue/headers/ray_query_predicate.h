////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RAY_QUERY_PREDICATE_H_INCLUDED
#define RAY_QUERY_PREDICATE_H_INCLUDED

/* INCLUDES */
class vostok::collision::object;
class vostok::collision::object const* const;

/* FORWARD REFS */
class vostok::collision::ray_triangle_result;

namespace survarium {

struct ray_query_predicate : public boost::noncopyable {
	inline			ray_query_predicate	(
						float&								arg_0,
						collision::object const* const		arg_1,
						collision::object const* const		arg_2,
						const float							arg_3
					) { /* no source */ }

	inline	bool	predicate			( collision::ray_triangle_result const& arg_0 ) { /* no source */ }

	inline			~ray_query_predicate( ) { /* no source */ }

public:
	/* 0x0000 */	/* boost::noncopyable */
	/* 0x0000 */	float&								visibility_value;
	/* 0x0004 */	collision::object const* const		requested_object;
	/* 0x0008 */	collision::object const* const		object_to_ignore;
	/* 0x000c */	const float							transparency_threshold;
	/* 0x0010 */	bool								requested_object_was_found;
}; // struct ray_query_predicate

STATIC_SIZE_ASSERT(ray_query_predicate, 0x14);

} // namespace survarium

#endif // #ifndef RAY_QUERY_PREDICATE_H_INCLUDED
