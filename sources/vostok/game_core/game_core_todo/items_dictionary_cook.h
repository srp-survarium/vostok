////////////////////////////////////////////////////////////////////////////
//	Created 	: 12.10.2025
////////////////////////////////////////////////////////////////////////////

#ifndef ITEMS_DICTIONARY_COOK_H_INCLUDED
#define ITEMS_DICTIONARY_COOK_H_INCLUDED

namespace survarium {

// STATE[STUB]
// bool survarium::dictionary_item::is_ammo()
bool dictionary_item::is_ammo( )
{
	return false;
}

	/* TYPEDEFS

	typedef
		base_project::resolve_link_object*
		iterator_type;

	typedef
		char[32]
		account_name_type;

	typedef
		inventory_item_instance*
		iterator_type;

	typedef
		player_skill*
		iterator_type;

	typedef
		relocate_item_descr*
		iterator_type;

	typedef
		scheduler::record*
		iterator_type;

	typedef
		u8*
		iterator_type;

	typedef
		vostok::collision::bone_collision_data const*
		iterator_type;

	typedef
		vostok::collision::bone_collision_data*
		iterator_type;

	typedef
		vostok::one_way_threads_channel<vostok::intrusive_spsc_queue<vostok::sound::sound_order,vostok::sound::sound_order,4>,vostok::intrusive_spsc_queue<vostok::sound::sound_order,vostok::sound::sound_order,4> >
		orders_channel_type;

	typedef
		vostok::one_way_threads_channel<vostok::intrusive_spsc_queue<vostok::sound::sound_response,vostok::sound::sound_response,4>,vostok::intrusive_spsc_queue<vostok::sound::sound_response,vostok::sound::sound_response,4> >
		responses_channel_type;

} // namespace survarium

#endif // #ifndef ITEMS_DICTIONARY_COOK_H_INCLUDED
