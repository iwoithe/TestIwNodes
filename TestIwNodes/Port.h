#pragma once

#include <string>
#include <vector>

#include "iwany.h"

namespace iw
{
	class Node;

	enum PortType
	{
		INPUT_PORT = 0,
		OUTPUT_PORT = 1
	};

	class Port
	{
	public:
		Port(std::string id, Any defaultData, PortType type);

		~Port();

		std::string id() const;

		void setId(std::string id);

		Any data() const;

		template<typename T> void setData(T data)
		{
			setData(data);
		}

		void setData(Any data);

		const type_info& dataType();

		Any defaultData() const;

		void setDefaultData(Any data);

		template<typename T> T get() const
		{
			// Convience function to use port->get() instead of port->data().get()
			return m_data.get<T>();
		}

		bool isDirty() const;

		void setIsDirty(bool isDirty);

		bool isLinked() const;

		bool isUnlinked() const;

		std::vector<Port*> linkedInputPorts();

		std::vector<Port*> linkedOutputPorts();

		void linkPort(Port* port);

		Node* node();

		void setNode(Node* node);

		PortType type() const;

		void setType(PortType type);

		void reset();

	protected:
		std::string m_id;
		Any m_data;
		Any m_defaultData;
		bool m_isDirty;
		std::vector<Port*> m_linkedInputPorts;
		std::vector<Port*> m_linkedOutputPorts;
		Node* m_node;
		PortType m_type;
	};
};

