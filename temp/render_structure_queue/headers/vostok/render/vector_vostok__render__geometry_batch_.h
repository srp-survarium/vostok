////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_VECTOR_H_INCLUDED
#define RENDER_VECTOR_H_INCLUDED

/* INCLUDES */
typedef stlp_std::vector<vostok::render::geometry_batch,vostok::render::std_allocator<vostok::render::geometry_batch> >
	stlp_std::vector<vostok::render::geometry_batch >;
class vostok::render::geometry_batch;

namespace vostok {
namespace render {

class vector< geometry_batch > : public std::vector< geometry_batch > {
public:
	inline								vector<vostok::render::geometry_batch>( ) { /* no source */ }
	inline								vector<vostok::render::geometry_batch>( u32 arg_0, geometry_batch const& arg_1 ) { /* no source */ }
	inline	explicit					vector<vostok::render::geometry_batch>( u32 arg_0 ) { /* no source */ }

	inline	geometry_batch&				operator[]	( u32 arg_0 ) { /* no source */ }
	inline	geometry_batch const&		operator[]	( u32 arg_0 ) const { /* no source */ }

	inline	u32							size		( ) const { /* no source */ }

	inline								~vector<vostok::render::geometry_batch>( ) { /* no source */ }
}; // class vector< geometry_batch >

STATIC_SIZE_ASSERT(vector< geometry_batch >, 0xC);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_VECTOR_H_INCLUDED
