#include <iostream>

#include "Node.h"
#include "NodeTree.h"
#include "Port.h"

using namespace iw;

class UpdateInputNode : public Node
{
public:
    UpdateInputNode()
    {
        m_name = "UpdateInput";

        Port* execOutPort = new Port("execOut", Any(0), PortType::OUTPUT_PORT);
        m_deltaTimePort = new Port("deltaTime", Any(0), PortType::OUTPUT_PORT);
        addPort(execOutPort);
        addPort(m_deltaTimePort);
    }

    void exec() override
    {
        // Set delta time
        m_deltaTimePort->setData(Any(0.1f));
    }

protected:
    Port* m_deltaTimePort;
};

class UpdateOutputNode : public Node
{
public:
    UpdateOutputNode()
    {
        m_name = "Output";

        Port* execInPort = new Port("execIn", Any(0), PortType::INPUT_PORT);
        addPort(execInPort);
    }
};

class PrintNode : public Node
{
public:
    PrintNode()
    {
        m_name = "Print";

        Port* execInPort = new Port("execIn", Any(0), PortType::INPUT_PORT);
        Port* execOutPort = new Port("execOut", Any(0), PortType::OUTPUT_PORT);
        
        m_valuePort = new Port("value", Any("Hello, World!"), PortType::INPUT_PORT);

        addPort(execInPort);
        addPort(execOutPort);
        addPort(m_valuePort);
    }

    void exec() override
    {
        std::cout << "[DEBUG] " << m_valuePort->get<float>() << std::endl;
    }

protected:
    Port* m_valuePort;
};


int main()
{
    NodeTree* nodeTree = new NodeTree();

    UpdateInputNode* updateInput = new UpdateInputNode();
    PrintNode* print = new PrintNode();
    UpdateOutputNode* updateOutput = new UpdateOutputNode();

    updateInput->port("execOut")->linkPort(print->port("execIn"));
    print->port("execOut")->linkPort(updateOutput->port("execIn"));

    updateInput->port("deltaTime")->linkPort(print->port("value"));

    nodeTree->addOutputNode(updateOutput);
    nodeTree->exec();

    delete nodeTree;

    return 0;
}
