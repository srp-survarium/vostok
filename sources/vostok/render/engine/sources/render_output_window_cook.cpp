#include "pch.h"
// claude@NOTE: legacy-harvest disposition: legacy cook implemented the retired resources::unmanaged_cook contract (see triage_log 'Obsolete predecessor interfaces'); no body maps onto translate_query/delete_resource - matcher-phase work.
#include "render_output_window_cook.h"

namespace vostok {
namespace render {

// STATE[STUB]
render_output_window_cook::render_output_window_cook( ) :
	resources::translate_query_cook(
		resources::render_output_window_class,
		reuse_false,
		use_current_thread_id
	)
{
	// FUNCTION BODY[0x76ac70]
	// ******
}

// STATE[STUB]
void render_output_window_cook::translate_query( resources::query_result_for_cook& parent )
{
	// LOCALS
	// output_window_configuration 		window_configuration
	// ******

	// FUNCTION BODY[0x76ad40]: 8
	// <0>
	// <0x76ad49>|0x009|+0x034:'22'
	// <0>
	// <0x76ad7d>|0x03d|+0x023:'24'
	// <0x76ada0>|0x060|+0x02c:'25'
	// <0>
	// <0x76adcc>|0x08c|+0x01f:'27'
	// <0x76adeb>|0x0ab|+0x00a:'28'
	// ******
}

// STATE[STUB]
void render_output_window_cook::delete_resource( resources::resource_base* resource )
{
	// CALL SITE INFO
	// <0x76ad20> -> void* < unknown >( u32 )
	// ******

	// FUNCTION BODY[0x76acd0]: 5
	// <0>
	// <1>
	// <0x76acd2>|0x002|+0x031:'35'
	// <0>
	// <0x76ad03>|0x033|+0x034:'37'
	// ******
}

} // namespace render
} // namespace vostok
