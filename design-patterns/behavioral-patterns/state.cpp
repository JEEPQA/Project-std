#include <iostream>
using namespace std;

class Context;

class State
{
public:
    virtual void Handle(Context* pContext) = 0;
};

class DefaultState : public State
{
public:
    virtual void Handle(Context* pContext)
    {
        cout << "I am DefaultState." << endl;
    }
};


class ConcreteStateA : public State
{
public:
    virtual void Handle(Context* pContext)
    {
        cout << "I am concretestateA." << endl;
    }
};

class ConcreteStateB : public State
{
public:
    virtual void Handle(Context* pContext)
    {
        cout << "I am concretestateB." << endl;
    }
};

class Context
{
public:
    Context() : m_pState(new DefaultState) { }
    Context(State* pState) : m_pState(pState) {}
    ~Context() { delete m_pState; }

    void Request()
    {
        if (m_pState)
        {
            m_pState->Handle(this);
        }
    }

    void ChangeState(State* pState)
    {
        m_pState = pState;
    }

private:
    State* m_pState;
};

int main()
{
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

    State* pStateA = new ConcreteStateA();
    State* pStateB = new ConcreteStateB();

    Context* pContext = new Context();
    pContext->ChangeState(pStateA);
    pContext->Request();

    pContext->ChangeState(pStateB);
    pContext->Request();

    delete pContext;
    //delete pStateB;
    delete pStateA;

    int a = 9;
    a = a & 10;
    _CrtDumpMemoryLeaks();
}