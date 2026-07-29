#include "pch.h"
#include "model_manager.h"
#include "render_model.h"
#include "render_surface.h"

namespace vostok {
namespace render {
namespace model_factory {

// STATE[STUB]
void destroy_render_model( render_model* model )
{
	// FUNCTION BODY[0x781940]
	DELETE( model );
}

// STATE[STUB]
void destroy_render_surface( render_surface* surface )
{
	// FUNCTION BODY[0x781980]
	DELETE( surface );
}

// STATE[STUB]
u16 get_model_type( memory::chunk_reader& )
{
	// FUNCTION BODY[0x7819c0]
	return 0;
}

// STATE[STUB]
render_surface* create_render_surface( u16 )
{
	// FUNCTION BODY[0x781a90]
	return 0;
}

// STATE[STUB]
render_model* create_render_model( u16 )
{
	// FUNCTION BODY[0x781a00]
	return 0;
}

} // namespace model_factory
} // namespace render
} // namespace vostok
