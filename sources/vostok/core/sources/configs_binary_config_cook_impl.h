////////////////////////////////////////////////////////////////////////////
//	Created		: 02.04.2009
//	Author		: Dmitriy Iassenev
//	Copyright (C) GSC Game World - 2009
////////////////////////////////////////////////////////////////////////////

#ifndef CONFIGS_BINARY_CONFIG_COOK_IMPL_H_INCLUDED
#define CONFIGS_BINARY_CONFIG_COOK_IMPL_H_INCLUDED

#include <vostok/resources_cook_classes.h>
#include <vostok/resources_fs.h>

namespace vostok {
namespace core {
namespace configs {

class binary_config_cook_impl : public resources::unmanaged_cook
{
public:
								binary_config_cook_impl		();

	virtual	mutable_buffer		allocate_resource		(resources::query_result_for_cook &	in_query,
														 const_buffer						raw_file_data,
														 bool								file_exist);

	virtual void				destroy_resource		(resources::unmanaged_resource *	resource);
	virtual void				deallocate_resource		(pvoid buffer);
	virtual void				create_resource			(resources::query_result_for_cook &	in_out_query,
													 const_buffer						raw_file_data,
													 mutable_buffer						in_out_unmanaged_resource_buffer);

	virtual					~binary_config_cook_impl	() {}
}; // class binary_config_cook_impl

} // namespace configs
} // namespace core
} // namespace vostok

#endif // #ifndef CONFIGS_BINARY_CONFIG_COOK_IMPL_H_INCLUDED
