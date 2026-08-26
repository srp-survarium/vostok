////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#ifndef OBJECT_LPV_OCCLUDER_H_INCLUDED
#define OBJECT_LPV_OCCLUDER_H_INCLUDED

#include "game_object_static.h"

namespace survarium {

class object_lpv_occluder : public game_object_static {
	typedef game_object_static super;

public:
			explicit	object_lpv_occluder	( base_game_scene& w );
	virtual				~object_lpv_occluder( );

	virtual	void		load				(
							configs::binary_config_value const&		t,
							pcstr									__formal,
							boost::function< void( game_object_& ) >&	cb
						) override;

	virtual	void		insert				( ) override;
	virtual	void		remove				( ) override;

public:
	/* 0x0000 */	/* game_object_static */
	/* 0x0150 */	u32		m_occluder_id;
}; // class object_lpv_occluder

STATIC_SIZE_ASSERT(object_lpv_occluder, 0x158);

} // namespace survarium

#endif // #ifndef OBJECT_LPV_OCCLUDER_H_INCLUDED
