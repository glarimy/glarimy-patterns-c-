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
        RegisteredSubscriber subscribers[10]; 
        int subscriber_count;
        static SimpleSecurity* instance;
        SimpleSecurity()
        {
            subscriber_count = 0;
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
            int index;
            bool found = false;
            for(index=0; index<subscriber_count; index++)
            {
                cout << "checking for duplicate at " << index << "endl";
                if(subscribers[index].credentials.userid == subscriber.credentials.userid)
                {
                    found = true;
                    cout << "duplicate found" << endl;
                    break;
                }
            }
            if(found == true)
            {
                throw "Duplicate Login ID";
            }else{
                subscribers[subscriber_count++] = RegisteredSubscriber(subscriber);
                cout << "registered subscriber" << endl; 
            }
        }
        Profile login(Credentials credentials)
        {
            int index;
            for(index=0; index<subscriber_count; index++)
            {
                cout << "checking for match at " << index << "endl";
                if(subscribers[index].credentials.userid == credentials.userid 
                    && subscribers[index].credentials.password == credentials.password)
                {
                    cout << "foud the match" << endl;
                    subscribers[index].loggedin = true;
                    cout << "logged in";
                    return subscribers[index].profile;
                }
            }
            cout << "no match found " << endl;
            throw "Login failed";
        }
        void logout(string userid)
        {
            int index;
            for(index=0; index<subscriber_count; index++)
            {
                cout << "checking for match " << index << "endl";
                if(subscribers[index].credentials.userid == userid && subscribers[index].loggedin == true)
                {
                    cout << "foud the match" << endl;
                    subscribers[index].loggedin = false;
                    cout << "logged out" << endl;
                    return;
                }
            }
            cout << "currently not logged in" << endl;
            throw "Logout failed";
        }
};

SimpleSecurity *SimpleSecurity::instance = 0;

class SecurityFactory 
{
    public:
        static Security* getSecurity()
        {
            //return new SimpleSecurity();
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