////////////////////////////////////////////////////////////////////////////
//	Created 	: 25.12.2025
////////////////////////////////////////////////////////////////////////////

#ifndef ANIMATION_BI_SPLINE_SKELETON_ANIMATION_BAKED_COOK_H_INCLUDED
#define ANIMATION_BI_SPLINE_SKELETON_ANIMATION_BAKED_COOK_H_INCLUDED

#include <vostok/resources_cook_classes.h>

namespace vostok {
namespace animation {

class bi_spline_skeleton_animation_baked_cook : public resources::inplace_unmanaged_cook {
public:
								bi_spline_skeleton_animation_baked_cook( );

private:
	virtual	mutable_buffer		allocate_resource		(
									resources::query_result_for_cook&		in_query,
									u32										file_size,
									u32&									out_offset_to_file,
									bool									file_exist
								) override;
	virtual	void				deallocate_resource		( void* buffer ) override;

	virtual	void				create_resource			(
									resources::query_result_for_cook&		in_out_query,
									mutable_buffer							in_out_unmanaged_resource_buffer
								) override;
	virtual	void				destroy_resource		( resources::unmanaged_resource* resource ) override;

	typedef fastdelegate::FastDelegate< void( resources::query_result_for_cook&, mutable_buffer ) > resource_delegate;
	virtual	resource_delegate	get_create_resource_inplace_in_inline_fat_delegate( ) override;
	virtual	resource_delegate	get_create_resource_inplace_in_creation_data_delegate( ) override;

private:
			void				create_resource_inplace	(
									resources::query_result_for_cook&		in_out_query,
									mutable_buffer							in_out_unmanaged_resource_buffer
								);

}; // class bi_spline_skeleton_animation_baked_cook

STATIC_SIZE_ASSERT(bi_spline_skeleton_animation_baked_cook, 0x20);

} // namespace animation
} // namespace vostok

#endif // #ifndef ANIMATION_BI_SPLINE_SKELETON_ANIMATION_BAKED_COOK_H_INCLUDED
