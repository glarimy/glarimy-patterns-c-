#include<iostream> 
using namespace std; 

class Profile 
{
    public:
        Profile(){

        }
        Profile(string name, string email)
        {
            this->name = name;
            this->email = email;
        }
        string name;
        string email;
};

class Credentials 
{
    public:
        Credentials()
        {

        }
        Credentials(string userid, string password)
        {
            this->userid = userid;
            this->password = password;
        }
        string userid;
        string password;
};

class Subscriber
{
    public:
        Subscriber()
        {

        }
        Subscriber(Credentials credentials, Profile profile)
        {
            this->credentials = credentials;
            this->profile = profile;
        }
        Profile profile;
        Credentials credentials;
};

class RegisteredSubscriber:public Subscriber
{
    public:
        RegisteredSubscriber()
        {

        }
        RegisteredSubscriber(Subscriber subscriber)
        {
            this->credentials = subscriber.credentials;
            this->profile = subscriber.profile;
            loggedin = false;
        }
        bool loggedin;
};

class Adapter 
{
    public:
        virtual void insert(RegisteredSubscriber subscriber) = 0;
        virtual RegisteredSubscriber find(string key) = 0;
        virtual void update(RegisteredSubscriber subcriber) = 0;
};

class ArrayAdapter : public Adapter
{
    private: 
        RegisteredSubscriber entities[10]; 
        int count;
    public:
        ArrayAdapter()
        {
            count = 0;
        }
        void insert(RegisteredSubscriber subscriber)
        {
            cout << "adding " << subscriber.credentials.userid << endl;
            if (count != 10)
            {
                entities[count++] = subscriber;
                cout << "inserted entity" << endl;
            }
            else
                throw "overflow";
        }
        RegisteredSubscriber find(string key)
        {
            cout << "finding " << key << endl;
            int index;
            for(index=0; index<count; index++)
            {
                cout << "finding entity at " << index << endl;
                if(entities[index].credentials.userid == key)
                {
                    cout << "found entity" << endl;
                    return entities[index];
                }
            }
            throw "entity not found";
        }
        void update(RegisteredSubscriber subscriber)
        {
            cout << "updating " << subscriber.credentials.userid << endl;
            int index;
            for(index=0; index<count; index++)
            {
                cout << "finding entity at " << index << endl;
                if(entities[index].credentials.userid == subscriber.credentials.userid)
                {
                    cout << "found entity" << endl;
                    entities[index] = subscriber;
                    return;
                }
            }
            throw "entity not found";
        }
};

class Security
{
    public: 
        virtual Profile login(Credentials credentials) = 0;
        virtual void add(Subscriber subscriber) = 0;
        virtual void logout(string userid) = 0;
};

class SimpleSecurity : public Security
{
    private:
        Adapter* adapter;
        static SimpleSecurity* instance;
        SimpleSecurity()
        {
            adapter = new ArrayAdapter();
        }
    public: 
        static SimpleSecurity* getInstance()
        {
            if(SimpleSecurity::instance == NULL)
            {
                SimpleSecurity::instance = new SimpleSecurity();
            }
            return SimpleSecurity::instance;
        }
        void add(Subscriber subscriber)
        {
            try {
                adapter->find(subscriber.credentials.userid);
                throw "Duplicate Login ID";
            }catch(char const* s){
                cout << "No duplicate" << endl;
                adapter->insert(RegisteredSubscriber(subscriber));
            }
        }
        Profile login(Credentials credentials)
        {
            try {
                RegisteredSubscriber subscriber = (RegisteredSubscriber) adapter->find(credentials.userid);
                subscriber.loggedin = true;
                adapter->update(subscriber);
                return subscriber.profile;
            }catch(char const* s){
                cout << "no match found " << endl;
                throw "Login failed";
            }
        }

        void logout(string userid)
        {
            try {
                RegisteredSubscriber subscriber = (RegisteredSubscriber) adapter->find(userid);
                if(subscriber.loggedin == true)
                {
                    subscriber.loggedin = false;
                    adapter->update(subscriber);
                }else{
                    throw "Logout Failed";
                }
            }catch(char const* s){
                throw "Logout failed";
            }
        }
};

SimpleSecurity *SimpleSecurity::instance = 0;

class SecurityFactory 
{
    public:
        static Security* getSecurity()
        {
            return SimpleSecurity::getInstance();
        }
};



int main()
{
    Security* security = SecurityFactory::getSecurity();
    Profile profile = Profile("Krishna Mohan Koyya", "krishna@glarimy.com");
    Credentials credentials = Credentials("koyya", "123456");
    Subscriber subsriber = Subscriber(credentials, profile);
    try {
        security->add(subsriber);
        cout << "added subscriber" << endl;
        Profile profile = security->login(credentials);
        cout << "Hi " << profile.name << "! You are now logged in" << endl;
        security->logout("koyya");
        cout << "Logged out successfully" << endl;
    }catch(char const* s){
        cout << s << endl;;
    }


}