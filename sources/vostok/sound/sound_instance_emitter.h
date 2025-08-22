////////////////////////////////////////////////////////////////////////////
//	Created		: 18.02.2011
//	Author		: Tetyana Meleshchenko
//	Copyright (C) GSC Game World - 2011
////////////////////////////////////////////////////////////////////////////

#ifndef VOSTOK_SOUND_SOUND_INSTANCE_EMITTER_H_INCLUDED
#define VOSTOK_SOUND_SOUND_INSTANCE_EMITTER_H_INCLUDED

#include <vostok/sound/encoded_sound_interface.h>

namespace vostok {
namespace sound {

class world_user;
class sound_instance;
class sound_instance_proxy;
class sound_spl;

typedef	resources::resource_ptr < sound_spl, resources::unmanaged_intrusive_base > sound_spl_ptr;

class VOSTOK_NOVTABLE sound_instance_emitter
{
public:
	virtual	void	emit_sound_instances	(
						sound_instance_proxy* proxy,
						world_user& user,
						float composite_sounds_offset = 0.f
					) const = 0;

	virtual encoded_sound_child_ptr const&	get_quality_for_resource( world& world, resources::positional_unmanaged_resource const* const resource ) const = 0;
	virtual sound_spl_ptr const&			get_sound_spl			( ) const = 0;
protected:
	VOSTOK_DECLARE_PURE_VIRTUAL_DESTRUCTOR	( sound_instance_emitter );
}; // class sound_instance_emitter

} // namespace sound
} // namespace vostok

#endif // #ifndef VOSTOK_SOUND_SOUND_INSTANCE_EMITTER_H_INCLUDED