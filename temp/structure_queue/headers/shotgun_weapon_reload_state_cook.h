////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#ifndef SHOTGUN_WEAPON_RELOAD_STATE_COOK_H_INCLUDED
#define SHOTGUN_WEAPON_RELOAD_STATE_COOK_H_INCLUDED

/* INCLUDES */
class vostok::core::noncopyable;
class vostok::resources::unmanaged_cook;
class vostok::const_buffer;

/* FORWARD REFS */
class vostok::resources::queries_result;
class vostok::resources::query_result_for_cook;
class vostok::resources::unmanaged_resource;
class survarium::weapon_state_creation_params;

namespace survarium {

class shotgun_weapon_reload_state_cook : public resources::unmanaged_cook , public core::noncopyable {
public:
								shotgun_weapon_reload_state_cook	( );
	virtual						~shotgun_weapon_reload_state_cook	( );

	virtual	mutable_buffer		allocate_resource					(
									resources::query_result_for_cook&		in_query,
									const_buffer							raw_file_data,
									bool									file_exist
								) override;
	virtual	void				deallocate_resource					( void* buffer ) override;
	virtual	void				create_resource						(
									resources::query_result_for_cook&		parent,
									const_buffer							raw_file_data,
									mutable_buffer							in_out_unmanaged_resource_buffer
								) override;
	virtual	void				destroy_resource					( resources::unmanaged_resource* resource ) override;

			void				on_substates_ready					(
									resources::queries_result&				data,
									mutable_buffer							buffer,
									weapon_state_creation_params const*		params
								);
}; // class shotgun_weapon_reload_state_cook

STATIC_SIZE_ASSERT(shotgun_weapon_reload_state_cook, 0x20);

} // namespace survarium

#endif // #ifndef SHOTGUN_WEAPON_RELOAD_STATE_COOK_H_INCLUDED
