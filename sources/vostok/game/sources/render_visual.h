////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_VISUAL_H_INCLUDED
#define RENDER_VISUAL_H_INCLUDED

// PDB spells the model member vostok::render::static_model_instance_ptr - the
// same resource_ptr type our render tree typedefs as static_model_ptr
#include <vostok/render/facade/model.h>

namespace survarium {

class base_game_scene;

// survarium's own render_visual - a name collision with
// vostok::render::render_visual (render/engine/visual.h), NOT the same type
// (triage CHECK resolved in batch 7); insert/remove bodies live in the
// game_project.cpp compiland (still queued)
struct render_visual {
			void	insert			( base_game_scene& w );
			void	remove			( base_game_scene& w );


public:
	/* 0x0000 */	float4x4					matrix;
	/* 0x0040 */	render::static_model_ptr	model;
}; // struct render_visual

STATIC_SIZE_ASSERT(render_visual, 0x44);

} // namespace survarium

#endif // #ifndef RENDER_VISUAL_H_INCLUDED
