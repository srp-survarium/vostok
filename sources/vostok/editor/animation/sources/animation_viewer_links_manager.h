////////////////////////////////////////////////////////////////////////////
//	Created		: 07.09.2010
//	Author		: Sergey Prishchepa
//	Copyright (C) GSC Game World - 2010
////////////////////////////////////////////////////////////////////////////

#ifndef ANIMATION_VIEWER_LINKS_MANAGER_H_INCLUDED
#define ANIMATION_VIEWER_LINKS_MANAGER_H_INCLUDED

using namespace System;
using namespace System::Collections;
using namespace System::ComponentModel;

namespace vostok {
namespace animation_editor {
	ref class animation_viewer_links_manager : public vostok::editor::controls::hypergraph::links_manager
	{
		typedef vostok::editor::controls::hypergraph::node				node;
		typedef vostok::editor::controls::hypergraph::link				link;
		typedef vostok::editor::controls::hypergraph::connection_point	connection_point;
		typedef Generic::List<link^>					links;
		typedef Generic::List<node^>					nodes;

	public:
						animation_viewer_links_manager(vostok::editor::controls::hypergraph::hypergraph_control^ h);
						~animation_viewer_links_manager();
		virtual void	on_node_added		(node^ node);
		virtual void	on_node_removed		(node^ node);
		virtual void	on_node_destroyed	(node^ node);
		virtual void	create_link			(vostok::editor::controls::hypergraph::connection_point^ pt_src, vostok::editor::controls::hypergraph::connection_point^ pt_dst);
		virtual void	destroy_link		(vostok::editor::controls::hypergraph::link^ link);
		virtual links^	visible_links		();
		virtual void	clear				();

	private:
		links^													m_links;
		vostok::editor::controls::hypergraph::hypergraph_control^	m_hypergraph;
	}; // ref class dialog_links_manager
} // namespace animation_editor
} // namespace vostok

#endif // #ifndef ANIMATION_VIEWER_LINKS_MANAGER_H_INCLUDED