////////////////////////////////////////////////////////////////////////////
//	Created 	: 17.11.2008
//	Author		: Dmitriy Iassenev
//	Copyright (C) GSC Game World - 2009
////////////////////////////////////////////////////////////////////////////

#ifndef VOSTOK_RENDER_FACADE_ENGINE_RENDERER_H_INCLUDED
#define VOSTOK_RENDER_FACADE_ENGINE_RENDERER_H_INCLUDED

#include <vostok/render/api.h>

namespace vostok {
namespace render {

class world;

namespace engine {

class world;

class VOSTOK_RENDER_API renderer : public core::noncopyable {
private:
	friend class render::world;
	explicit	renderer	( engine::world& engine_world );

public:
	void		initialize	( bool is_editor );
	u32			frame_id	( );
	inline		~renderer	( ) { }

private:
	/* 0x0000 */	engine::world&	m_render_engine_world;
}; // class renderer

STATIC_SIZE_ASSERT( renderer, 0x4 );

} // namespace engine
} // namespace render
} // namespace vostok

#endif // VOSTOK_RENDER_FACADE_ENGINE_RENDERER_H_INCLUDED
