#ifndef VOSTOK_UI_API_H_INCLUDED
#define VOSTOK_UI_API_H_INCLUDED

#include <vostok/render/engine/base_classes.h>

#ifndef VOSTOK_UI_API
#	ifdef VOSTOK_STATIC_LIBRARIES
#		define VOSTOK_UI_API
#	else // #ifdef VOSTOK_STATIC_LIBRARIES
#		ifdef VOSTOK_UI_BUILDING
#			define VOSTOK_UI_API				VOSTOK_DLL_EXPORT
#		else // #ifdef VOSTOK_UI_BUILDING
#			ifndef VOSTOK_ENGINE_BUILDING
#				define VOSTOK_UI_API			VOSTOK_DLL_IMPORT
#			else // #ifndef VOSTOK_ENGINE_BUILDING
#				define VOSTOK_UI_API			VOSTOK_DLL_EXPORT
#			endif // #ifndef VOSTOK_ENGINE_BUILDING
#		endif // #ifdef VOSTOK_UI_BUILDING
#	endif // #ifdef VOSTOK_STATIC_LIBRARIES
#endif // #ifndef VOSTOK_UI_API


namespace vostok {

namespace input {
	struct world;
} // namespace input

namespace render {
	class world;

	namespace ui {
		class renderer;
	} // namespace ui
} // namespace input

namespace ui {

struct engine;
struct world;

typedef memory::doug_lea_allocator_type	allocator_type;

VOSTOK_UI_API	world*	create_world		(
						engine& engine,
						render::ui::renderer& renderer,
						memory::base_allocator& allocator,
						render::scene_view_ptr const& scene_view
					);
VOSTOK_UI_API	void	destroy_world		( world*& world);

} // namespace ui
} // namespace vostok

#endif // #ifndef VOSTOK_UI_API_H_INCLUDED