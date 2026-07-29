#ifndef VOSTOK_RENDER_ENGINE_SCENE_VIEW_H_INCLUDED
#define VOSTOK_RENDER_ENGINE_SCENE_VIEW_H_INCLUDED

#include <vostok/render/engine/base_classes.h>
#include <vostok/scaleform/sources/flash_movie_resource.h>

namespace survarium {

struct flash_text_manager;

} // namespace survarium

namespace vostok {
namespace render {

class scene_view : public base_scene_view {
public:
	scene_view( );
	virtual ~scene_view( );

	void add_movie( survarium::flash_movie_resource_ptr& movie );
	void remove_movie( survarium::flash_movie_resource_ptr& movie );
	void add_text_manager( survarium::flash_text_manager* tm );
	void remove_text_manager( survarium::flash_text_manager* tm );
};

} // namespace render
} // namespace vostok

#endif // #ifndef VOSTOK_RENDER_ENGINE_SCENE_VIEW_H_INCLUDED
