////////////////////////////////////////////////////////////////////////////
//	Created		: 22.02.2011
//	Author		: Sergey Chechin
//	Copyright (C) GSC Game World - 2011
////////////////////////////////////////////////////////////////////////////

#ifndef FS_PHYSICAL_PATH_ITERATOR_H_INCLUDED
#define FS_PHYSICAL_PATH_ITERATOR_H_INCLUDED

#include <vostok/fs/native_path_string.h>
#include <vostok/fs/physical_path_info.h>

namespace vostok {
namespace fs_new {

class physical_path_iterator : public physical_path_info, public core::noncopyable
{
public:
				physical_path_iterator	() : search_handle(u64(-1)) {}
				physical_path_iterator	(physical_path_initializer const & initializer) 
					: search_handle(initializer.search_handle), physical_path_info(initializer) {}

				~physical_path_iterator	();

	void		operator ++				();
	bool		operator !=				(physical_path_iterator const & other) 
										{ return search_handle != other.search_handle; }

private:
	void		initialize_path			() const;

private:
	u64			search_handle;	// for internal use

}; // physical_path_iterator

} // namespace fs_new
} // namespace vostok

#endif // #ifndef FS_PHYSICAL_PATH_ITERATOR_H_INCLUDED