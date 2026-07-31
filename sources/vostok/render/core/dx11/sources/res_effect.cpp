#include "pch.h"
#include <vostok/render/core/res_effect.h>
#include <vostok/render/core/res_state.h>
#include <vostok/render/core/res_xs.h>
#include <vostok/render/core/dx11/res_input_layout.h>
#include <vostok/render/core/effect_manager.h>
#include <vostok/render/core/backend.h>

namespace vostok {
namespace render {

s32 compare( res_pass const&, res_pass const& )
{
	// STATE[STUB]
	// FUNCTION BODY[0x6e7b10]
	return 0;
}

s32 compare( res_shader_technique const&, res_shader_technique const& )
{
	// STATE[STUB]
	// FUNCTION BODY[0x6e7b80]
	return 0;
}

void res_effect::push_texture_unique( res_texture*, pcstr )
{
	// STATE[STUB]
	// FUNCTION BODY[0x6e7ce0]
}

void res_pass::apply() const
{
	// FUNCTION BODY[0x6e7e20]
	m_vs->apply();
	m_gs->apply();
	m_ps->apply();

	backend::ref().set_input_layout( m_input_layout.c_ptr());

	m_state->apply();
}

res_pass::~res_pass()
{
	// FUNCTION BODY[0x6e7e50]
}

void res_pass::destroy_impl	() const
{
	// FUNCTION BODY[0x6e7ee0]
	effect_manager::ref().delete_pass( this );
}

res_shader_technique::~res_shader_technique()
{
	// FUNCTION BODY[0x6e7ef0]
}

void res_shader_technique::destroy_impl() const
{
	// FUNCTION BODY[0x6e7f30]
	effect_manager::ref().delete_effect_technique(this);
}

bool res_effect::apply_pass(u32 id)
{
	// FUNCTION BODY[0x6e7f70]
	res_shader_technique_ptr	technique = m_techniques[m_cur_technique];

	ASSERT(id < technique->m_passes.size());

	res_pass_ptr pass = technique->m_passes[id];

	pass->apply();

	return true;
}

res_effect::~res_effect()
{
	// FUNCTION BODY[0x6e8010]
}

} // namespace render
} // namespace vostok
