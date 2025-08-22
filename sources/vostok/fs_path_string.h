#ifndef VOSTOK_FS_PATH_STRING_H_INCLUDED
#define VOSTOK_FS_PATH_STRING_H_INCLUDED

#include <vostok/fixed_string.h>

namespace vostok	{
namespace fs	{

static	u32	const max_path_length			= sizeof(string_path) / sizeof(char);
typedef	fixed_string< max_path_length >		path_string;

//template class VOSTOK_CORE_API fixed_string< max_path_length >;

} // namespace fs
} // namespace vostok

#endif // VOSTOK_FS_PATH_STRING_H_INCLUDED