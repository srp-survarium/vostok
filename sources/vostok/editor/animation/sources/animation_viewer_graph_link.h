////////////////////////////////////////////////////////////////////////////
//	Created		: 07.09.2010
//	Author		: Sergey Prishchepa
//	Copyright (C) GSC Game World - 2010
////////////////////////////////////////////////////////////////////////////
#ifndef ANIMATION_VIEWER_GRAPH_LINK_H_INCLUDED
#define ANIMATION_VIEWER_GRAPH_LINK_H_INCLUDED

using namespace System;
using namespace System::Collections;
using namespace System::ComponentModel;

namespace vostok {
namespace animation_editor {

	public ref class animation_viewer_graph_link : public vostok::editor::controls::hypergraph::link
	{
		typedef vostok::editor::controls::hypergraph::link				super;
		typedef vostok::editor::controls::hypergraph::connection_point	connection_point;
		typedef vostok::editor::wpf_controls::property_container			wpf_property_container;

	public:
						animation_viewer_graph_link	(connection_point^ src, connection_point^ dst);
						~animation_viewer_graph_link();
wpf_property_container^	container					() {return m_container;};

		virtual	void	load						(vostok::configs::lua_config_value const& cfg);
		virtual	void	save						(vostok::configs::lua_config_value cfg);

	protected:
		wpf_property_container^			m_container;
	}; // ref class animation_viewer_graph_link
} // namespace animation_editor
} // namespace vostok
#endif // #ifndef ANIMATION_VIEWER_GRAPH_LINK_H_INCLUDED