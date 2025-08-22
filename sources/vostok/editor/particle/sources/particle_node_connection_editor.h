////////////////////////////////////////////////////////////////////////////
//	Created		: 20.01.2010
//	Author		: 
//	Copyright (C) GSC Game World - 2010
////////////////////////////////////////////////////////////////////////////

#ifndef PARTICLE_NODE_CONNECTION_EDITOR_H_INCLUDED
#define PARTICLE_NODE_CONNECTION_EDITOR_H_INCLUDED

namespace vostok {
	namespace particle_editor {

		ref class particle_graph_document;

		public ref class particle_node_connection_editor : public vostok::editor::controls::hypergraph::connection_editor
		{
		public:
			particle_node_connection_editor(particle_graph_document^ d):m_document(d)	{};
			virtual void	execute(vostok::editor::controls::hypergraph::node^ src, vostok::editor::controls::hypergraph::node^ dst);

		private:
			particle_graph_document^ m_document;
		}; // ref class particle_node_connection_editor
	} // namespace particle_editor
} // namespace vostok


#endif // #ifndef PARTICLE_NODE_CONNECTION_EDITOR_H_INCLUDED