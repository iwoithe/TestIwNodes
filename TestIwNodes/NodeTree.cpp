#include "NodeTree.h"

#include "Node.h"
#include "Port.h"

#define EMPTY_QUEUE(queue) while (!(queue).empty()) { (queue).pop(); }

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

void NodeTree::compile()
{
    // Step 1: Clear the node execution order and reset the nodes
    for (Node* node : m_nodes) {
        node->setBeenVisited(false);
        node->resetIndegree();

        for (Port* port : node->ports()) {
            port->setIsDirty(false);
        }
    }

    m_nodeExecOrder.clear();

    // Step 2: Get all input nodes
    std::queue<Node*> inputNodes;

    for (Node* outputNode : m_outputNodes) {
        findInputNode(outputNode, inputNodes);
    }

    // Step 3: Perform a topological sort based on Kahn's algorithm
    while (!inputNodes.empty())
    {
        Node* node = inputNodes.front();

        if (node->beenVisited()) {
            inputNodes.pop();
            continue;
        }

        node->setBeenVisited(true);

        m_nodeExecOrder.push_back(node);
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
    }
}

void NodeTree::exec()
{
    for (Node* node : m_nodeExecOrder)
    {
        node->updateInputPortData();
        node->exec();
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
