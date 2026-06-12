////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#ifndef WEAPON_CORE_STATE_COOK_TEMPLATE_H_INCLUDED
#define WEAPON_CORE_STATE_COOK_TEMPLATE_H_INCLUDED

/* INCLUDES */
class vostok::resources::unmanaged_cook;
class vostok::const_buffer;
class vostok::resources::managed_resource;

/* FORWARD REFS */
class vostok::resources::queries_result;
class vostok::resources::query_result_for_cook;
class vostok::resources::unmanaged_resource;
class survarium::double_barreled_weapon_core_reload_state;
class survarium::weapon_state_creation_params;

namespace survarium {

class weapon_core_state_cook_template< double_barreled_weapon_core_reload_state > : public resources::unmanaged_cook {
public:
	inline						weapon_core_state_cook_template<survarium::double_barreled_weapon_core_reload_state>( ) { /* no source */ }
	virtual						~weapon_core_state_cook_template<survarium::double_barreled_weapon_core_reload_state>( ) { /* no source */ }

	virtual	mutable_buffer		allocate_resource		( resources::query_result_for_cook& arg_0, const_buffer arg_1, bool arg_2 ) override { /* no source */ }
	virtual	void				deallocate_resource		( void* arg_0 ) override { /* no source */ }
	virtual	void				create_resource			( resources::query_result_for_cook& arg_0, const_buffer arg_1, mutable_buffer arg_2 ) override { /* no source */ }
	virtual	void				destroy_resource		( resources::unmanaged_resource* arg_0 ) override { /* no source */ }

	inline	void				on_subresources_ready	(
									resources::queries_result&				arg_0,
									mutable_buffer							arg_1,
									weapon_state_creation_params const*		arg_2
								) { /* no source */ }

			double_barreled_weapon_core_reload_state*	new_object				(
									mutable_buffer							buffer,
									weapon_state_creation_params const*		params,
									resources::managed_resource_ptr const*	animations,
									const u32								animations_count
								);
}; // class weapon_core_state_cook_template< double_barreled_weapon_core_reload_state >

STATIC_SIZE_ASSERT(weapon_core_state_cook_template< double_barreled_weapon_core_reload_state >, 0x20);

} // namespace survarium

#endif // #ifndef WEAPON_CORE_STATE_COOK_TEMPLATE_H_INCLUDED
