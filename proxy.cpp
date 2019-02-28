#include<iostream> 
using namespace std; 

// an interface for business logic
class Component {
    public:
        virtual void doSomething() = 0;
};

// core businesss logic implmentation without any non-functional aspects
class ConcreteComponent:public Component {
    public:
        void doSomething()
        {
            cout << "concrete component doing something" << endl;
        }
};

// implementing a non-functional requirement without business logic
class Proxy : public Component{
    private:
        Component* target;
    public:
        Proxy(Component* target)
        {
            this->target = target;
        }
        void doSomething()
        {
            cout << "proxy is pre-processing" << endl;
            target->doSomething();
            cout << "proxy is post-processing" << endl;
        }
};

// choosing component with or without non-functional aspects
class Factory{
    public:
    static Component* getComponent(bool intercept)
    {
        Component* component = new ConcreteComponent(); 
        if(intercept == true){
            Component* proxy = new Proxy(component);
            return proxy;
        }else{
            return component;
        }
    }
};

int main()
{
    // with proxy
    cout << "With proxy:" << endl;
    Component* component = Factory::getComponent(true);
    component->doSomething();

    // without proxy
    cout << "Without proxy:" << endl;
    component = Factory::getComponent(false);
    component->doSomething();

    // in both the cases, client has no idea of presence/absence of proxy
}