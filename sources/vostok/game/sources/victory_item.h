// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef VICTORY_ITEM_H_INCLUDED
#define VICTORY_ITEM_H_INCLUDED

#include <vostok/game_core/victory_item_core.h>
#include <vostok/game_core/scheduler.h>
#include <vostok/render/facade/model.h>

namespace survarium {

class game_world;

class victory_item : public victory_item_core {
	typedef victory_item_core super;

public:
			explicit	victory_item	( game_world& w );

			void		tick			( const u32, const u32 );	// PDB: both params __formal, genuinely unused

	// three params vs the core's two-param virtual - a new overload slot, not
	// an override (matches the PDB shape)
	virtual	void		put				( physics::world* world, float4x4 const& transform, scheduler& scheduler );

	virtual	pcstr		use_info		( usable_object_user_data* user ) override;

	virtual	void		take			( ) override;

	virtual	void		unload			( ) override;

	virtual	bool		use_initialize	( usable_object_user_data* arg_0 ) override { /* no source */ return false; }
	virtual	bool		use_execute		( usable_object_user_data* arg_0 ) override { /* no source */ return false; }
	virtual	bool		use_finalize	( usable_object_user_data* arg_0 ) override { /* no source */ return false; }


private:
	friend class victory_item_cook;

	/* 0x0000 */	/* victory_item_core */
	/* 0x0178 */	scheduler::identifier					m_scheduler_identifier;
	/* 0x017c */	scheduler*								m_scheduler;
	// PDB spells the member vostok::render::static_model_instance_ptr - the
	// same resource_ptr type our render tree typedefs as static_model_ptr
	/* 0x0180 */	render::static_model_ptr				m_model;
	/* 0x0184 */	game_world&								m_game_world;
}; // class victory_item

STATIC_SIZE_ASSERT(victory_item, 0x188);

typedef resources::resource_ptr<
	victory_item,
	resources::unmanaged_intrusive_base
> victory_item_ptr;

} // namespace survarium

#endif // #ifndef VICTORY_ITEM_H_INCLUDED
