////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#ifndef OBJECT_AMBIENT_VOLUME_H_INCLUDED
#define OBJECT_AMBIENT_VOLUME_H_INCLUDED

#include "game_object_static.h"

namespace survarium {

class object_ambient_volume : public game_object_static {
	typedef game_object_static super;

public:
			explicit	object_ambient_volume	( base_game_scene& w );
	virtual				~object_ambient_volume	( );

	virtual	void		load					(
							configs::binary_config_value const&		t,
							pcstr									__formal,
							boost::function< void( game_object_& ) >&	cb
						) override;

	virtual	void		insert					( ) override;
	virtual	void		remove					( ) override;

	/* 0x0000 */	/* game_object_static */
	/* 0x0150 */	u32			m_id;
	/* 0x0154 */	float		m_ambient_multiplier;
	/* 0x0158 */	bool		m_enabled;
	/* 0x0159 */	bool		m_valid;
}; // class object_ambient_volume

STATIC_SIZE_ASSERT(object_ambient_volume, 0x160);

} // namespace survarium

#endif // #ifndef OBJECT_AMBIENT_VOLUME_H_INCLUDED
