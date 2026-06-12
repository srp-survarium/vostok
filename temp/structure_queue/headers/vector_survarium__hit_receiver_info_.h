////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#ifndef VECTOR_H_INCLUDED
#define VECTOR_H_INCLUDED

/* INCLUDES */
typedef stlp_std::vector<survarium::hit_receiver_info,survarium::std_allocator<survarium::hit_receiver_info> >
	stlp_std::vector<survarium::hit_receiver_info >;
class survarium::hit_receiver_info;

namespace survarium {

class vector< hit_receiver_info > : public std::vector< hit_receiver_info > {
public:
	inline								vector<survarium::hit_receiver_info>( ) { /* no source */ }
	inline								vector<survarium::hit_receiver_info>( u32 arg_0, hit_receiver_info const& arg_1 ) { /* no source */ }
	inline	explicit					vector<survarium::hit_receiver_info>( u32 arg_0 ) { /* no source */ }

	inline	hit_receiver_info&			operator[]	( u32 arg_0 ) { /* no source */ }
	inline	hit_receiver_info const&	operator[]	( u32 arg_0 ) const { /* no source */ }

	inline	u32							size		( ) const { /* no source */ }

	inline								~vector<survarium::hit_receiver_info>( ) { /* no source */ }
}; // class vector< hit_receiver_info >

STATIC_SIZE_ASSERT(vector< hit_receiver_info >, 0xC);

} // namespace survarium

#endif // #ifndef VECTOR_H_INCLUDED
