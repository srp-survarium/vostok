////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_VECTOR_H_INCLUDED
#define RENDER_VECTOR_H_INCLUDED

/* INCLUDES */
typedef stlp_std::vector<vostok::render::light_data,vostok::render::std_allocator<vostok::render::light_data> >
	stlp_std::vector<vostok::render::light_data >;
class vostok::render::light_data;

namespace vostok {
namespace render {

class vector< light_data > : public std::vector< light_data > {
public:
	inline							vector<vostok::render::light_data>	( ) { /* no source */ }
	inline							vector<vostok::render::light_data>	( u32 arg_0, light_data const& arg_1 ) { /* no source */ }
	inline	explicit				vector<vostok::render::light_data>	( u32 arg_0 ) { /* no source */ }

	inline	light_data&				operator[]							( u32 arg_0 ) { /* no source */ }
	inline	light_data const&		operator[]							( u32 arg_0 ) const { /* no source */ }

	inline	u32						size								( ) const { /* no source */ }

	inline							~vector<vostok::render::light_data>	( ) { /* no source */ }
}; // class vector< light_data >

STATIC_SIZE_ASSERT(vector< light_data >, 0xC);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_VECTOR_H_INCLUDED
