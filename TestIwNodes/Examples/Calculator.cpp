#include <iostream>
#include "../Port.h"
#include "../Node.h"
#include "../NodeTree.h"

using namespace iw;

class IntNode : public Node
{
public:
    IntNode()
    {
        m_name = "Integer";

        Port* outputPort = new Port("output", Any(0), PortType::OUTPUT_PORT);
        addPort(outputPort);
    }
};

class AddNode : public Node
{
public:
    AddNode()
    {
        m_name = "Add";

        m_num1Port = new Port("num1", Any(0), PortType::INPUT_PORT);
        addPort(m_num1Port);
        
        m_num2Port = new Port("num2", Any(0), PortType::INPUT_PORT);
        addPort(m_num2Port);

        m_outputPort = new Port("output", Any(0), PortType::OUTPUT_PORT);
        addPort(m_outputPort);
    }

    void exec() override
    {
        m_outputPort->setData(Any(m_num1Port->get<int>() + m_num2Port->get<int>()));
    }

private:
    Port* m_num1Port;
    Port* m_num2Port;
    Port* m_outputPort;
};

class OutputNode : public Node
{
public:
    OutputNode()
    {
        m_name = "Output";

        Port* outputPort = new Port("output", Any(0), PortType::INPUT_PORT);
        addPort(outputPort);
    }

    void exec() override
    {
        std::cout << "[OUTPUT NUMBER]" << port("output")->get<int>() << std::endl;
    }
};

int main()
{
    AddNode* add1Node = new AddNode();
    add1Node->port("num1")->setData(Any(1));
    add1Node->port("num2")->setData(Any(1));

    IntNode* int1Node = new IntNode();
    int1Node->port("output")->setData(Any(3));

    AddNode* add2Node = new AddNode();
    add1Node->port("output")->linkPort(add2Node->port("num1"));
    int1Node->port("output")->linkPort(add2Node->port("num2"));
    
    OutputNode* output1Node = new OutputNode();
    add2Node->port("output")->linkPort(output1Node->port("output"));
    
    IntNode* int2Node = new IntNode();
    int2Node->port("output")->setData(Any(4));

    OutputNode* output2Node = new OutputNode();
    int2Node->port("output")->linkPort(output2Node->port("output"));

    NodeTree* nodeTree = new NodeTree();
    nodeTree->addNode(add1Node);
    nodeTree->addNode(int1Node);
    nodeTree->addNode(add2Node);
    nodeTree->addNode(output1Node);
    nodeTree->addOutputNode(output1Node);

    nodeTree->addNode(int2Node);
    nodeTree->addNode(output2Node);
    nodeTree->addOutputNode(output2Node);

    nodeTree->compile();
    nodeTree->exec();

    delete nodeTree;

    return 0;
}
