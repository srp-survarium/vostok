////////////////////////////////////////////////////////////////////////////
//	Created		: 12.10.2011
//	Author		: Evgeniy Obertyukh
//	Copyright (C) GSC Game World - 2011
////////////////////////////////////////////////////////////////////////////

#ifndef ANIMATION_ITEM_TYPE_H_INCLUDED
#define ANIMATION_ITEM_TYPE_H_INCLUDED

#include <vostok/resources_classes.h>

namespace vostok
{
	namespace animation_editor
	{
		enum class animation_item_type
		{
			animation_single_item		= vostok::resources::single_animation_class,
			animation_composite_item	= vostok::resources::composite_animation_class,
			animation_collection_item	= vostok::resources::animation_collection_class,
		}; // class animation_item_type
	} // namespace animation_editor
} // namespace vostok

#endif // #ifndef ANIMATION_ITEM_TYPE_H_INCLUDED