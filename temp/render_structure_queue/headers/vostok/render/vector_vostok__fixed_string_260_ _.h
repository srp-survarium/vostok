////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_VECTOR_H_INCLUDED
#define RENDER_VECTOR_H_INCLUDED

/* INCLUDES */
typedef stlp_std::vector<vostok::fixed_string<260>,vostok::render::std_allocator<vostok::fixed_string<260> > >
	stlp_std::vector<vostok::fixed_string<260> >;
class vostok::fixed_string<260>;

namespace vostok {
namespace render {

class vector< fixed_string< 260 > > : public std::vector< fixed_string< 260 > > {
public:
	inline									vector<vostok::fixed_string<260> >	( ) { /* no source */ }
	inline									vector<vostok::fixed_string<260> >	( u32 arg_0, fixed_string< 260 > const& arg_1 ) { /* no source */ }
	inline	explicit						vector<vostok::fixed_string<260> >	( u32 arg_0 ) { /* no source */ }

	inline	fixed_string< 260 >&			operator[]							( u32 arg_0 ) { /* no source */ }
	inline	fixed_string< 260 > const&		operator[]							( u32 arg_0 ) const { /* no source */ }

	inline	u32								size								( ) const { /* no source */ }

	inline									~vector<vostok::fixed_string<260> >	( ) { /* no source */ }
}; // class vector< fixed_string< 260 > >

STATIC_SIZE_ASSERT(vector< fixed_string< 260 > >, 0xC);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_VECTOR_H_INCLUDED
