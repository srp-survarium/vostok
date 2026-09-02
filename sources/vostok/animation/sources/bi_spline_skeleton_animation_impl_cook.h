// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef ANIMATION_BI_SPLINE_SKELETON_ANIMATION_IMPL_COOK_H_INCLUDED
#define ANIMATION_BI_SPLINE_SKELETON_ANIMATION_IMPL_COOK_H_INCLUDED

#include <vostok/resources_cook_classes.h>

namespace vostok {
namespace animation {

class bi_spline_skeleton_animation_impl_cook : public resources::translate_query_cook {
public:
					bi_spline_skeleton_animation_impl_cook( );

private:
	virtual	void	translate_query	( resources::query_result_for_cook& parent ) override;
	virtual	void	delete_resource	( resources::resource_base* resource_to_delete ) override;

public:
	virtual			~bi_spline_skeleton_animation_impl_cook( ) { }

private:
	static	void	on_resources_ready(
						resources::queries_result&				results,
						resources::query_result_for_cook* const	parent_query
					);
}; // class bi_spline_skeleton_animation_impl_cook

STATIC_SIZE_ASSERT(bi_spline_skeleton_animation_impl_cook, 0x20);

} // namespace animation
} // namespace vostok

#endif // #ifndef ANIMATION_BI_SPLINE_SKELETON_ANIMATION_IMPL_COOK_H_INCLUDED
