////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_VECTOR_H_INCLUDED
#define RENDER_VECTOR_H_INCLUDED

/* INCLUDES */
typedef stlp_std::vector<char const *,vostok::render::std_allocator<char const *> >
	stlp_std::vector<char const * >;
class char const *;

namespace vostok {
namespace render {

class vector< pcstr > : public std::vector< pcstr > {
public:
	inline					vector<char const *>	( ) { /* no source */ }
	inline					vector<char const *>	( u32 arg_0, pcstr const& arg_1 ) { /* no source */ }
	inline	explicit		vector<char const *>	( u32 arg_0 ) { /* no source */ }

	inline	pcstr&			operator[]				( u32 arg_0 ) { /* no source */ }
	inline	pcstr const&	operator[]				( u32 arg_0 ) const { /* no source */ }

	inline	u32				size					( ) const { /* no source */ }

	inline					~vector<char const *>	( ) { /* no source */ }
}; // class vector< pcstr >

STATIC_SIZE_ASSERT(vector< pcstr >, 0xC);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_VECTOR_H_INCLUDED
