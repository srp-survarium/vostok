////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#ifndef REMOVE_VERTEX_FROM_HIT_PARAMETERS_PREDICATE_H_INCLUDED
#define REMOVE_VERTEX_FROM_HIT_PARAMETERS_PREDICATE_H_INCLUDED

/* INCLUDES */
class survarium::body_part_parameters;
class survarium::hit_type_parameters* const;

namespace survarium {

struct remove_vertex_from_hit_parameters_predicate : public boost::noncopyable {
	inline	explicit	remove_vertex_from_hit_parameters_predicate( body_part_parameters* arg_0 ) { /* no source */ }

	inline	void		operator()	( hit_type_parameters* const arg_0 ) const { /* no source */ }

	inline				~remove_vertex_from_hit_parameters_predicate( ) { /* no source */ }

public:
	/* 0x0000 */	/* boost::noncopyable */
	/* 0x0000 */	body_part_parameters*		vertex;
}; // struct remove_vertex_from_hit_parameters_predicate

STATIC_SIZE_ASSERT(remove_vertex_from_hit_parameters_predicate, 0x4);

} // namespace survarium

#endif // #ifndef REMOVE_VERTEX_FROM_HIT_PARAMETERS_PREDICATE_H_INCLUDED
