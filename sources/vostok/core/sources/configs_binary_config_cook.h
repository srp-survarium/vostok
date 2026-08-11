////////////////////////////////////////////////////////////////////////////
//	Created		: 02.04.2009
//	Author		: Dmitriy Iassenev
//	Copyright (C) GSC Game World - 2009
////////////////////////////////////////////////////////////////////////////

#ifndef CONFIGS_BINARY_CONFIG_COOK_H_INCLUDED
#define CONFIGS_BINARY_CONFIG_COOK_H_INCLUDED

#include <vostok/resources_cook_classes.h>
#include <vostok/resources_fs.h>

namespace vostok {
namespace core {
namespace configs {

class binary_config;

class binary_config_cook : public resources::translate_query_cook {
	typedef resources::translate_query_cook				super;
public:
								explicit binary_config_cook		(resources::class_id_enum cls_id);
	virtual	void				translate_query			(resources::query_result_for_cook& parent);
	virtual void				delete_resource			(resources::resource_base* resource);
			void				register_object_to_delete	(binary_config* resource, u32 deallocation_thread_id);

protected:
#ifndef MASTER_GOLD
			void				on_lua_config_loaded	(resources::queries_result& data, resources::query_result_for_cook* parent_query);
#endif // #ifndef MASTER_GOLD
			void				on_fs_iterators_ready	(resources::queries_result & results);
			void				on_binary_config_loaded	(resources::queries_result& data, resources::query_result_for_cook* parent_query);

public:
	virtual					~binary_config_cook		() {}
}; // class binary_config_cook

} // namespace configs
} // namespace core
} // namespace vostok

#endif // #ifndef CONFIGS_BINARY_CONFIG_COOK_H_INCLUDED
