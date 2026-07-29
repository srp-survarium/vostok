////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_VECTOR_H_INCLUDED
#define RENDER_VECTOR_H_INCLUDED

/* INCLUDES */
typedef stlp_std::vector<vostok::fixed_string<32>,vostok::render::std_allocator<vostok::fixed_string<32> > >
	stlp_std::vector<vostok::fixed_string<32> >;
class vostok::fixed_string<32>;

namespace vostok {
namespace render {

class vector< fixed_string< 32 > > : public std::vector< fixed_string< 32 > > {
public:
	inline									vector<vostok::fixed_string<32> >	( ) { /* no source */ }
	inline									vector<vostok::fixed_string<32> >	( u32 arg_0, fixed_string< 32 > const& arg_1 ) { /* no source */ }
	inline	explicit						vector<vostok::fixed_string<32> >	( u32 arg_0 ) { /* no source */ }

	inline	fixed_string< 32 >&				operator[]							( u32 arg_0 ) { /* no source */ }
	inline	fixed_string< 32 > const&		operator[]							( u32 arg_0 ) const { /* no source */ }

	inline	u32								size								( ) const { /* no source */ }

	inline									~vector<vostok::fixed_string<32> >	( ) { /* no source */ }
}; // class vector< fixed_string< 32 > >

STATIC_SIZE_ASSERT(vector< fixed_string< 32 > >, 0xC);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_VECTOR_H_INCLUDED
