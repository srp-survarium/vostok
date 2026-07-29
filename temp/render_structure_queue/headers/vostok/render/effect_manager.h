////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_EFFECT_MANAGER_H_INCLUDED
#define RENDER_EFFECT_MANAGER_H_INCLUDED

/* INCLUDES */
class vostok::quasi_singleton<vostok::render::effect_manager>;
class vostok::render::map<vostok::fixed_string<128>,vostok::render::effect_descriptor *,stlp_std::less<vostok::fixed_string<128> > >;
class vostok::render::set<vostok::render::res_effect *,vostok::render::effect_manager::compare_predicate<vostok::render::res_effect> >;
class vostok::render::set<vostok::render::res_pass *,vostok::render::effect_manager::compare_predicate<vostok::render::res_pass> >;
class vostok::render::set<vostok::render::res_shader_technique *,vostok::render::effect_manager::compare_predicate<vostok::render::res_shader_technique> >;
class vostok::render::vector<vostok::render::effect_manager::effect_holder_struct>;
class vostok::vectora<vostok::render::effect_manager::shader_cache_info>;
class vostok::render::custom_config;
typedef vostok::render::vector<vostok::resources::resource_ptr<vostok::render::res_effect,vostok::resources::unmanaged_intrusive_base> *>
	vostok::render::vector<vostok::render::res_effect_ptr *>;
class vostok::render::res_effect;

/* FORWARD REFS */
class vostok::configs::binary_config_value;
class vostok::render::custom_config_value;
class vostok::render::effect_descriptor;
class vostok::render::effect_loader;
class vostok::render::effect_options_descriptor;
class vostok::render::res_pass;
class vostok::render::res_shader_technique;
class vostok::render::vector<vostok::fs_new::virtual_path_string>;
class vostok::resources::queries_result;
class vostok::vectora<vostok::render::effect_manager::effect_to_recompile_struct>;

namespace vostok {
namespace render {

class effect_manager : public quasi_singleton< effect_manager > {
public:
										effect_manager					( );
										~effect_manager					( );

			void						add_effect						(
											effect_descriptor*			in_descriptor,
											custom_config_ptr const&	in_config,
											res_effect*					in_effect
										);
			void						remove_effect					( res_effect* in_effect );

			res_pass*					create_pass						( res_pass const& pass );
			void						delete_pass						( res_pass const* pass );

			res_shader_technique*		create_effect_technique			( res_shader_technique const& element );
			void						delete_effect_technique			( res_shader_technique const* technique );

	inline	res_effect_ptr				create_effect					( pcstr arg_0, effect_options_descriptor const& arg_1 ) { /* no source */ }
	inline	res_effect_ptr				create_effect					( pcstr arg_0, configs::binary_config_value const& arg_1, bool arg_2 ) { /* no source */ }
	inline	res_effect_ptr				create_effect					( pcstr arg_0, custom_config_value const& arg_1, bool arg_2 ) { /* no source */ }
	inline	res_effect_ptr				create_effect					( pcstr arg_0, pcstr arg_1 ) { /* no source */ }
	inline	res_effect_ptr				create_effect					( effect_descriptor* arg_0, pcstr arg_1 ) { /* no source */ }

private:
			void						on_effect_created				( res_effect_ptr* out_effect_ptr, resources::queries_result& data );
			void						on_async_effect_created			(
											resources::queries_result&		data,
											res_effect_ptr*					out_effect_ptr,
											effect_descriptor*				descriptor
										);
			void						on_async_effect_created_callback(
											resources::queries_result&		data,
											res_effect_ptr*					out_effect_ptr,
											effect_descriptor*				descriptor,
											boost::function< void() >		callback
										);

			res_effect_ptr				create_new_effect				( effect_descriptor& descriptor, custom_config_ptr const& ptr, u32 crc );
			void						create_new_effect				(
											res_effect_ptr*				out_effect,
											effect_descriptor*			descriptor,
											custom_config_ptr const&	config,
											u32							crc
										);
	inline	void						create_new_effect				(
											res_effect_ptr*					arg_0,
											effect_descriptor*				arg_1,
											custom_config_ptr const&		arg_2,
											u32								arg_3,
											boost::function< void() >		arg_4
										) { /* no source */ }
	inline	void						create_new_effect				(
											effect_loader*				arg_0,
											effect_descriptor*			arg_1,
											custom_config_ptr const&	arg_2,
											u32							arg_3
										) { /* no source */ }
public:
	inline	void						delete_effect					( res_effect const* arg_0 ) { /* no source */ }

			void						register_effect_desctiptor		( pcstr name, effect_descriptor* dectriptor );

	inline	void						recompile_shaders				( vector< fs_new::virtual_path_string > const& arg_0 ) { /* no source */ }
			void						recompile_shaders_async			( vector< fs_new::virtual_path_string > const& in_changed_defines );

private:
			void						on_effects_recompiled			(
											vectora< effect_manager::effect_to_recompile_struct >*	effects_to_recompile,
											resources::queries_result&		data
										);

public:
			effect_descriptor*			get_effect_descriptor_by_name	( pcstr name );

	inline	void						delete_pending_effect			( res_effect_ptr* arg_0 ) { /* no source */ }
private:
	inline	effect_descriptor*			find_effect						( pcstr arg_0, pcstr arg_1 ) { /* no source */ }

	inline	void						load_effects					( ) { /* no source */ }
	inline	void						load_raw_file					( resources::queries_result& arg_0 ) { /* no source */ }

	inline	effect_descriptor*			make_effect						( u64 arg_0 ) { /* no source */ }

	/* 0x0000 */	/* quasi_singleton< effect_manager > */
	/* 0x0000 */	bool							m_is_effects_query_processing;
	/* 0x0004 */	vectora< effect_manager::shader_cache_info >	m_shader_cache_info;
public:
	/* 0x0014 */	bool							force_sync;
private:
	/* 0x0018 */	set< res_pass*, effect_manager::compare_predicate< res_pass > >	m_passes;
	/* 0x0030 */	set< res_effect*, effect_manager::compare_predicate< res_effect > >	m_shaders;
	/* 0x0048 */	set< res_shader_technique*, effect_manager::compare_predicate< res_shader_technique > >	m_techniques;
	/* 0x0060 */	vector< effect_manager::effect_holder_struct >	m_effects;
	/* 0x006c */	map< fixed_string< 128 >, effect_descriptor*, std::less< fixed_string< 128 > > >	m_effect_descriptors;
	/* 0x0084 */	map< fixed_string< 128 >, effect_descriptor*, std::less< fixed_string< 128 > > >	m_effect_descriptors_by_texture;
	/* 0x009c */	bool							m_loading_incomplete;
	/* 0x00a0 */	vector< res_effect_ptr* >		m_effects_deleted_in_pending;
}; // class effect_manager

STATIC_SIZE_ASSERT(effect_manager, 0xAC);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_EFFECT_MANAGER_H_INCLUDED
