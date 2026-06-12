////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#ifndef VECTOR_H_INCLUDED
#define VECTOR_H_INCLUDED

/* INCLUDES */
typedef stlp_std::vector<survarium::zone_group::zone_wrapper,survarium::std_allocator<survarium::zone_group::zone_wrapper> >
	stlp_std::vector<survarium::zone_group::zone_wrapper >;
class survarium::zone_group::zone_wrapper;

namespace survarium {

class vector< zone_group::zone_wrapper > : public std::vector< zone_group::zone_wrapper > {
public:
	inline										vector<survarium::zone_group::zone_wrapper>( ) { /* no source */ }
	inline										vector<survarium::zone_group::zone_wrapper>( u32 arg_0, zone_group::zone_wrapper const& arg_1 ) { /* no source */ }
	inline	explicit							vector<survarium::zone_group::zone_wrapper>( u32 arg_0 ) { /* no source */ }

	inline	zone_group::zone_wrapper&			operator[]	( u32 arg_0 ) { /* no source */ }
	inline	zone_group::zone_wrapper const&		operator[]	( u32 arg_0 ) const { /* no source */ }

	inline	u32									size		( ) const { /* no source */ }

	inline										~vector<survarium::zone_group::zone_wrapper>( ) { /* no source */ }
}; // class vector< zone_group::zone_wrapper >

STATIC_SIZE_ASSERT(vector< zone_group::zone_wrapper >, 0xC);

} // namespace survarium

#endif // #ifndef VECTOR_H_INCLUDED
