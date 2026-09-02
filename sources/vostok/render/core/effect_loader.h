// SPDX-License-Identifier: GPL-3.0-or-later
#ifndef VOSTOK_RENDER_CORE_EFFECT_LOADER_H_INCLUDED
#define VOSTOK_RENDER_CORE_EFFECT_LOADER_H_INCLUDED
#include <vostok/render/core/res_effect.h>

namespace vostok {

namespace resources {
class queries_result;
} // namespace resources

namespace render {

struct effect_loader {
	explicit effect_loader( res_effect_ptr* effect )
		: effect_ptr( effect ), query_rejected( false )
	{
	}

	void on_effect_ready( resources::queries_result& data );

	res_effect_ptr* effect_ptr;
	bool query_rejected;
};

STATIC_SIZE_ASSERT( effect_loader, 0x8 );

} // namespace render
} // namespace vostok

#endif // #ifndef VOSTOK_RENDER_CORE_EFFECT_LOADER_H_INCLUDED
