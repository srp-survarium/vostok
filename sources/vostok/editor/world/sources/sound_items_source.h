////////////////////////////////////////////////////////////////////////////
//	Created		: 24.12.2009
//	Author		: Evgeniy Obertyukh
//	Copyright (C) GSC Game World - 2009
////////////////////////////////////////////////////////////////////////////

#ifndef SOUND_ITEMS_SOURCE_H_INCLUDED
#define SOUND_ITEMS_SOURCE_H_INCLUDED

using namespace System;
using namespace System::Windows::Forms;

using vostok::editor::controls::tree_view_source;
using vostok::editor::controls::tree_view;

namespace vostok
{
	namespace editor
	{
		ref class sound_items_source : tree_view_source
		{
		private:
			tree_view^ m_parent;

		public:
			virtual property tree_view^	parent
			{
				tree_view^			get	();
				void				set	(tree_view^ value);
			};

		private:
			void	on_fs_iterator_ready	(vostok::resources::fs_iterator fs_it);
			void	process_fs				(vostok::resources::fs_iterator it,  String^ file_path);

		public:
			virtual void refresh();

		}; // class sound_items_source
	}//namespace editor
}//namespace vostok
#endif // #ifndef SOUND_ITEMS_SOURCE_H_INCLUDED