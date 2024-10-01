#pragma once
#include <iostream>
#include <map>
#include <memory>

namespace MSGE 
{
//TODO: MAKE THREAD SAFE?
template<typename T, typename KeyType>
class ReferenceManager
{
public:
    ReferenceManager()
    :_cachedReferences(std::make_shared<std::map<KeyType, std::weak_ptr<T>>>())
    {
        std::cout<<"Ref manager created" << std::endl;
    }

    virtual ~ReferenceManager()
    {
        std::cout<<"Ref manager destroyed" << std::endl;
    }

    inline bool hasRef(const KeyType& id)
    {
       return _cachedReferences->end() != _cachedReferences->find(id);
    }
    
    std::shared_ptr<T> getRef(const KeyType& id)
    {
        auto refIterator = _cachedReferences->find(id);
        if (refIterator == _cachedReferences->end())
        {
            return std::shared_ptr<T>();
        }
        return refIterator->second.lock();
    }

    void eraseRef(const KeyType& id)
    {
        _cachedReferences->erase(id);
    }

    std::shared_ptr<T> addRefUnique(const KeyType& id, T* value)
    {
        //This adds a little bit of overhead but makes sure that we don't try to have a new object with the same id.
        //If there is already a reference to that value then use getRef
        if (hasRef(id))
            return std::shared_ptr<T>();

        // I need to create a variable to pass the shared map into the lambda...
        auto mapRef = _cachedReferences;
        auto deleter = [mapRef, id](T* ref) 
        { 
            std::cout << "Deleting shared ptr: " << id << std::endl;
            if (mapRef)
            {
                mapRef->erase(id);        
            }
            delete ref;
        };

        auto newRef = std::shared_ptr<T> (value, deleter);
        auto inserted = _cachedReferences->insert( {id, std::weak_ptr<T>(newRef)} );
        
        return newRef;
    }

    //This is for debugging purposes
    const std::map<KeyType, std::weak_ptr<T>>& getCacheMap() const {return *_cachedReferences;}

protected:
    std::shared_ptr<std::map<KeyType, std::weak_ptr<T>>> _cachedReferences;
};
}