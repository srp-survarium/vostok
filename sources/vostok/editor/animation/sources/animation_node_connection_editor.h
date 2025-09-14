////////////////////////////////////////////////////////////////////////////
//	Created		: 07.09.2010
//	Author		: Sergey Prishchepa
//	Copyright (C) GSC Game World - 2010
////////////////////////////////////////////////////////////////////////////

#ifndef ANIMATION_NODE_CONNECTION_EDITOR_H_INCLUDED
#define ANIMATION_NODE_CONNECTION_EDITOR_H_INCLUDED

namespace vostok {
namespace animation_editor {

	ref class viewer_document;

	public ref class animation_node_connection_editor : public vostok::editor::controls::hypergraph::connection_editor
	{
	public:
						animation_node_connection_editor(viewer_document^ d):m_document(d)	{};
		virtual void	execute(vostok::editor::controls::hypergraph::node^ src, vostok::editor::controls::hypergraph::node^ dst);

	private:
		viewer_document^ m_document;
	}; // ref class animation_node_connection_editor
} // namespace animation_editor
} // namespace vostok

#endif // #ifndef ANIMATION_NODE_CONNECTION_EDITOR_H_INCLUDED