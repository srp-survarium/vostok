// SPDX-License-Identifier: GPL-3.0-or-later
#ifndef VOSTOK_RENDER_CORE_SHADER_MACROS_H_INCLUDED
#define VOSTOK_RENDER_CORE_SHADER_MACROS_H_INCLUDED
#include <cstring>
#include <vostok/fixed_string.h>
#include <vostok/fixed_vector.h>
#include <vostok/fs/virtual_path_string.h>
#include <vostok/render/core/quasi_singleton.h>
#include <vostok/render/core/shader_configuration.h>

namespace vostok {
namespace render {

enum {
	shader_macros_max_cout = 128,
};

struct shader_macro {
	fs_new::virtual_path_string name;
	fixed_string<256> definition;
};

typedef fixed_vector<shader_macro, shader_macros_max_cout>
	shader_defines_list;
typedef fixed_vector<pcstr, shader_macros_max_cout>
	shader_defines_names_list;
typedef fixed_vector<fs_new::virtual_path_string, shader_macros_max_cout>
	shader_declarated_macroses_list;

struct shader_macros_dort_predicate {
	bool operator()( pcstr left, pcstr right )
	{
		return std::strcmp( left, right ) < 0;
	}
};

class shader_macros : public quasi_singleton<shader_macros> {
public:
	shader_macros( );

	void fill_shader_macro_list(
		shader_defines_list& macros,
		shader_configuration shader_config
	);

	void merge_with_declared_macroses(
		shader_declarated_macroses_list const& declared_macroses,
		shader_defines_list& macros
	);

	void append_macros_name( shader_defines_names_list& names, pcstr name )
	{
		names.push_back( name );
	}

private:
	void register_available_macros( );
	void fill_global_macros( shader_defines_list& defines );
	void fill_shader_configuration_macros(
		shader_defines_list& macros,
		shader_configuration shader_config
	);

private:
	shader_defines_names_list m_available_macros;
	shader_defines_list m_working_macro_list;
};

STATIC_SIZE_ASSERT( shader_macro, 0x220 );
STATIC_SIZE_ASSERT( shader_macros, 0x11210 );

} // namespace render
} // namespace vostok

#endif // #ifndef VOSTOK_RENDER_CORE_SHADER_MACROS_H_INCLUDED
