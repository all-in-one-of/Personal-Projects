#pragma once

#include "FoundationType.h"



/*
  The idea is to use factory&manager to contain all possible components of the entity


  //! Singleton factory -> Scott Meyers' Singleton factory


*/

///! To be use as super class for all factory created
struct FactoryHolder
{
  size_t _factoryID;
  virtual void Reset() = 0;

  virtual void* GetIndexComponent(size_t loc) { return nullptr; };
  virtual void RemoveComponent(size_t index) {};
};


//! to hold all the singleton factory as well as to bridge 
//! the gap between entity and their components
class FactoriesManager
{


public:

  std::vector<FactoryHolder *> _factoriesHolder;

  void Reset()
  {
    for (auto & elem : _factoriesHolder)
    {
      elem->Reset();
    }
  }

  static FactoriesManager& Instance()
  {
    static FactoriesManager s;
    return s;
  }


private:
  FactoriesManager() {}
  ~FactoriesManager() {}
  FactoriesManager(const FactoriesManager&) {}
};


//! Temporary Design for the moment to know if the component is alive or not
template<typename t>
struct ComponentData
{
  bool alive = true;
  std::unique_ptr<t> data;

  ComponentData() {};
  ComponentData(const ComponentData<t> &) {};
  ~ComponentData()
  {
    data.reset();
  }

  template<typename n>
  void SetData(Entity * _owner)
  {
    data.reset(new n);
    data->owner = _owner;
  }

};

//! Singleton factory to a component type
template<typename t>
class TypeFactory : public FactoryHolder
{
public:
  static TypeFactory<t>& Instance()
  {
    static TypeFactory<t> factory;
    return factory;
  }

  //! to set the component as dead
  virtual void RemoveComponent(size_t index)
  {
    if (index < 0 || index >= _FactoryData.size())
      return;

    if (_FactoryData[index].alive)
    {
      _FactoryData[index].alive = false;
      _Dead_Id.emplace(static_cast<int>(index));
    }
  }



  //! add component, allow the other struct/class as long as factory type is a superclass to it
  template<typename n>
  CompData AddComponent(Entity * _owner)
  {
    if (_Dead_Id.empty())
    {
      auto loc = _FactoryData.size();

      _FactoryData.push_back(ComponentData<t>{});
      _FactoryData[loc].SetData<n>(_owner);
      return CompData{ GetTypeID::Value<n>(), _factoryID, static_cast<size_t>(loc) };
    }
    else
    {
      auto loc = _Dead_Id.front();
      auto & tmpFac = _FactoryData[loc];
      tmpFac.SetData<n>(_owner);
      tmpFac.alive = true;
      _Dead_Id.pop();
      return CompData{ GetTypeID::Value<n>(), _factoryID, static_cast<size_t>(loc) };
    }
  }

  //! clear the list
  virtual void Reset()
  {
    _FactoryData.clear();
    while (!_Dead_Id.empty())
    {
      _Dead_Id.pop();
    }
  }

  //! get the void pointer base on the position index 
  virtual void* GetIndexComponent(size_t loc)
  {
    if (loc < 0 || loc >= _FactoryData.size())
    {
      return nullptr;
    }
    return static_cast<void *>(_FactoryData[loc].data.get());

  };
  std::vector<ComponentData<t>> & GetComponentList()
  {
    return _FactoryData;
  }

private:
  //! to store the id of the dead component
  std::queue<int> _Dead_Id;

  //! the vector of component
  std::vector<ComponentData<t>> _FactoryData;

  //! get the whole list of component

  TypeFactory()
  {
    FactoriesManager& _FM = FactoriesManager::Instance();
    _factoryID = _FM._factoriesHolder.size();
    _FM._factoriesHolder.push_back(static_cast<FactoryHolder *>(this));
  }
  ~TypeFactory() {}
  TypeFactory(const TypeFactory<t>&) {}
};