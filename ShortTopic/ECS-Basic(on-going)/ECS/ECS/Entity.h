#pragma once

#include "ComponentFactory.h"

struct Entity
{
  std::string name;

  std::vector<CompData> Components;

private:
  // helper

  template<typename currentType>
  size_t LocateExistComponent()
  {
    size_t uniqueTypeId = GetTypeID::Value<currentType>();
    size_t location = 0;

    for (auto & elem : Components)
    {
      if (uniqueTypeId == elem.typeID)
      {
        return location;
      }

      location++;
    }

    return -1;

  }

public:

  Entity(std::string s = "")
    : name{ s }
  {

  }

  template<typename currentType>
  currentType* GetComponent()
  {
    size_t found = LocateExistComponent<currentType>();
    if (found != -1)
    {
      auto& _FM = FactoriesManager::Instance();

      void * getPointer = _FM._factoriesHolder[Components[found].factoryID]->GetIndexComponent(Components[found].compIndex);

      return static_cast<currentType*>(getPointer);
    }


    return nullptr;
  }

  //! to add struct and class into the list in the baseType factory
  template<typename currentType, typename baseType>
  currentType* AddComponent()
  {
    if (LocateExistComponent<currentType>() == -1)
    {
      auto& _componentFactory = TypeFactory<baseType>::Instance();
      Components.push_back(_componentFactory.AddComponent<currentType>(static_cast<Entity *>(this)));
    }

    return GetComponent<currentType>();
  }

  template<typename currentType>
  currentType* AddComponent()
  {
    if (LocateExistComponent<currentType>() == -1)
    {
      auto& _componentFactory = TypeFactory<currentType>::Instance();
      Components.push_back(_componentFactory.AddComponent<currentType>(static_cast<Entity *>(this)));
    }

    return GetComponent<currentType>();
  }



  template<typename currentType>
  void RemoveComponent()
  {
    size_t found = LocateExistComponent<currentType>();
    if (found != -1)
    {
      auto& _FM = FactoriesManager::Instance();

      _FM._factoriesHolder[Components[found].factoryID]->RemoveComponent(Components[found].compIndex);

      auto itr = Components.begin();

      while (found--)
      {
        itr++;
      }

      Components.erase(itr);


    }
  }


};

//! temporary basic entity list
struct BasicEntityList
{

  std::vector<std::unique_ptr<Entity>> List;

  static BasicEntityList& Instance()
  {
    static BasicEntityList listGet;
    return listGet;
  }


  static Entity* AddEntity(std::string name = "")
  {
    BasicEntityList& listGet = Instance();
    return listGet.AddEnt(name);
  }
  


private:

  Entity* AddEnt(std::string& name)
  {
    size_t loc = List.size();

    try
    {

      List.emplace_back(new Entity{ name });

      return List[loc].get();
    }
    catch (...)
    {
      return nullptr;
    }
  }

  BasicEntityList(){}
  ~BasicEntityList() {}
  BasicEntityList(const BasicEntityList&) {}


};