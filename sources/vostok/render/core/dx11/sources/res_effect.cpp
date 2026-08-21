#include "pch.h"
#include <vostok/render/core/res_effect.h>
#include <vostok/render/core/res_state.h>
#include <vostok/render/core/res_xs.h>
#include <vostok/render/core/dx11/res_input_layout.h>
#include <vostok/render/core/effect_manager.h>
#include <vostok/render/core/backend.h>

namespace vostok {
namespace render {

// Kept out-of-line to avoid the res_effect/res_xs include cycle.
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

s32 compare( res_pass const& left, res_pass const& right )
{
	s32 result = left.m_state < right.m_state ? -1 : right.m_state < left.m_state ? 1 : 0;
	if ( result )
		return result;

	result = left.m_ps < right.m_ps ? -1 : right.m_ps < left.m_ps ? 1 : 0;
	if ( result )
		return result;

	result = left.m_vs < right.m_vs ? -1 : right.m_vs < left.m_vs ? 1 : 0;
	if ( result )
		return result;

	result = left.m_gs < right.m_gs ? -1 : right.m_gs < left.m_gs ? 1 : 0;
	if ( result )
		return result;

	result = left.m_input_layout < right.m_input_layout ? -1
		: right.m_input_layout < left.m_input_layout ? 1 : 0;
	if ( result )
		return result;

	return 0;
}

s32 compare( res_shader_technique const& left, res_shader_technique const& right )
{
	s32 result = left.m_flags.priority < right.m_flags.priority ? -1
		: right.m_flags.priority < left.m_flags.priority ? 1 : 0;
	if ( result )
		return result;

	result = left.m_flags.strict_b2f < right.m_flags.strict_b2f ? -1
		: right.m_flags.strict_b2f < left.m_flags.strict_b2f ? 1 : 0;
	if ( result )
		return result;

	result = left.m_flags.has_emissive < right.m_flags.has_emissive ? -1
		: right.m_flags.has_emissive < left.m_flags.has_emissive ? 1 : 0;
	if ( result )
		return result;

	result = left.m_flags.has_wmark < right.m_flags.has_wmark ? -1
		: right.m_flags.has_wmark < left.m_flags.has_wmark ? 1 : 0;
	if ( result )
		return result;

	result = left.m_flags.has_distort < right.m_flags.has_distort ? -1
		: right.m_flags.has_distort < left.m_flags.has_distort ? 1 : 0;
	if ( result )
		return result;

	result = left.m_passes.size( ) < right.m_passes.size( ) ? -1
		: right.m_passes.size( ) < left.m_passes.size( ) ? 1 : 0;
	if ( result )
		return result;

	u32 const size = std::min( left.m_passes.size( ), right.m_passes.size( ) );
	for ( u32 i = 0; i < size; ++i )
	{
		result = compare( *left.m_passes[i], *right.m_passes[i] );
		if ( result )
			return result;
	}

	return 0;
}

void res_effect::push_texture_unique( res_texture* in_texture, pcstr path )
{
	struct find_texture_predicate {
		find_texture_predicate( res_texture_ptr const& texture ) : m_texture( texture ) { }

		bool operator()( texture_named_instance const& other ) const
		{
			return m_texture == other.texture;
		}

		res_texture_ptr m_texture;
	};

	if ( std::find_if(
		m_used_textures.begin( ),
		m_used_textures.end( ),
		find_texture_predicate( in_texture )
	) == m_used_textures.end( ) )
	{
		texture_named_instance instance;
		instance.texture = in_texture;
		instance.path = path;
		m_used_textures.push_back( instance );
	}
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
}

void res_pass::destroy_impl	() const
{
	effect_manager::ref().delete_pass( this );
}

res_shader_technique::~res_shader_technique()
{
}

void res_shader_technique::destroy_impl() const
{
	effect_manager::ref().delete_effect_technique(this);
}

bool res_effect::apply_pass(u32 id)
{
	res_shader_technique_ptr	technique = m_techniques[m_cur_technique];

	if (id >= technique->m_passes.size())
		return false;

	res_pass_ptr pass = technique->m_passes[id];

	pass->apply();

	return true;
}

res_effect::~res_effect()
{
}

} // namespace render
} // namespace vostok
