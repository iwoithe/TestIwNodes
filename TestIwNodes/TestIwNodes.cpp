#include <iostream>

#include "iwdeltatime.h"

#include "Node.h"
#include "NodeTree.h"
#include "Port.h"

using namespace iw;

class UpdateInputNode : public Node
{
public:
    UpdateInputNode() : UpdateInputNode(nullptr) {}
    UpdateInputNode(DeltaTime* deltaTime)
    {
        m_name = "UpdateInput";
        m_deltaTime = deltaTime;

        Port* execOutPort = new Port("execOut", Any(0), PortType::OUTPUT_PORT);
        m_deltaTimePort = new Port("deltaTime", Any(0), PortType::OUTPUT_PORT);
        addPort(execOutPort);
        addPort(m_deltaTimePort);
    }

    void exec() override
    {
        m_deltaTimePort->setData(Any(m_deltaTime->get()));
    }

protected:
    Port* m_deltaTimePort;
    DeltaTime* m_deltaTime;
};

class UpdateOutputNode : public Node
{
public:
    UpdateOutputNode()
    {
        m_name = "UpdateOutput";

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
        if (m_valuePort->dataType() == typeid(double)) {
            std::cout << "[DEBUG] " << m_valuePort->get<double>() << std::endl;
        } else if (m_valuePort->dataType() == typeid(int)) {
            std::cout << "[DEBUG] " << m_valuePort->get<int>() << std::endl;
        } else if (m_valuePort->dataType() == typeid(std::string)) {
            std::cout << "[DEBUG] " << m_valuePort->get<std::string>() << std::endl;
        }
    }

protected:
    Port* m_valuePort;
};


int main()
{
    DeltaTime* deltaTime = new DeltaTime();

    NodeTree* updateNodeTree = new NodeTree();

    UpdateInputNode* updateInput = new UpdateInputNode(deltaTime);
    PrintNode* print = new PrintNode();
    UpdateOutputNode* updateOutput = new UpdateOutputNode();

    updateInput->port("execOut")->linkPort(print->port("execIn"));
    print->port("execOut")->linkPort(updateOutput->port("execIn"));

    updateInput->port("deltaTime")->linkPort(print->port("value"));

    updateNodeTree->addOutputNode(updateOutput);
    updateNodeTree->compile();

    bool isRunning = true;
    double totalFrameTime = 0.0;
    // deltaTime->setFps(30);

    while (isRunning)
    {
        deltaTime->tick();
        if (deltaTime->fps() > 0)
        {
            totalFrameTime += deltaTime->getUnlimited();
            if (totalFrameTime < 1 / deltaTime->fps()) continue;
            totalFrameTime = 0;
        }
        
        updateNodeTree->exec();
    }


    delete updateNodeTree;
    delete deltaTime;

    return 0;
}
