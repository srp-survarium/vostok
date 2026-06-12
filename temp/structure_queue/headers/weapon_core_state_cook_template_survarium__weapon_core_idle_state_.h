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
class survarium::weapon_core_idle_state;
class survarium::weapon_state_creation_params;

namespace survarium {

class weapon_core_state_cook_template< weapon_core_idle_state > : public resources::unmanaged_cook {
public:
	inline								weapon_core_state_cook_template<survarium::weapon_core_idle_state>( ) { /* no source */ }
	virtual								~weapon_core_state_cook_template<survarium::weapon_core_idle_state>( ) { /* no source */ }

	virtual	mutable_buffer				allocate_resource		(
											resources::query_result_for_cook&		in_query,
											const_buffer							raw_file_data,
											bool									file_exist
										) override { /* no source */ }
	virtual	void						deallocate_resource		( void* arg_0 ) override { /* no source */ }
	virtual	void						create_resource			(
											resources::query_result_for_cook&		parent,
											const_buffer							raw_file_data,
											mutable_buffer							in_out_unmanaged_resource_buffer
										) override { /* no source */ }
	virtual	void						destroy_resource		( resources::unmanaged_resource* resource ) override { /* no source */ }

	inline	void						on_subresources_ready	(
											resources::queries_result&				data,
											mutable_buffer							buffer,
											weapon_state_creation_params const*		params
										) { /* no source */ }

			weapon_core_idle_state*		new_object				(
											mutable_buffer							buffer,
											weapon_state_creation_params const*		params,
											resources::managed_resource_ptr const*	animations,
											const u32								animations_count
										);
}; // class weapon_core_state_cook_template< weapon_core_idle_state >

STATIC_SIZE_ASSERT(weapon_core_state_cook_template< weapon_core_idle_state >, 0x20);

} // namespace survarium

#endif // #ifndef WEAPON_CORE_STATE_COOK_TEMPLATE_H_INCLUDED
