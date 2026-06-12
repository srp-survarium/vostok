////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#ifndef VECTOR_H_INCLUDED
#define VECTOR_H_INCLUDED

/* INCLUDES */
typedef stlp_std::vector<survarium::zone_group *,survarium::std_allocator<survarium::zone_group *> >
	stlp_std::vector<survarium::zone_group * >;
class survarium::zone_group *;

/* FORWARD REFS */
class survarium::zone_group;
class survarium::zone_group*;

namespace survarium {

class vector< zone_group* > : public std::vector< zone_group* > {
public:
	inline							vector<survarium::zone_group *>	( ) { /* no source */ }
	inline							vector<survarium::zone_group *>	( u32 arg_0, zone_group* const& arg_1 ) { /* no source */ }
	inline	explicit				vector<survarium::zone_group *>	( u32 arg_0 ) { /* no source */ }

	inline	zone_group*&			operator[]						( u32 arg_0 ) { /* no source */ }
	inline	zone_group* const&		operator[]						( u32 arg_0 ) const { /* no source */ }

	inline	u32						size							( ) const { /* no source */ }

	inline							~vector<survarium::zone_group *>( ) { /* no source */ }
}; // class vector< zone_group* >

STATIC_SIZE_ASSERT(vector< zone_group* >, 0xC);

} // namespace survarium

#endif // #ifndef VECTOR_H_INCLUDED
