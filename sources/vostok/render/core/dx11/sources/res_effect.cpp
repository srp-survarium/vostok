#include "pch.h"
#include <vostok/render/core/res_effect.h>
#include <vostok/render/core/res_state.h>
#include <vostok/render/core/res_xs.h>
#include <vostok/render/core/dx11/res_input_layout.h>
#include <vostok/render/core/effect_manager.h>
#include <vostok/render/core/backend.h>

namespace vostok {
namespace render {

// claude@NOTE: declared in res_pass.h with no legacy out-of-line ancestor
// (legacy res_pass ctor was header-inline); init-list ctor for the
// effect_compiler end_pass construction path.
res_pass::res_pass(
	res_vs_ptr const& vs,
	res_gs_ptr const& gs,
	res_ps_ptr const& ps,
	res_state_ptr const& state
) :
	m_state	( state ),
	m_vs	( vs ),
	m_gs	( gs ),
	m_ps	( ps )
{
}

s32 compare( res_pass const&, res_pass const& )
{
	// claude@NOTE: legacy body diverged - legacy is member bool res_pass::equal, not a free s32 compare (equal->compare refactor, see legacy remainder note); matcher-phase work.
	// STATE[STUB]
	// FUNCTION BODY[0x6e7b10]
	return 0;
}

s32 compare( res_shader_technique const&, res_shader_technique const& )
{
	// claude@NOTE: legacy body diverged - legacy is member bool res_shader_technique::equal, not a free s32 compare; matcher-phase work.
	// STATE[STUB]
	// FUNCTION BODY[0x6e7b80]
	return 0;
}

void res_effect::push_texture_unique( res_texture*, pcstr )
{
	// claude@NOTE: no legacy ancestor - push_texture_unique has no legacy ancestor; matcher-phase work.
	// STATE[STUB]
	// FUNCTION BODY[0x6e7ce0]
}

void res_pass::apply() const
{
	m_vs->apply();
	m_gs->apply();
	m_ps->apply();

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
