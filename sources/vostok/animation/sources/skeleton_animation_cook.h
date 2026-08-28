////////////////////////////////////////////////////////////////////////////
//	Created		: 20.07.2011
//	Author		: Dmitriy Iassenev
//	Copyright (C) GSC Game World - 2011
////////////////////////////////////////////////////////////////////////////

#ifndef SKELETON_ANIMATION_COOK_H_INCLUDED
#define SKELETON_ANIMATION_COOK_H_INCLUDED

#include <vostok/resources_cook_classes.h>

namespace vostok {
namespace animation {

class bi_spline_skeleton_animation_baked;

typedef resources::resource_ptr< bi_spline_skeleton_animation_baked, resources::unmanaged_intrusive_base >
	bi_spline_skeleton_animation_baked_ptr;

class skeleton_animation_cook :
	public resources::translate_query_cook,
	private boost::noncopyable
{
public:
						skeleton_animation_cook		( );

private:
	typedef translate_query_cook					super;

private:
	virtual	void	translate_query					( resources::query_result_for_cook& parent );
	virtual void	delete_resource					( resources::resource_base* resource_to_delete );

public:
	virtual			~skeleton_animation_cook		( ) { }

private:
	static	void	on_cubic_spline_animation_cooked(
						resources::queries_result&				data,
						bi_spline_skeleton_animation_baked_ptr	bi_spline_animation
					);
	static	void	on_bi_spline_animation_arrived	( resources::queries_result& result );
}; // class skeleton_animation_cook

STATIC_SIZE_ASSERT(skeleton_animation_cook, 0x20);

} // namespace animation
} // namespace vostok

#endif // #ifndef SKELETON_ANIMATION_COOK_H_INCLUDED
