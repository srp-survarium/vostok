// SPDX-License-Identifier: GPL-3.0-or-later
#include "pch.h"
#include "stage.h"
#include <vostok/render/core/backend.h>
#include "renderer_context.h"
#include "shared_names.h"
namespace vostok {
namespace render {

stage::stage( renderer* in_renderer, renderer_context* in_context ) :
	m_context		( in_context ),
	m_renderer		( in_renderer ),
	m_enabled		( true ),
	m_prev_enabled	( true )
{
}

void stage::execute( )
{
}

void stage::set_enabled( bool enable )
{
	m_enabled = enable;
}

bool stage::is_enabled( ) const
{
	return m_enabled;
}

void stage::rm_near()
{
	D3D_VIEWPORT VP = {0};//	{0,0,T->get_width(),T->get_height(),0,0.02f };
	backend::ref().get_viewport( VP);
	VP.MinDepth = 0;
	VP.MaxDepth = 0.02f;
	backend::ref().set_viewport( VP);
}

void stage::rm_normal()
{
	D3D_VIEWPORT VP = {0};//	{0,0,T->get_width(),T->get_height(),0,1.f };
	backend::ref().get_viewport( VP);
	VP.MinDepth = 0;
	VP.MaxDepth = 1.0f;
	backend::ref().set_viewport( VP);
}

void stage::rm_far()
{
	D3D_VIEWPORT VP = {0};//	{0,0,T->get_width(),T->get_height(),0.99999f,1.f };
	backend::ref().get_viewport( VP);
	VP.MinDepth = 0.99999f;
	VP.MaxDepth = 1.0f;
	backend::ref().set_viewport( VP);
}

//!!!!!!!!!!!!!!!!!!!!!FIX THIS!!!!!!!!!!!!!!!!!!!!!!
void stage::u_compute_texgen_jitter(float4x4& m_Texgen_J)
{
	// place into	0..1 space
	float4x4			m_TexelAdjust = float4x4(
		float4(0.5f,	0.0f,	0.0f,	0.0f),
		float4(0.0f,	-0.5f,	0.0f,	0.0f),
		float4(0.0f,	0.0f,	1.0f,	0.0f),
		float4(0.0f,	0.0f,	0.0f,	1.0f)
	);
	m_Texgen_J = math::mul4x4(m_context->get_wvp()/*m_full_xform*/, m_TexelAdjust);

	// rescale - tile it
	float	scale_X			= float(backend::ref().target_width())	/ float(tex_jitter);
	float	scale_Y			= float(backend::ref().target_height()) / float(tex_jitter);
	float	offset			= (.0f / float(tex_jitter));
	m_TexelAdjust = create_scale(float3(scale_X, scale_Y, 1.f));
	m_TexelAdjust = math::mul4x4(math::create_translation(float3(offset, offset, 0)), m_TexelAdjust);
	m_Texgen_J = math::mul4x4(m_TexelAdjust, m_Texgen_J);
}

void stage::u_compute_texgen_screen(float4x4& m_Texgen)
{
// 	float	_w						= float(device::ref().get_width());
// 	float	_h						= float(device::ref().get_height());
// 	float	o_w						= (.5f / _w);
// 	float	o_h						= (.5f / _h);
	float4x4	m_TexelAdjust		= float4x4(
		float4(0.5f,		0.0f,			0.0f,	0.0f),
		float4(0.0f,		-0.5f,			0.0f,	0.0f),
		float4(0.0f,		0.0f,			1.0f,	0.0f),
		//float4(0.5f + o_w,	0.5f + o_h,		0.0f,	1.0f)
		float4(0.5f,	0.5f,		0.0f,	1.0f)
	);
	m_Texgen = math::mul4x4(m_context->get_wvp()/*m_full_xform*/, m_TexelAdjust);
}

} // namespace render
} // namespace vostok
