////////////////////////////////////////////////////////////////////////////
//	Created		: 20.04.2010
//	Author		: Dmitriy Iassenev
//	Copyright (C) GSC Game World - 2010
////////////////////////////////////////////////////////////////////////////

#ifndef VOSTOK_ANIMATION_MIXING_N_ARY_TREE_INTRUSIVE_BASE_H_INCLUDED
#define VOSTOK_ANIMATION_MIXING_N_ARY_TREE_INTRUSIVE_BASE_H_INCLUDED

#include <vostok/animation/api.h>

namespace vostok {
namespace animation {
namespace mixing {

class VOSTOK_ANIMATION_API n_ary_tree_intrusive_base {
public:
	inline		n_ary_tree_intrusive_base	( );

	template <typename T>
	static	inline	void	destroy			( T& );

	inline	u32				reference_count	( ) const;

private:
	friend class threading::single_threading_policy;
	u32		m_reference_count;
}; // class n_ary_tree_intrusive_base

} // namespace mixing
} // namespace animation
} // namespace vostok

#include <vostok/animation/mixing_n_ary_tree_intrusive_base_inline.h>

#endif // #ifndef VOSTOK_ANIMATION_MIXING_N_ARY_TREE_INTRUSIVE_BASE_H_INCLUDED