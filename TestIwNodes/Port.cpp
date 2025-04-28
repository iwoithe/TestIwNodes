#include "Port.h"

using namespace iw;

Port::Port(std::string id, Any defaultData, PortType type) : m_id{ id }, m_defaultData{ defaultData }, m_isDirty{ false }, m_type{ type }
{
	reset();
}

Port::~Port()
{
	m_node = nullptr;
}

std::string Port::id() const
{
	return m_id;
}

void Port::setId(std::string id)
{
	m_id = id;
}

Any Port::data() const
{
	return m_data;
}

void Port::setData(Any data)
{
	m_data = data;
	m_isDirty = true;
}

Any Port::defaultData() const
{
	return m_defaultData;
}

void Port::setDefaultData(Any data)
{
	m_defaultData = data;
}

bool Port::isDirty() const
{
	return m_isDirty;
}

void Port::setIsDirty(bool isDirty)
{
	m_isDirty = isDirty;
}

bool Port::isLinked() const
{
	return m_linkedInputPorts.size() > 0 || m_linkedOutputPorts.size() > 0;
}

bool Port::isUnlinked() const
{
	return m_linkedInputPorts.empty() && m_linkedOutputPorts.empty();
}

std::vector<Port*> Port::linkedInputPorts()
{
	return m_linkedInputPorts;
}

std::vector<Port*> Port::linkedOutputPorts()
{
	return m_linkedOutputPorts;
}

void Port::linkPort(Port* port)
{
	if (m_type == PortType::INPUT_PORT && port->type() == PortType::OUTPUT_PORT) {
		m_linkedOutputPorts.push_back(port);
		port->m_linkedInputPorts.push_back(this);
	}
	else if (m_type == PortType::OUTPUT_PORT && port->type() == PortType::INPUT_PORT) {
		m_linkedInputPorts.push_back(port);
		port->m_linkedOutputPorts.push_back(this);
	}
}

Node* Port::node()
{
	return m_node;
}

void Port::setNode(Node* node)
{
	m_node = node;
}

PortType Port::type() const
{
	return m_type;
}

void Port::setType(PortType type)
{
	m_type = type;
}

void Port::reset()
{
	m_data = m_defaultData;
}
