////////////////////////////////////////////////////////////////////////////
//	Created		: 18.08.2010
//	Author		: Sergey Pryshchepa
//	Copyright (C) GSC Game World - 2010
////////////////////////////////////////////////////////////////////////////
#ifndef CHANNELS_FILES_SOURCE_H_INCLUDED
#define CHANNELS_FILES_SOURCE_H_INCLUDED

using namespace System;
using namespace System::Windows::Forms;
using vostok::editor::controls::tree_view_source;
using vostok::editor::controls::tree_view;

namespace vostok {
namespace animation_editor {
	ref class channels_node_sorter : public System::Collections::IComparer 
	{
	public:
		virtual int Compare	(Object^ x, Object^ y);
	}; // ref class channels_node_sorter

	ref class channels_files_source : public vostok::editor::controls::tree_view_source
	{
	public:
								channels_files_source	();
		virtual					~channels_files_source	();
		virtual	void			refresh					();
		virtual property		tree_view^		parent
		{
			tree_view^			get						();
			void				set						(tree_view^ value);
		};
		
				void			fs_callback				(vostok::vfs::vfs_notification const & info);

	private:
				void	on_fs_iterator_ready			(vostok::vfs::vfs_locked_iterator const & fs_it);
				void	process_fs						(vostok::vfs::vfs_iterator const & it,  String^ file_path);

	private:
		tree_view^						m_parent;
		file_watcher_delegate*			m_callback;
		u32								m_watcher_subscribe_id;
	}; // class channels_files_source
}//namespace animation_editor
}//namespace vostok
#endif // #ifndef CHANNELS_FILES_SOURCE_H_INCLUDED