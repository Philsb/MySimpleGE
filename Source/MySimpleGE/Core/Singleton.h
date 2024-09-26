#pragma once 
#include<mutex>
#include<memory>

namespace MSGE
{

/*
To inherit from this class
declare :

class MyDerivedClass: public Singleton<MyDerivedClass> 
{
private:
    friend class Singleton<MyDerivedClass>;
    MyDerivedClass();
    ~MyDerivedClass();
    MyDerivedClass(const MyDerivedClass&) = delete;
    MyDerivedClass& operator= (cosnt MyDerivedClass&) = delete; 
};

*/
template<typename T>
class Singleton
{
public:
    static T* getInstance()
    {
        std::lock_guard<std::mutex> lock(_mutex);
        if (!_instance)
        {
           _instance = std::make_unique<T>();
        }
        return _instance.get();
    }

protected:
    //Only subclasses can instanciate this
    //With this setup constructor and destructor don't even get called without inheritance
    Singleton(){}
    virtual ~Singleton(){}
    Singleton(const Singleton& other) = delete;
    Singleton& operator= (const Singleton& other) = delete;

private:
    static std::unique_ptr<T> _instance;
    static std::mutex _mutex;
};

template<typename T> std::unique_ptr<T> Singleton<T>::_instance;
template<typename T> std::mutex Singleton<T>::_mutex;

}