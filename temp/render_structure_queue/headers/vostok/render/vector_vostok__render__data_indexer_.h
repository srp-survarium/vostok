////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_VECTOR_H_INCLUDED
#define RENDER_VECTOR_H_INCLUDED

/* INCLUDES */
typedef stlp_std::vector<vostok::render::data_indexer,vostok::render::std_allocator<vostok::render::data_indexer> >
	stlp_std::vector<vostok::render::data_indexer >;
class vostok::render::data_indexer;

namespace vostok {
namespace render {

class vector< data_indexer > : public std::vector< data_indexer > {
public:
	inline							vector<vostok::render::data_indexer>( ) { /* no source */ }
	inline							vector<vostok::render::data_indexer>( u32 arg_0, data_indexer const& arg_1 ) { /* no source */ }
	inline	explicit				vector<vostok::render::data_indexer>( u32 arg_0 ) { /* no source */ }

	inline	data_indexer&			operator[]	( u32 arg_0 ) { /* no source */ }
	inline	data_indexer const&		operator[]	( u32 arg_0 ) const { /* no source */ }

	inline	u32						size		( ) const { /* no source */ }

	inline							~vector<vostok::render::data_indexer>( ) { /* no source */ }
}; // class vector< data_indexer >

STATIC_SIZE_ASSERT(vector< data_indexer >, 0xC);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_VECTOR_H_INCLUDED
