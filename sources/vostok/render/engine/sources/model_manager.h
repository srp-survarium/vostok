#ifndef VOSTOK_RENDER_ENGINE_MODEL_MANAGER_H_INCLUDED
#define VOSTOK_RENDER_ENGINE_MODEL_MANAGER_H_INCLUDED

namespace vostok {

namespace memory {

class chunk_reader;

} // namespace memory

namespace render {

class render_model;
class render_surface;

namespace model_factory {

render_model*	create_render_model		( u16 type );
render_surface*	create_render_surface	( u16 type );
void			destroy_render_model	( render_model* model );
void			destroy_render_surface	( render_surface* surface );
u16				get_model_type			( memory::chunk_reader& chunk );

} // namespace model_factory
} // namespace render
} // namespace vostok

#endif // #ifndef VOSTOK_RENDER_ENGINE_MODEL_MANAGER_H_INCLUDED
