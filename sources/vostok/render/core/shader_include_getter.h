////////////////////////////////////////////////////////////////////////////
//	Created		: 28.01.2011
//	Author		: Nikolay Partas
//	Copyright (C) GSC Game World - 2011
////////////////////////////////////////////////////////////////////////////

#ifndef VOSTOK_RENDER_CORE_SHADER_INCLUDE_GETTER_H_INCLUDED
#define VOSTOK_RENDER_CORE_SHADER_INCLUDE_GETTER_H_INCLUDED

namespace vostok {
namespace render {

struct shader_include_getter
{
	virtual pcstr get_shader_include(pcstr include_file_name) const { return include_file_name;}
}; // struct shader_include_getter

} // namespace render
} // namespace vostok

#endif // #ifndef VOSTOK_RENDER_CORE_SHADER_INCLUDE_GETTER_H_INCLUDED