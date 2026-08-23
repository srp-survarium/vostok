////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.12.2025
////////////////////////////////////////////////////////////////////////////

#ifndef VICTORY_ITEM_CORE_H_INCLUDED
#define VICTORY_ITEM_CORE_H_INCLUDED

#include <vostok/game_core/usable_object.h>
#include <vostok/game_core/game_team_id.h>

namespace vostok {
namespace physics {
	struct world;
}
}

namespace survarium {

struct usable_object_user_data;

class victory_item_core : public usable_object , public resources::unmanaged_resource {
public:
			explicit		victory_item_core			( );
	virtual					~victory_item_core			( );

	virtual	void			load						( configs::binary_config_value const& cfg ) override;

	virtual	bool			use_initialize				( usable_object_user_data* user ) override;
	virtual	bool			use_execute					( usable_object_user_data* user ) override;
	virtual	bool			use_finalize				( usable_object_user_data* user ) override;

	virtual	pcstr			use_info					( usable_object_user_data* user ) override { VOSTOK_UNREFERENCED_PARAMETER( user ); return ""; }

	virtual	void			put							( physics::world* world, float4x4 const& transform );
	virtual	void			take						( );

	virtual	void			set_transform				( float4x4 const& transform );
	virtual	float4x4		get_transform				( );

	virtual	void			unload						( );

	inline	void			set_spotted_to_team			( game_team_id spoted_to_team ) { m_spoted_to_team = spoted_to_team; }
	inline	game_team_id	get_spotted_to_team			( ) { return m_spoted_to_team; }

	inline	void			set_carrier_id				( u8 carrier_id ) { m_carrier_id = carrier_id; }
	inline	u8				get_carrier_id				( ) { return m_carrier_id; }

	inline	bool			is_inserted					( ) { return m_is_inserted; }

	/* 0x0000 */	/* usable_object */
	/* 0x0020 */	/* resources::unmanaged_resource */
	/* 0x0128 */	u8				id;

protected:
	/* 0x012c */	float4x4		m_transform;
	/* 0x016c */	bool			m_is_inserted;
	/* 0x0170 */	game_team_id	m_spoted_to_team;
	/* 0x0174 */	u8				m_carrier_id;
}; // class victory_item_core

STATIC_SIZE_ASSERT(victory_item_core, 0x178);

} // namespace survarium

#endif // #ifndef VICTORY_ITEM_CORE_H_INCLUDED
