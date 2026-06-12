////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#ifndef GAME_WORLD_BULLET_TRACER_H_INCLUDED
#define GAME_WORLD_BULLET_TRACER_H_INCLUDED

/* INCLUDES */
class survarium::bullet;
typedef vostok::resources::resource_ptr<vostok::render::tracer_model_instance,vostok::resources::unmanaged_intrusive_base>
	vostok::render::tracer_model_instance_ptr;
class vostok::render::tracer_model_instance;

namespace survarium {

struct game_world::bullet_tracer {
	inline		bullet_tracer	( bullet* arg_0, render::tracer_model_instance_ptr arg_1 ) { /* no source */ }
	inline		~bullet_tracer	( ) { /* no source */ }

public:
	/* 0x0000 */	bullet*									bullet;
	/* 0x0004 */	render::tracer_model_instance_ptr		tracer;
}; // struct game_world::bullet_tracer

STATIC_SIZE_ASSERT(game_world::bullet_tracer, 0x8);

} // namespace survarium

#endif // #ifndef GAME_WORLD_BULLET_TRACER_H_INCLUDED
