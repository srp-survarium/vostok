////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_SPEEDTREE_INSTANCE_IMPL_H_INCLUDED
#define RENDER_SPEEDTREE_INSTANCE_IMPL_H_INCLUDED

/* INCLUDES */
class SpeedTree::CInstance;
class vostok::render::speedtree_instance;
class vostok::render::speedtree_tree_base;

namespace vostok {
namespace render {

class speedtree_instance_impl : public speedtree_instance {
public:
			explicit						speedtree_instance_impl	( speedtree_tree_base_ptr in_speedtree_tree_ptr );
	virtual									~speedtree_instance_impl( );

			void							set_transform			( float4x4 const& transform );

			SpeedTree::CInstance const&		get_speedtree_instance	( ) const;

	/* 0x0000 */	/* speedtree_instance */
protected:
	/* 0x0150 */	SpeedTree::CInstance*		m_speedtree_instance;
	/* 0x0154 */	s32							m_speedtree_instance_index;
}; // class speedtree_instance_impl

STATIC_SIZE_ASSERT(speedtree_instance_impl, 0x158);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_SPEEDTREE_INSTANCE_IMPL_H_INCLUDED
