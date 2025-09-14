////////////////////////////////////////////////////////////////////////////
//	Created		: 18.12.2009
//	Author		: Sergey Prishchepa
//	Copyright (C) GSC Game World - 2009
////////////////////////////////////////////////////////////////////////////

#ifndef DIALOG_NODE_CONNECTION_EDITOR_H_INCLUDED
#define DIALOG_NODE_CONNECTION_EDITOR_H_INCLUDED

namespace vostok {
namespace dialog_editor {

	ref class dialog_document;

	public ref class dialog_node_connection_editor : public vostok::editor::controls::hypergraph::connection_editor
	{
	public:
						dialog_node_connection_editor(dialog_document^ d):m_document(d)	{};
		virtual void	execute(vostok::editor::controls::hypergraph::node^ src, vostok::editor::controls::hypergraph::node^ dst);

	private:
		dialog_document^ m_document;
	}; // ref class dialog_node_connection_editor
} // namespace dialog_editor
} // namespace vostok

#endif // #ifndef DIALOG_NODE_CONNECTION_EDITOR_H_INCLUDED