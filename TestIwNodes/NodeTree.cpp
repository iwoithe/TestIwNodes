#include "NodeTree.h"

#include "Node.h"
#include "Port.h"

using namespace iw;

NodeTree::~NodeTree()
{
	DEL_STD_VEC(m_nodes);
}

void NodeTree::addNode(Node* node)
{
	m_nodes.push_back(node);
}

void NodeTree::addOutputNode(Node* node)
{
	m_outputNodes.push_back(node);
}

void NodeTree::exec()
{
	// Step 1: Get all input nodes
	std::queue<Node*> inputNodes;

	for (Node* outputNode : m_outputNodes) {
		findInputNode(outputNode, inputNodes);
	}

	// Step 2: Perform a topological sort based on Kahn's algorithm
	std::queue<Node*> nodeExecOrder;

	while (!inputNodes.empty())
	{
		Node* node = inputNodes.front();

		if (node->beenVisited()) {
			inputNodes.pop();
			continue;
		}

		node->setBeenVisited(true);

		nodeExecOrder.push(node);
		inputNodes.pop();

		for (Port* outputPort : node->outputPorts()) {
			for (Port* linkedPort : outputPort->linkedInputPorts()) {
				Node* linkedPortNode = linkedPort->node();
				linkedPortNode->setIndegree(linkedPortNode->indegree() - 1);
				if (linkedPortNode->indegree() <= 0) {
					inputNodes.push(linkedPortNode);
				}
			}
		}

		// Step 3: Run each node's execution method in correct order
		while (!nodeExecOrder.empty())
		{
			Node* node = nodeExecOrder.front();

			node->updateInputPortData();
			node->exec();

			// Step 4: Reset (avoids having to loop through nodes again later)
			node->setBeenVisited(false);
			node->resetIndegree();

			for (Port* port : node->ports()) {
				port->setIsDirty(false);
			}

			nodeExecOrder.pop();
		}
	}
}

std::vector<Node*> NodeTree::nodes()
{
	return m_nodes;
}

std::vector<Node*> NodeTree::outputNodes()
{
	return m_outputNodes;
}

void NodeTree::findInputNode(Node* node, std::queue<Node*>& inputNodes)
{
	for (Port* port : node->ports()) {
		if (port->type() == PortType::OUTPUT_PORT) continue;
		else {
			for (Port* p : port->linkedOutputPorts()) {
				Node* n = p->node();
				bool anyInputsLinked = false;
				for (Port* nextNodePort : n->ports())
				{
					if (nextNodePort->type() == PortType::OUTPUT_PORT) continue;
					else {
						if (nextNodePort->isLinked()) anyInputsLinked = true;
					}
				}

				if (anyInputsLinked) {
					findInputNode(n, inputNodes);
				}
				else
					inputNodes.push(n);
			}
		}
	}
}
