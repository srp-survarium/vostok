////////////////////////////////////////////////////////////////////////////
//	Created		: 12.02.2009
//	Author		: Mykhailo Parfeniuk
//	Copyright (C) GSC Game World - 2009
////////////////////////////////////////////////////////////////////////////

// HARVEST REMAINDER (2026-07-31): everything else ported into
// sources/vostok/render/core/dx11/sources/res_effect.cpp or superseded by the
// canonical inline bodies in res_effect.h / res_pass.h / res_shader_technique.h.
// The two `equal` bodies below are kept as the ONLY surviving ancestors of the
// still-stubbed free functions
//   s32 compare( res_pass const&, res_pass const& )                   [0x6e7b10]
//   s32 compare( res_shader_technique const&, res_shader_technique const& ) [0x6e7b80]
// (equal -> compare rename in the shipped generation; matcher-phase work).

#include "pch.h"
#include <vostok/render/core/res_effect.h>

namespace vostok {
namespace render {

bool res_pass::equal(const res_pass& other) const
{
	return	m_state	== other.m_state
			&& m_ps	== other.m_ps
			&& m_vs	== other.m_vs
			&& m_gs	== other.m_gs
			&& m_input_layout == other.m_input_layout;
}

bool res_shader_technique::equal(const res_shader_technique& other) const
{
	if (m_flags.priority != other.m_flags.priority) return false;
	if (m_flags.strict_b2f != other.m_flags.strict_b2f) return false;
	if (m_flags.has_emissive != other.m_flags.has_emissive) return false;
	if (m_flags.has_wmark != other.m_flags.has_wmark) return false;
	if (m_flags.has_distort != other.m_flags.has_distort) return false;

	if (m_passes.size() != other.m_passes.size()) return false;

	for (u32 p = 0; p < m_passes.size(); ++p)
		if (m_passes[p] != other.m_passes[p])
			return false;

	return true;
}

} // namespace render
} // namespace vostok
