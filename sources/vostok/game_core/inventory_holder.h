////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.12.2025
////////////////////////////////////////////////////////////////////////////

#ifndef INVENTORY_HOLDER_H_INCLUDED
#define INVENTORY_HOLDER_H_INCLUDED

namespace survarium {

/* survarium::inventory_holder */

//////////////////////////
// FORWARD DECLARATIONS //
//////////////////////////

class boost::noncopyable;
class survarium::scheduler;
class vostok::resources::resource_ptr<survarium::inventory,vostok::resources::unmanaged_intrusive_base>;

//////////////////////////
//     DEFINITIONS      //
//////////////////////////

class inventory_holder : public boost::noncopyable {
public:
	inline										inventory_holder		( scheduler& arg_0, resources::resource_ptr<inventory,resources::unmanaged_intrusive_base> arg_1 ) { /* no source */ }
	virtual										~inventory_holder		( ) { /* no source */ }

	virtual	bool								set_new_active_item		( resources::resource_ptr<inventory_item,resources::unmanaged_intrusive_base> const& arg_0 ) = 0;
	virtual	void								take_inventory_item		( resources::resource_ptr<inventory_item,resources::unmanaged_intrusive_base> const& arg_0 ) = 0;

	virtual	resources::resource_ptr<damage_model,resources::unmanaged_intrusive_base> const&	damage_model			( ) const = 0;

	virtual	base_player*						cast_to_base_player		( ) = 0;

	virtual	physics::world*						get_physics_world		( ) = 0;

	inline	inventory const&					inventory				( ) const { /* no source */ }
	inline	inventory&							inventory				( ) { /* no source */ }

			scheduler&							scheduler				( );

	virtual	void								insert_game_world_object( game_world_object& arg_0 ) = 0;
	virtual	void								remove_game_world_object( game_world_object& arg_0 ) = 0;


private:
	/* 0x0004 */	/* boost::noncopyable */
	/* 0x0004 */	scheduler&							m_scheduler;
	/* 0x0008 */	resources::resource_ptr<inventory,resources::unmanaged_intrusive_base>	m_inventory;
}; // class inventory_holder

STATIC_SIZE_ASSERT(inventory_holder, 0xC);

} // namespace survarium

#endif // #ifndef INVENTORY_HOLDER_H_INCLUDED
