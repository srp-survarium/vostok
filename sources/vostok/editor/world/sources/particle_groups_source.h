////////////////////////////////////////////////////////////////////////////
//	Created		: 20.01.2010
//	Author		: Evgeniy Obertyukh
//	Copyright (C) GSC Game World - 2010
////////////////////////////////////////////////////////////////////////////

#ifndef PARTICLE_GROUPS_SOURCE_H_INCLUDED
#define PARTICLE_GROUPS_SOURCE_H_INCLUDED

using namespace System;
using namespace System::Windows::Forms;

using vostok::editor::controls::tree_view_source;
using vostok::editor::controls::tree_view;

namespace vostok
{
	namespace editor
	{
		ref class particle_graph_node;

		ref class particle_groups_source : tree_view_source
		{
		private:
			tree_view^						m_parent;

		public:
			virtual property tree_view^		parent
			{
				tree_view^			get	(){return m_parent;};
				void				set	(tree_view^ value){m_parent = value;};
			};
			
		private:
			void	on_fs_iterator_ready	(vostok::resources::fs_iterator fs_it);
			void	process_fs				(vostok::resources::fs_iterator it,  String^ file_path);

		public:
			virtual void refresh();

		}; // class particle_groups_source
	}//namespace editor
}//namespace vostok

#endif // #ifndef PARTICLE_GROUPS_SOURCE_H_INCLUDED