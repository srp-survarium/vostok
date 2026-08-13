////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.12.2025
////////////////////////////////////////////////////////////////////////////

#ifndef INVENTORY_HOLDER_H_INCLUDED
#define INVENTORY_HOLDER_H_INCLUDED

#include <vostok/game_core/inventory_item.h>
#include <vostok/game_core/scheduler.h>
#include <vostok/game_core/damage_model.h>
#include <vostok/game_core/inventory.h>

namespace vostok {
namespace physics {
	struct world;
}
}

namespace survarium {

class game_world_object;

class inventory_holder : public boost::noncopyable {
public:
	inline	explicit							inventory_holder		( survarium::scheduler& scheduler, inventory_ptr inventory ) : m_scheduler( scheduler ), m_inventory( inventory ) { }
	virtual										~inventory_holder		( ) { }

	virtual	bool								set_new_active_item		( inventory_item_ptr const& arg_0 ) = 0;
	virtual	void								take_inventory_item		( inventory_item_ptr const& arg_0 ) = 0;

	virtual	damage_model_ptr const&				damage_model			( ) const = 0;
	virtual	base_player*						cast_to_base_player		( ) = 0;
	virtual	physics::world*						get_physics_world		( ) = 0;

	inline	survarium::inventory&				inventory				( ) { return *m_inventory; }
	inline	survarium::inventory const&			inventory				( ) const { return *m_inventory; }

			scheduler&							scheduler				( ) { return m_scheduler; }

	virtual	void								insert_game_world_object( game_world_object& arg_0 ) = 0;
	virtual	void								remove_game_world_object( game_world_object& arg_0 ) = 0;


private:
	/* 0x0004 */	/* boost::noncopyable */
	/* 0x0004 */	survarium::scheduler&		m_scheduler;

protected:
	/* 0x0008 */	inventory_ptr				m_inventory;
}; // class inventory_holder

STATIC_SIZE_ASSERT(inventory_holder, 0xC);

} // namespace survarium

#endif // #ifndef INVENTORY_HOLDER_H_INCLUDED
