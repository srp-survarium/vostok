////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_VECTOR_H_INCLUDED
#define RENDER_VECTOR_H_INCLUDED

/* INCLUDES */
typedef stlp_std::vector<unsigned int,vostok::render::std_allocator<unsigned int> >
	stlp_std::vector<unsigned int >;
class unsigned int;

namespace vostok {
namespace render {

class vector< u32 > : public std::vector< u32 > {
public:
	inline					vector<unsigned int>	( ) { /* no source */ }
	inline					vector<unsigned int>	( u32 arg_0, u32 const& arg_1 ) { /* no source */ }
	inline	explicit		vector<unsigned int>	( u32 arg_0 ) { /* no source */ }

	inline	u32&			operator[]				( u32 arg_0 ) { /* no source */ }
	inline	u32 const&		operator[]				( u32 arg_0 ) const { /* no source */ }

	inline	u32				size					( ) const { /* no source */ }

	inline					~vector<unsigned int>	( ) { /* no source */ }
}; // class vector< u32 >

STATIC_SIZE_ASSERT(vector< u32 >, 0xC);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_VECTOR_H_INCLUDED
