////////////////////////////////////////////////////////////////////////////
//	Created		: 14.12.2011
//	Author		: Nikolay Partas
//	Copyright (C) GSC Game World - 2011
////////////////////////////////////////////////////////////////////////////

#ifndef VOSTOK_RENDER_ENGINE_SCENE_XBOX360_H_INCLUDED
#define VOSTOK_RENDER_ENGINE_SCENE_XBOX360_H_INCLUDED

//#include <vostok/particle/engine.h>
//#include <vostok/render/engine/vertex_colored.h>
#include <vostok/render/engine/base_classes.h>
//#include "render_model.h"
//#include "decal_instance.h"


namespace vostok {
namespace render {

struct editor_renderer_configuration;

class scene : public base_scene {
public:
								scene					( editor_renderer_configuration const& renderer_configuration );
}; // class scene


} // namespace render
} // namespace vostok

#endif // #ifndef VOSTOK_RENDER_ENGINE_SCENE_XBOX360_H_INCLUDED