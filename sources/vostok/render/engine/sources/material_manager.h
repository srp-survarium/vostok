#ifndef VOSTOK_RENDER_ENGINE_MATERIAL_MANAGER_H_INCLUDED
#define VOSTOK_RENDER_ENGINE_MATERIAL_MANAGER_H_INCLUDED

#include <vostok/fs/virtual_path_string.h>
#include <vostok/render/core/memory.h>
#include <vostok/render/core/quasi_singleton.h>

#include "material.h"
#include "material_effects_instance.h"

namespace vostok {

namespace resources {

class queries_result;

} // namespace resources

namespace render {

struct post_process_parameters;

struct material_effects_entry {
	material_effects_entry(
		material_effects_instance_ptr*	material_effects_instance,
		fs_new::virtual_path_string		material_name
	) :
		m_material_effects_instance_ptr	( material_effects_instance ),
		m_material_name					( material_name )
	{
	}

	material_effects_instance_ptr*	m_material_effects_instance_ptr;
	fs_new::virtual_path_string		m_material_name;
};

STATIC_SIZE_ASSERT( material_effects_entry, 0x118 );

class material_manager : public quasi_singleton< material_manager > {
public:
	typedef vector< material_effects_entry > material_effects_entries_type;
	typedef map<
		post_process_parameters*,
		fs_new::virtual_path_string
	> post_process_parameters_type;

	material_manager( );
	~material_manager( );

	void add_material_effects(
		material_effects_instance_ptr&		instance,
		fs_new::virtual_path_string const&	material_name
	);

	void remove_material_effects(
		material_effects_instance_ptr const& instance
	);

	void initialize_post_process_parameters(
		post_process_parameters*	parameters,
		material_ptr				material,
		bool						force_recompilation
	);

	void remove_post_process_parameters( post_process_parameters* )
	{
	}

	void get_output_text( fs_new::virtual_path_string*, u32& num_lines )
	{
		num_lines = 0;
	}

private:
	pcstr get_materials_path( ) const
	{
		return "resources/materials";
	}

	pcstr get_material_instances_path( ) const
	{
		return "resources/material_instances";
	}

	void on_material_effects_instance_loaded(
		resources::queries_result&,
		material_effects_instance_ptr*
	)
	{
	}

	void on_material_loaded(
		resources::queries_result&,
		post_process_parameters*
	)
	{
	}

private:
	material_effects_entries_type	m_material_effects;
	post_process_parameters_type	m_post_process_parameters;
	u32								m_watcher_materials_subscribe_id;
	u32								m_watcher_material_instanced_subscribe_id;
};

STATIC_SIZE_ASSERT( material_manager, 0x2C );

} // namespace render
} // namespace vostok

#endif // #ifndef VOSTOK_RENDER_ENGINE_MATERIAL_MANAGER_H_INCLUDED
