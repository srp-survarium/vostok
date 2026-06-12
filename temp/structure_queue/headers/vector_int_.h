////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#ifndef VECTOR_H_INCLUDED
#define VECTOR_H_INCLUDED

/* INCLUDES */
typedef stlp_std::vector<int,survarium::std_allocator<int> >
	stlp_std::vector<int >;
class int;

namespace survarium {

class vector< int > : public std::vector< int > {
public:
	inline					vector<int>	( ) { /* no source */ }
	inline					vector<int>	( u32 arg_0, s32 const& arg_1 ) { /* no source */ }
	inline	explicit		vector<int>	( u32 arg_0 ) { /* no source */ }

	inline	s32&			operator[]	( u32 arg_0 ) { /* no source */ }
	inline	s32 const&		operator[]	( u32 arg_0 ) const { /* no source */ }

	inline	u32				size		( ) const { /* no source */ }

	inline					~vector<int>( ) { /* no source */ }
}; // class vector< int >

STATIC_SIZE_ASSERT(vector< int >, 0xC);

} // namespace survarium

#endif // #ifndef VECTOR_H_INCLUDED
