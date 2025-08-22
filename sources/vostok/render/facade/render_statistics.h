////////////////////////////////////////////////////////////////////////////
//	Created		: 23.05.2011
//	Author		: Nikolay Partas
//	Copyright (C) GSC Game World - 2011
////////////////////////////////////////////////////////////////////////////

#ifndef VOSTOK_RENDER_RENDER_STATISTICS_H_INCLUDED
#define VOSTOK_RENDER_RENDER_STATISTICS_H_INCLUDED

namespace vostok {
	namespace ui{
		struct world;
	}
namespace render {

class render_statistics
{
	virtual void render(vostok::ui::ui_world, u32 x, u32 y) = 0;
};

} // namespace render
} // namespace vostok

#endif // VOSTOK_RENDER_RENDER_STATISTICS_H_INCLUDED