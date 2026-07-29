////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_MATERIAL_MANAGER_H_INCLUDED
#define RENDER_MATERIAL_MANAGER_H_INCLUDED

/* INCLUDES */
class vostok::quasi_singleton<vostok::render::material_manager>;
class vostok::render::map<vostok::render::post_process_parameters *,vostok::fs_new::virtual_path_string,stlp_std::less<vostok::render::post_process_parameters *> >;
class vostok::render::vector<vostok::render::material_effects_entry>;
class vostok::render::material;
class vostok::render::material_effects_instance;

/* FORWARD REFS */
class vostok::fs_new::virtual_path_string;
class vostok::render::post_process_parameters;
class vostok::resources::queries_result;

namespace vostok {
namespace render {

class material_manager : public quasi_singleton< material_manager > {
public:
						material_manager					( );
						~material_manager					( );

			void		add_material_effects				(
							material_effects_instance_ptr&			in_material_effects_instance,
							fs_new::virtual_path_string const&		in_material_name
						);
			void		remove_material_effects				( material_effects_instance_ptr const& in_material_effects_instance );

			void		initialize_post_process_parameters	(
							post_process_parameters*	out_post_process_parameters_ptr,
							material_ptr				mtl,
							bool						force_recompilation
						);
	inline	void		remove_post_process_parameters		( post_process_parameters* arg_0 ) { /* no source */ }

	inline	void		get_output_text						( fs_new::virtual_path_string* arg_0, u32& arg_1 ) { /* no source */ }
private:
	inline	pcstr		get_materials_path					( ) const { /* no source */ }
	inline	pcstr		get_material_instances_path			( ) const { /* no source */ }

	inline	void		on_material_effects_instance_loaded	( resources::queries_result& arg_0, material_effects_instance_ptr* arg_1 ) { /* no source */ }
	inline	void		on_material_loaded					( resources::queries_result& arg_0, post_process_parameters* arg_1 ) { /* no source */ }

	/* 0x0000 */	/* quasi_singleton< material_manager > */
	/* 0x0000 */	vector< material_effects_entry >	m_material_effects;
	/* 0x000c */	map< post_process_parameters*, fs_new::virtual_path_string, std::less< post_process_parameters* > >	m_post_process_parameters;
	/* 0x0024 */	u32									m_watcher_materials_subscribe_id;
	/* 0x0028 */	u32									m_watcher_material_instanced_subscribe_id;
}; // class material_manager

STATIC_SIZE_ASSERT(material_manager, 0x2C);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_MATERIAL_MANAGER_H_INCLUDED
