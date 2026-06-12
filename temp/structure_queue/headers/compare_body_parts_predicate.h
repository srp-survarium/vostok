////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#ifndef COMPARE_BODY_PARTS_PREDICATE_H_INCLUDED
#define COMPARE_BODY_PARTS_PREDICATE_H_INCLUDED

/* FORWARD REFS */
class vostok::fixed_string<16>;

namespace survarium {

struct compare_body_parts_predicate {
	inline	explicit	compare_body_parts_predicate( pcstr arg_0 ) { /* no source */ }

	inline	bool		operator()					( fixed_string< 16 > const& arg_0 ) const { /* no source */ }

public:
	/* 0x0000 */	pcstr		m_name;
}; // struct compare_body_parts_predicate

STATIC_SIZE_ASSERT(compare_body_parts_predicate, 0x4);

} // namespace survarium

#endif // #ifndef COMPARE_BODY_PARTS_PREDICATE_H_INCLUDED
