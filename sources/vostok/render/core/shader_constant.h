////////////////////////////////////////////////////////////////////////////
//	Created		: 31.03.2010
//	Author		: Armen Abroyan
//	Copyright (C) GSC Game World - 2010
////////////////////////////////////////////////////////////////////////////

#ifndef VOSTOK_RENDER_CORE_SHADER_CONSTANT_H_INCLUDED
#define VOSTOK_RENDER_CORE_SHADER_CONSTANT_H_INCLUDED

#include <vostok/render/core/shader_constant_defines.h>
#include <vostok/render/core/shader_constant_slot.h>
#include <vostok/render/core/shader_constant_source.h>
#include <vostok/render/core/shader_constant_host.h>

namespace vostok {
namespace render {

class shader_constant_host;

class shader_constant {
public:
	inline	explicit						shader_constant	( shader_constant_host const& host );
	inline									shader_constant	( shader_constant const& other );
	inline	shader_constant&				operator =		( shader_constant const& other );
	inline	void							set_source		( shader_constant_source const& source );
	inline	shader_constant_slot const&		slot			( ) const;
	inline	shader_constant_source const&	source			( ) const;
	inline	shader_constant_host const&		host			( ) const;
	inline	shader_constant_host&			get_host		( ) const;

public:
	shader_constant_slot		m_slot;
	shader_constant_source		m_source;
	shader_constant_host const&	m_host;
}; // class shader_constant

	inline	bool							operator ==		( shader_constant const& left, shader_constant const& right );
	inline	bool							operator !=		( shader_constant const& left, shader_constant const& right );
} // namespace render
} // namespace vostok

#include <vostok/render/core/shader_constant_inline.h>

#endif // #ifndef VOSTOK_RENDER_CORE_SHADER_CONSTANT_H_INCLUDED