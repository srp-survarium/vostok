////////////////////////////////////////////////////////////////////////////
//	Created		: 27.10.2011
//	Author		: Tetyana Meleshchenko
//	Copyright (C) GSC Game World - 2011
////////////////////////////////////////////////////////////////////////////

#ifndef VOSTOK_AI_SOUND_ITEM_H_INCLUDED
#define VOSTOK_AI_SOUND_ITEM_H_INCLUDED

namespace vostok {
namespace ai {

struct sound_item
{
	sound_item							( resources::unmanaged_resource_ptr const& new_emitter, pcstr filename );

	resources::unmanaged_resource_ptr	sound;
	fs_new::virtual_path_string			name;
}; // struct sound_item

struct sound_item_wrapper
{
	inline sound_item_wrapper			( sound_item const* const item ) :
		sound							( item ),
		next							( 0 )
	{
	}
	
	sound_item const*					sound;
	sound_item_wrapper*					next;
}; // struct sound_item_wrapper

typedef intrusive_list< sound_item_wrapper,
						sound_item_wrapper*,
						&sound_item_wrapper::next >	sounds_type;
typedef fixed_vector< sound_item const*, 32 >		sound_items_type;

} // namespace ai
} // namespace vostok

#endif // #ifndef VOSTOK_AI_SOUND_ITEM_H_INCLUDED