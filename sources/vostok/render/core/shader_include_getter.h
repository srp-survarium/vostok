#ifndef VOSTOK_RENDER_CORE_SHADER_INCLUDE_GETTER_H_INCLUDED
#define VOSTOK_RENDER_CORE_SHADER_INCLUDE_GETTER_H_INCLUDED

namespace vostok {
namespace render {

struct shader_include_getter {
	virtual pcstr get_shader_include( pcstr include_file_name ) const
	{
		return include_file_name;
	}
};

STATIC_SIZE_ASSERT( shader_include_getter, 0x4 );

} // namespace render
} // namespace vostok

#endif // #ifndef VOSTOK_RENDER_CORE_SHADER_INCLUDE_GETTER_H_INCLUDED
