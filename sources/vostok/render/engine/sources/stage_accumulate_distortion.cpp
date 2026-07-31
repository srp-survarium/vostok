#include "pch.h"
#include "stage_accumulate_distortion.h"

#include <vostok/render/core/options.h>

namespace vostok {
namespace render {

stage_accumulate_distortion::stage_accumulate_distortion(
	renderer*			in_renderer,
	renderer_context*	context
) :
	stage( in_renderer, context )
{
	// FUNCTION BODY[0x7489c0]
	m_enabled = options::ref().current.m_enabled_distortion_stage;
}

stage_accumulate_distortion::~stage_accumulate_distortion( )
{
	// FUNCTION BODY[0x7489b0]
}

void stage_accumulate_distortion::execute( )
{
	// STATE[STUB]
	// FUNCTION BODY[0x7489f0]
	// claude@NOTE: legacy execute blocked - scene::select_models signature drift (5-arg now) and
	// the legacy m_dynamic_visuals member was dropped from the canonical stage; particle loop also
	// depends on render_particle_emitter_instance/material_effects wiring - matcher-phase
}

} // namespace render
} // namespace vostok
