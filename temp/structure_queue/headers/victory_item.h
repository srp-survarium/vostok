////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#ifndef VICTORY_ITEM_H_INCLUDED
#define VICTORY_ITEM_H_INCLUDED

/* INCLUDES */
class survarium::game_world;
class survarium::scheduler;
class survarium::victory_item_core;
struct survarium::scheduler::identifier;
typedef vostok::resources::resource_ptr<vostok::render::static_model_instance,vostok::resources::unmanaged_intrusive_base>
	vostok::render::static_model_instance_ptr;
class vostok::render::static_model_instance;

/* FORWARD REFS */
class vostok::physics::world;
class survarium::usable_object_user_data;

namespace survarium {

class victory_item : public victory_item_core {
public:
			explicit	victory_item	( game_world& w );

			void		tick			( const u32 __formal, const u32 __formal );

	virtual	void		put				( physics::world* world, float4x4 const& transform, scheduler& scheduler );

	virtual	pcstr		use_info		( usable_object_user_data* user ) override;

	virtual	void		take			( ) override;

	virtual	void		unload			( ) override;

	virtual	bool		use_initialize	( usable_object_user_data* arg_0 ) override { /* no source */ }
	virtual	bool		use_execute		( usable_object_user_data* arg_0 ) override { /* no source */ }
	virtual	bool		use_finalize	( usable_object_user_data* arg_0 ) override { /* no source */ }

	virtual				~victory_item	( ) { /* no source */ }

private:
	/* 0x0000 */	/* victory_item_core */
	/* 0x0178 */	scheduler::identifier					m_scheduler_identifier;
	/* 0x017c */	scheduler*								m_scheduler;
	/* 0x0180 */	render::static_model_instance_ptr		m_model;
	/* 0x0184 */	game_world&								m_game_world;
}; // class victory_item

STATIC_SIZE_ASSERT(victory_item, 0x188);

} // namespace survarium

#endif // #ifndef VICTORY_ITEM_H_INCLUDED
