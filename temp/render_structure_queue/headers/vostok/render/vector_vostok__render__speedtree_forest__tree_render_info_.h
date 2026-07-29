////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_VECTOR_H_INCLUDED
#define RENDER_VECTOR_H_INCLUDED

/* INCLUDES */
typedef stlp_std::vector<vostok::render::speedtree_forest::tree_render_info,vostok::render::std_allocator<vostok::render::speedtree_forest::tree_render_info> >
	stlp_std::vector<vostok::render::speedtree_forest::tree_render_info >;
class vostok::render::speedtree_forest::tree_render_info;

namespace vostok {
namespace render {

class vector< speedtree_forest::tree_render_info > : public std::vector< speedtree_forest::tree_render_info > {
public:
	inline											vector<vostok::render::speedtree_forest::tree_render_info>( ) { /* no source */ }
	inline											vector<vostok::render::speedtree_forest::tree_render_info>( u32 arg_0, speedtree_forest::tree_render_info const& arg_1 ) { /* no source */ }
	inline	explicit								vector<vostok::render::speedtree_forest::tree_render_info>( u32 arg_0 ) { /* no source */ }

	inline	speedtree_forest::tree_render_info&		operator[]	( u32 arg_0 ) { /* no source */ }
	inline	speedtree_forest::tree_render_info const&	operator[]	( u32 arg_0 ) const { /* no source */ }

	inline	u32										size		( ) const { /* no source */ }

	inline											~vector<vostok::render::speedtree_forest::tree_render_info>( ) { /* no source */ }
}; // class vector< speedtree_forest::tree_render_info >

STATIC_SIZE_ASSERT(vector< speedtree_forest::tree_render_info >, 0xC);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_VECTOR_H_INCLUDED
