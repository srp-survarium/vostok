#ifndef VOSTOK_RENDER_ENGINE_RENDERER_CONTEXT_H_INCLUDED
#define VOSTOK_RENDER_ENGINE_RENDERER_CONTEXT_H_INCLUDED

#include <vostok/render/engine/base_classes.h>

namespace vostok {
namespace render {

class renderer_context_targets;
class scene;
class scene_view;

class renderer_context {
public:
	renderer_context( );
	inline ~renderer_context( ) { }

	void set_scene( scene* scene );
	scene_view const* scene_view( ) const;
	render::scene_view* get_scene_view( );
	void set_scene_view( base_scene_view_ptr view_ptr );

	void set_view2shadow( float4x4 const& view2shadow, u32 index );
	float4x4 const& get_view2shadow( u32 index ) const;

	void set_target_context( renderer_context_targets const* targets_context, bool force_set );
	void set_time_delta( float time_delta );
	void set_current_time( float current_time );
	void set_w( float4x4 const& matrix );
	void set_v( float4x4 const& matrix );
	void set_p( float4x4 const& matrix );
	void reset_matrices( );
	void update_eye_rays( );
	void clear_resources( );

private:
	void update_near_far( );
};

} // namespace render
} // namespace vostok

#endif // #ifndef VOSTOK_RENDER_ENGINE_RENDERER_CONTEXT_H_INCLUDED
