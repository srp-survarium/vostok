#ifndef VOSTOK_RENDER_ENGINE_SCENE_VIEW_COOK_H_INCLUDED
#define VOSTOK_RENDER_ENGINE_SCENE_VIEW_COOK_H_INCLUDED

#include <vostok/resources_cook_classes.h>

namespace vostok {
namespace render {

class scene_view_cook : public resources::translate_query_cook {
public:
	scene_view_cook( );
	virtual ~scene_view_cook( ) { }

	virtual void translate_query( resources::query_result_for_cook& parent ) override;
	virtual void delete_resource( resources::resource_base* resource ) override;
};

STATIC_SIZE_ASSERT( scene_view_cook, 0x20 );

} // namespace render
} // namespace vostok

#endif // #ifndef VOSTOK_RENDER_ENGINE_SCENE_VIEW_COOK_H_INCLUDED
