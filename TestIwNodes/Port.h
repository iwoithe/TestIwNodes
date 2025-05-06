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
		
		/**
		 * Returns this port's id
		 * @return This port's id
		 */
		std::string id() const;

		/**
		 * Sets the id of this port to `id`
		 * @param id This port's new id
		 */
		void setId(std::string id);

		/**
		 * Returns the data which is held by this port
		 * @return The data as an `iw::Any`
		 */
		Any data() const;

		/**
		 * Sets the data of this port to `data`
		 * @tparam T The type of `data`
		 * @param data The new data for this port to hold
		 */
		template<typename T> void setData(T data)
		{
			setData(data);
		}

		/**
		 * Sets the data of this port to `data`
		 * @param data The new data for this port to hold
		 */
		void setData(Any data);

		/**
		 * The type of the data this port holds
		 * @return The type of this port's data
		 */
		const type_info& dataType();

		/**
		 * Get the default value this port holds
		 * @return The default value of this port
		 */
		Any defaultData() const;

		/**
		 * Set the default value that this port holds
		 * @param data The new default value
		 */
		void setDefaultData(Any data);

		/**
		 * Get the value this port is currently holding.
		 * Convenience method to use `port->get<...>(...)` instead of `port->data().get<...>(...)`
		 * @tparam T The datatype to get. Must be correct or `std::bad_any_cast` will be raised
		 * @return The casted data value
		 */
		template<typename T> T get() const
		{
			return m_data.get<T>();
		}

		/**
		 * Get if data has changed since last execution
		 * @return If the data is dirty
		 */
		bool isDirty() const;

		/**
		 * Set if the data this port holds is dirty, i.e. the data has changed
		 * @param isDirty The new value for `m_isDirty`
		 */
		void setIsDirty(bool isDirty);

		/**
		 * Get if this port is linked to any other port
		 * @return If this port is linked
		 */
		bool isLinked() const;

		/**
		 * Get if this port is not linked to any other port
		 * @return If this port is unlinked
		 */
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

