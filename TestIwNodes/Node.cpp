#include "Node.h"

#include "Port.h"

using namespace iw;

Node::Node()
{
	m_beenVisited = false;
	m_indegree = 0;
}

Node::~Node()
{
	DEL_STD_VEC(m_ports);
}

void Node::addPort(Port* port)
{
	port->setNode(this);
	m_ports.push_back(port);

	if (port->type() == PortType::INPUT_PORT)
		m_indegree += 1;

	// TODO: Sort m_nodes alphabetically
}

bool Node::beenVisited() const
{
	return m_beenVisited;
}

void Node::setBeenVisited(bool beenVisited)
{
	m_beenVisited = beenVisited;
}

Port* Node::port(int index)
{
	return m_ports[index];
}

Port* Node::port(std::string id)
{
	// Brute-force search (m_nodes isn't alphabetically sorted yet, so can't use binary search)
	for (Port* port : m_ports) {
		if (port->id() == id)
			return port;
	}

	return nullptr;
}

int Node::indegree() const
{
	return m_indegree;
}

void Node::setIndegree(int indegree)
{
	m_indegree = indegree;
}

std::vector<Port*> Node::inputPorts()
{
	std::vector<Port*> inputPorts;

	for (Port* port : m_ports) {
		if (port->type() == PortType::INPUT_PORT)
			inputPorts.push_back(port);
	}

	return inputPorts;
}

std::vector<Port*> Node::outputPorts()
{
	std::vector<Port*> outputPorts;

	for (Port* port : m_ports) {
		if (port->type() == PortType::OUTPUT_PORT)
			outputPorts.push_back(port);
	}

	return outputPorts;
}

void Node::resetIndegree()
{
	m_indegree = 0;
	for (Port* port : m_ports) {
		if (port->type() == PortType::INPUT_PORT)
			m_indegree += 1;
	}
}

bool Node::isInput() const
{
	// Input nodes only have output ports
	for (Port* p : m_ports) {
		if (p->type() == PortType::INPUT_PORT)
			return false;
	}

	return true;
}

bool Node::isOutput() const
{
	// Output nodes only have output ports
	for (Port* p : m_ports) {
		if (p->type() == PortType::OUTPUT_PORT)
			return false;
	}

	return true;
}

std::string Node::name() const
{
	return m_name;
}

void Node::setName(std::string name)
{
	m_name = name;
}

std::vector<Port*> Node::ports()
{
	return m_ports;
}

void Node::updateInputPortData()
{
	for (Port* port : m_ports) {
		if (port->type() == PortType::INPUT_PORT)
		{
			std::vector<Port*> outputPorts = port->linkedOutputPorts();
			if (!outputPorts.empty())
				port->setData(port->linkedOutputPorts()[0]->data());
		}
	}
}

