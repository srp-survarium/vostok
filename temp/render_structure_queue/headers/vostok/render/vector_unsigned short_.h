////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_VECTOR_H_INCLUDED
#define RENDER_VECTOR_H_INCLUDED

/* INCLUDES */
typedef stlp_std::vector<unsigned short,vostok::render::std_allocator<unsigned short> >
	stlp_std::vector<unsigned short >;
class unsigned short;

namespace vostok {
namespace render {

class vector< u16 > : public std::vector< u16 > {
public:
	inline					vector<unsigned short>	( ) { /* no source */ }
	inline					vector<unsigned short>	( u32 arg_0, u16 const& arg_1 ) { /* no source */ }
	inline	explicit		vector<unsigned short>	( u32 arg_0 ) { /* no source */ }

	inline	u16&			operator[]				( u32 arg_0 ) { /* no source */ }
	inline	u16 const&		operator[]				( u32 arg_0 ) const { /* no source */ }

	inline	u32				size					( ) const { /* no source */ }

	inline					~vector<unsigned short>	( ) { /* no source */ }
}; // class vector< u16 >

STATIC_SIZE_ASSERT(vector< u16 >, 0xC);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_VECTOR_H_INCLUDED
