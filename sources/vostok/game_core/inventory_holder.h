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

	// claude@MATCH: `return *m_inventory` reads the raw ptr inside the smart-ptr
	// member @ +8 -> `mov eax,[this+8]; ret`. const + non-const are byte-identical
	// so /OPT:ICF folds them (the const one shares the non-const's fold -> None).
	// STATE[None|DONE]
	inline	survarium::inventory const&			inventory				( ) const { return *m_inventory; }
	// claude@NOTE: body verified byte-correct (inlined into the anchor's touch() as
	// `mov eax,[this+8]` - the raw ptr inside the smart-ptr @ +8). /GL LTCG inlines
	// this trivial accessor whole-program, so no standalone body reaches the EXE ->
	// None until a real game consumer keeps it standalone (inline-vs-call class).
	// STATE[None|PARTIAL]: trivial-accessor LTCG inline-vs-call (byte-correct).
	inline	survarium::inventory&				inventory				( ) { return *m_inventory; }

	// claude@NOTE: body verified byte-correct (inlined into the anchor's touch() as
	// `mov eax,[this+4]` = m_scheduler reference @ +4). Same trivial-accessor LTCG
	// inline-vs-call as inventory() above; no standalone body reaches the EXE -> None.
	// STATE[None|PARTIAL]: trivial-accessor LTCG inline-vs-call (byte-correct).
			scheduler&							scheduler				( ) { return m_scheduler; }

	virtual	void								insert_game_world_object( game_world_object& arg_0 ) = 0;
	virtual	void								remove_game_world_object( game_world_object& arg_0 ) = 0;


private:
	/* 0x0004 */	/* boost::noncopyable */
	/* 0x0004 */	survarium::scheduler&		m_scheduler;
	/* 0x0008 */	inventory_ptr				m_inventory;
}; // class inventory_holder

STATIC_SIZE_ASSERT(inventory_holder, 0xC);

} // namespace survarium

#endif // #ifndef INVENTORY_HOLDER_H_INCLUDED
