////////////////////////////////////////////////////////////////////////////
//	Created		: 09.02.2010
//	Author		: Evgeniy Obertyukh
//	Copyright (C) GSC Game World - 2010
////////////////////////////////////////////////////////////////////////////

#ifndef TEXTURE_EDITOR_SOURCE_H_INCLUDED
#define TEXTURE_EDITOR_SOURCE_H_INCLUDED

using vostok::editor::controls::tree_view_source;
using vostok::editor::controls::tree_view;

#include <vostok/render/core/texture_options.h>

namespace vostok{
namespace editor{

ref class texture_editor_source_filter
{
public:
	texture_editor_source_filter(vostok::render::texture_options::ETType* filter_type)
	:m_filter_type(filter_type)
	{}

	vostok::render::texture_options::ETType* m_filter_type;
};//ref class texture_editor_source_filter


ref class texture_editor_source: public vostok::editor::controls::tree_view_source
{

#pragma region | Initialize | 


public:
						texture_editor_source	( );
						texture_editor_source	( Object^ filter );


#pragma endregion 

#pragma region |   Fields   | 


private:
	tree_view^								m_parent;
	vostok::render::texture_options::ETType	m_filter_type;
	static System::String^		c_source_textures_path = "resources/textures";


#pragma endregion 

#pragma region | Properties | 


public:
	virtual property	tree_view^		parent
	{
		tree_view^			get	()					{ return m_parent; }
		void				set	(tree_view^ value)	{ m_parent = value; }
	};
	property	Object^					filter
	{
		Object^				get()				{ return ""; }
		void				set(Object^ value);
	}


#pragma endregion

#pragma region |   Methods  | 


private:
	void	on_fs_iterator_ready	( vfs::vfs_locked_iterator const & fs_it );
	void	process_fs				( vfs::vfs_iterator const & it,  System::String^ file_path );

public:
	virtual void refresh();


#pragma endregion 

}; // class texture_editor_source

}//namespace editor
}//namespace vostok

#endif // #ifndef TEXTURE_EDITOR_SOURCE_H_INCLUDED