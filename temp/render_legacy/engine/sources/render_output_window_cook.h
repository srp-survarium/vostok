////////////////////////////////////////////////////////////////////////////
//	Created		: 21.03.2011
//	Author		: Nikolay Partas
//	Copyright (C) GSC Game World - 2011
////////////////////////////////////////////////////////////////////////////

#ifndef VOSTOK_RENDER_RENDER_OUTPUT_WINDOW_COOK_H_INCLUDED
#define VOSTOK_RENDER_RENDER_OUTPUT_WINDOW_COOK_H_INCLUDED

#include <vostok/resources_cook_classes.h>

namespace vostok {
namespace render {

class render_output_window_cook : public resources::unmanaged_cook
{
public:
	render_output_window_cook						();
	virtual	mutable_buffer	allocate_resource		(resources::query_result_for_cook& in_query, const_buffer raw_file_data, bool file_exist);
	virtual void			deallocate_resource		(pvoid buffer);
	virtual void			create_resource			(resources::query_result_for_cook& in_out_query, const_buffer raw_file_data, mutable_buffer in_out_unmanaged_resource_buffer);
	virtual void			destroy_resource		(resources::unmanaged_resource* resource);
}; // class render_output_window_cook

} // namespace render
} // namespace vostok

#endif // #ifndef VOSTOK_RENDER_RENDER_OUTPUT_WINDOW_COOK_H_INCLUDED