////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#ifndef VECTOR_H_INCLUDED
#define VECTOR_H_INCLUDED

/* INCLUDES */
typedef stlp_std::vector<vostok::variant<32>,survarium::std_allocator<vostok::variant<32> > >
	stlp_std::vector<vostok::variant<32> >;
class vostok::variant<32>;

namespace survarium {

class vector< variant< 32 > > : public std::vector< variant< 32 > > {
public:
	inline							vector<vostok::variant<32> >	( ) { /* no source */ }
	inline							vector<vostok::variant<32> >	( u32 arg_0, variant< 32 > const& arg_1 ) { /* no source */ }
	inline	explicit				vector<vostok::variant<32> >	( u32 arg_0 ) { /* no source */ }

	inline	variant< 32 >&			operator[]						( u32 arg_0 ) { /* no source */ }
	inline	variant< 32 > const&	operator[]						( u32 arg_0 ) const { /* no source */ }

	inline	u32						size							( ) const { /* no source */ }

	inline							~vector<vostok::variant<32> >	( ) { /* no source */ }
}; // class vector< variant< 32 > >

STATIC_SIZE_ASSERT(vector< variant< 32 > >, 0xC);

} // namespace survarium

#endif // #ifndef VECTOR_H_INCLUDED
