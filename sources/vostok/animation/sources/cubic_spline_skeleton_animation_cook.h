////////////////////////////////////////////////////////////////////////////
//	Created		: 18.07.2011
//	Author		: Dmitriy Iassenev
//	Copyright (C) GSC Game World - 2011
////////////////////////////////////////////////////////////////////////////

#ifndef CUBIC_SPLINE_SKELETON_ANIMATION_COOK_H_INCLUDED
#define CUBIC_SPLINE_SKELETON_ANIMATION_COOK_H_INCLUDED

#include <vostok/resources_cook_classes.h>

namespace vostok {
namespace animation {

class cubic_spline_skeleton_animation_cook :
	public resources::managed_cook,
	private boost::noncopyable
{
public:
	inline			cubic_spline_skeleton_animation_cook( ) :
		resources::managed_cook(
			resources::cubic_spline_skeleton_animation_class,
			reuse_true,
			use_resource_manager_thread_id
		)
	{
		register_cook	( this );
	}

	virtual	u32		calculate_resource_size	( const_buffer in_raw_file_data, bool file_exist );
	virtual void	create_resource			(
						resources::query_result_for_cook&	in_out_query,
						const_buffer						bi_spline_skeleton_animation_buffer,
						resources::managed_resource_ptr		out_resource
					);
	virtual	void	destroy_resource		( resources::managed_resource* dying_resource ) ;

	virtual			~cubic_spline_skeleton_animation_cook( ) { }
}; // class cubic_spline_skeleton_animation_cook

STATIC_SIZE_ASSERT(cubic_spline_skeleton_animation_cook, 0x20);

} // namespace animation
} // namespace vostok

#endif // #ifndef CUBIC_SPLINE_SKELETON_ANIMATION_COOK_H_INCLUDED
