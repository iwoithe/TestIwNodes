#pragma once

#include <string>
#include <vector>

#include "iwdelete.h"

namespace iw
{
	class Port;

	class Node
	{
	public:
		Node();
		
		virtual ~Node();

		virtual void exec() {}

		void addPort(Port* port);

		bool beenVisited() const;

		void setBeenVisited(bool beenVisited);

		Port* port(int index);

		Port* port(std::string id);

		int indegree() const;

		void setIndegree(int indegree);

		std::vector<Port*> inputPorts();

		std::vector<Port*> outputPorts();

		void resetIndegree();

		bool isInput() const;

		bool isOutput() const;

		std::string name() const;

		void setName(std::string name);

		std::vector<Port*> ports();

		void updateInputPortData();

	protected:
		bool m_beenVisited;
		int m_indegree;
		std::string m_name;
		std::vector<Port*> m_ports;
	};
}
