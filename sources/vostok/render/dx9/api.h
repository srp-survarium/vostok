////////////////////////////////////////////////////////////////////////////
//	Created 	: 28.10.2008
//	Author		: Dmitriy Iassenev
//	Copyright (C) GSC Game World - 2009
////////////////////////////////////////////////////////////////////////////

#ifndef VOSTOK_RENDER_DX9_API_H_INCLUDED
#define VOSTOK_RENDER_DX9_API_H_INCLUDED

namespace vostok {

namespace render {
	struct world;

	namespace engine {
		struct wrapper;
	} // namespace engine
} // namespace render

namespace render_dx9 {

	VOSTOK_RENDER_API	render::world*	create_world	( render::engine::wrapper& engine, HWND window_handle );
	VOSTOK_RENDER_API	void			destroy_world	( render::world*& world );

} // namespace render_dx9

} // namespace vostok


#endif // #ifndef VOSTOK_RENDER_DX9_API_H_INCLUDED