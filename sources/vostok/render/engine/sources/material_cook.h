// SPDX-License-Identifier: GPL-3.0-or-later
#ifndef VOSTOK_RENDER_ENGINE_MATERIAL_COOK_H_INCLUDED
#define VOSTOK_RENDER_ENGINE_MATERIAL_COOK_H_INCLUDED
#include <vostok/resources_cook_classes.h>

namespace vostok {

namespace configs {

class binary_config;

} // namespace configs

namespace vfs {

class vfs_locked_iterator;

} // namespace vfs

namespace render {

struct material_cook : public resources::translate_query_cook {
	material_cook( );

	virtual void translate_query(
		resources::query_result_for_cook& parent
	) override;

	virtual void delete_resource(
		resources::resource_base* resource
	) override;

	void on_material_config_loaded( resources::queries_result& result );

private:
	void on_material_binary_config_loaded(
		resources::query_result_for_cook*	parent,
		configs::binary_config*			config
	);

	inline void on_fs_iterator_ready(
		vfs::vfs_locked_iterator const&,
		resources::query_result_for_cook*
	);
};

STATIC_SIZE_ASSERT( material_cook, 0x20 );

} // namespace render
} // namespace vostok

#endif // #ifndef VOSTOK_RENDER_ENGINE_MATERIAL_COOK_H_INCLUDED
