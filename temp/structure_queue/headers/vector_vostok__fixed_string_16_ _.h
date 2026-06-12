////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#ifndef VECTOR_H_INCLUDED
#define VECTOR_H_INCLUDED

/* INCLUDES */
typedef stlp_std::vector<vostok::fixed_string<16>,survarium::std_allocator<vostok::fixed_string<16> > >
	stlp_std::vector<vostok::fixed_string<16> >;
class vostok::fixed_string<16>;

namespace survarium {

class vector< fixed_string< 16 > > : public std::vector< fixed_string< 16 > > {
public:
	inline									vector<vostok::fixed_string<16> >	( ) { /* no source */ }
	inline									vector<vostok::fixed_string<16> >	( u32 arg_0, fixed_string< 16 > const& arg_1 ) { /* no source */ }
	inline	explicit						vector<vostok::fixed_string<16> >	( u32 arg_0 ) { /* no source */ }

	inline	fixed_string< 16 >&				operator[]							( u32 arg_0 ) { /* no source */ }
	inline	fixed_string< 16 > const&		operator[]							( u32 arg_0 ) const { /* no source */ }

	inline	u32								size								( ) const { /* no source */ }

	inline									~vector<vostok::fixed_string<16> >	( ) { /* no source */ }
}; // class vector< fixed_string< 16 > >

STATIC_SIZE_ASSERT(vector< fixed_string< 16 > >, 0xC);

} // namespace survarium

#endif // #ifndef VECTOR_H_INCLUDED
