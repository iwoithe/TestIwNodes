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
        m_deltaTimePort->setData(Any(m_deltaTime->getUnlimited()));
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
        std::cout << "[DEBUG] " << m_valuePort->get<double>() << std::endl;
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

    bool isRunning = true;
    double totalFrameTime = 0.0;
    bool limitFps = false;
    if (limitFps)
        deltaTime->setFps(30);

    while (isRunning)
    {
        deltaTime->tick();
        if (limitFps)
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
