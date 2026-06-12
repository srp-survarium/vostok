////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#ifndef VECTOR_H_INCLUDED
#define VECTOR_H_INCLUDED

/* INCLUDES */
typedef stlp_std::vector<enum survarium::profile_slot_enum,survarium::std_allocator<enum survarium::profile_slot_enum> >
	stlp_std::vector<enum survarium::profile_slot_enum >;
class enum survarium::profile_slot_enum;

/* FORWARD REFS */
enum survarium::profile_slot_enum;

namespace survarium {

class vector< enum profile_slot_enum > : public std::vector< enum profile_slot_enum > {
public:
	inline								vector<enum survarium::profile_slot_enum>( ) { /* no source */ }
	inline								vector<enum survarium::profile_slot_enum>( u32 arg_0, profile_slot_enum const& arg_1 ) { /* no source */ }
	inline	explicit					vector<enum survarium::profile_slot_enum>( u32 arg_0 ) { /* no source */ }

	inline	profile_slot_enum&			operator[]	( u32 arg_0 ) { /* no source */ }
	inline	profile_slot_enum const&	operator[]	( u32 arg_0 ) const { /* no source */ }

	inline	u32							size		( ) const { /* no source */ }

	inline								~vector<enum survarium::profile_slot_enum>( ) { /* no source */ }
}; // class vector< enum profile_slot_enum >

STATIC_SIZE_ASSERT(vector< enum profile_slot_enum >, 0xC);

} // namespace survarium

#endif // #ifndef VECTOR_H_INCLUDED
