#ifndef FS_PATH_H_INCLUDED
#define FS_PATH_H_INCLUDED

#include <vostok/fs_path_string.h>

namespace vostok	{
namespace fs	{

VOSTOK_CORE_API	path_string		convert_to_native	(pcstr path);
VOSTOK_CORE_API	path_string		convert_to_portable (pcstr path);
VOSTOK_CORE_API	pstr 			convert_to_portable_in_place	(pstr const path);
VOSTOK_CORE_API	pstr 			convert_to_native_in_place		(pstr const path);
} // namespace fs
} // namespace vostok

#endif // FS_PATH_H_INCLUDED