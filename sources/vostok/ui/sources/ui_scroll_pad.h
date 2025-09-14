////////////////////////////////////////////////////////////////////////////
//	Created		: 30.01.2009
//	Author		: Andrew Kolomiets
//	Copyright (C) GSC Game World - 2009
////////////////////////////////////////////////////////////////////////////
#ifndef UI_SCROLL_PAD_H_INCLUDED
#define UI_SCROLL_PAD_H_INCLUDED

#include "ui_window.h"

namespace vostok {
namespace ui {


class ui_scroll_pad : public ui_window
{
	typedef ui_window	super;
public:
					ui_scroll_pad			( memory::base_allocator& a );
	virtual void	draw					( vostok::render::ui::renderer& renderer, vostok::render::scene_view_ptr const& scene_view );
	virtual void	tick					( );
};

} // namespace vostok
} // namespace ui

#endif //UI_SCROLL_PAD_H_INCLUDED