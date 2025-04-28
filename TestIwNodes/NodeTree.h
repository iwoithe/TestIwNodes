#pragma once

#include <queue>
#include <vector>

#include "iwdelete.h"


namespace iw
{
	class Node;
	class Port;

	class NodeTree
	{
	public:
		~NodeTree();

		void addNode(Node* node);

		void addOutputNode(Node* node);

		void exec();

		std::vector<Node*> nodes();

		std::vector<Node*> outputNodes();

	protected:
		void findInputNode(Node* node, std::queue<Node*>& inputNodes);

		std::vector<Node*> m_nodes;
		std::vector<Node*> m_outputNodes;
	};
}
