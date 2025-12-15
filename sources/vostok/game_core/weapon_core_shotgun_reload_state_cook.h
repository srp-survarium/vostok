////////////////////////////////////////////////////////////////////////////
//	Created 	: 06.12.2025
////////////////////////////////////////////////////////////////////////////

#ifndef WEAPON_CORE_SHOTGUN_RELOAD_STATE_COOK_H_INCLUDED
#define WEAPON_CORE_SHOTGUN_RELOAD_STATE_COOK_H_INCLUDED

namespace survarium {

/* survarium::weapon_core_shotgun_reload_state_cook */

//////////////////////////
// FORWARD DECLARATIONS //
//////////////////////////

class vostok::resources::unmanaged_cook;

//////////////////////////
//     DEFINITIONS      //
//////////////////////////

class weapon_core_shotgun_reload_state_cook : public resources::unmanaged_cook {
public:
	inline	weapon_core_shotgun_reload_state_cook( ) { /* no source */ }

	virtual	~weapon_core_shotgun_reload_state_cook( );

	virtual	mutable_buffer		allocate_resource				( resources::query_result_for_cook& in_query, const_buffer raw_file_data, bool file_exist ) override;

	virtual	void				deallocate_resource				( void* buffer ) override;

	virtual	void				create_resource					( resources::query_result_for_cook& parent, const_buffer raw_file_data, mutable_buffer in_out_unmanaged_resource_buffer ) override;

	virtual	void				destroy_resource				( resources::unmanaged_resource* resource ) override;

			void				on_subresources_ready			( resources::queries_result& data, mutable_buffer buffer, weapon_state_creation_params const* params );

	inline	void				__local_vftable_ctor_closure	( ) { /* no source */ }


private:
	/* 0x0000 */	/* resources::unmanaged_cook */
}; // class weapon_core_shotgun_reload_state_cook

STATIC_SIZE_ASSERT(weapon_core_shotgun_reload_state_cook, 0x20);

} // namespace survarium

#endif // #ifndef WEAPON_CORE_SHOTGUN_RELOAD_STATE_COOK_H_INCLUDED
