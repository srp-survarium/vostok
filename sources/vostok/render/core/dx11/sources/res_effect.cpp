// SPDX-License-Identifier: GPL-3.0-or-later
#include "pch.h"
#include <vostok/render/core/res_effect.h>
#include <vostok/render/core/res_state.h>
#include <vostok/render/core/res_xs.h>
#include <vostok/render/core/dx11/res_input_layout.h>
#include <vostok/render/core/effect_manager.h>
#include <vostok/render/core/backend.h>
namespace vostok {
namespace render {

res_pass::~res_pass()
{
}

void res_pass::destroy_impl	() const
{
	effect_manager::ref().delete_pass( this );
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

void res_pass::apply() const
{
	m_vs->apply();
	m_gs->apply();
	m_ps->apply();

	m_state->apply();
}

res_shader_technique::~res_shader_technique()
{
}

void res_shader_technique::destroy_impl() const
{
	effect_manager::ref().delete_effect_technique(this);
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

	for ( u32 i = 0, size = std::min( left.m_passes.size( ), right.m_passes.size( ) ); i < size; ++i )
	{
		result = compare( *left.m_passes[i], *right.m_passes[i] );
		if ( result )
			return result;
	}

	return 0;
}

res_effect::~res_effect()
{
}

bool res_effect::apply_pass(u32 id)
{
	res_shader_technique_ptr	technique = m_techniques[m_cur_technique];

	if (technique->m_passes.size() <= id)
		return false;

	res_pass_ptr pass = technique->m_passes[id];

	pass->apply();

	return true;
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

	texture_named_instance* found_texture = std::find_if(
		m_used_textures.begin( ),
		m_used_textures.end( ),
		find_texture_predicate( in_texture )
	);

	if ( found_texture == m_used_textures.end( ) )
	{
		texture_named_instance instance;
		instance.texture = in_texture;
		instance.path = path;
		m_used_textures.push_back( instance );
	}
}

} // namespace render
} // namespace vostok
