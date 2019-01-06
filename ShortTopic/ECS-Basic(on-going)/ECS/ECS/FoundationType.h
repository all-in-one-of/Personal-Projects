#pragma once


#include <iostream>
#include <vector>
#include <queue>
#include <string>
#include <map>


//! Forward Declare
struct Entity;



//! To create a id for each type
class GetTypeID
{
  static size_t idCounter;

public:

  //! static function to evaluate a value for each type
  template<typename T>
  static size_t Value()
  {
    static size_t cur_ID = idCounter++;

    return cur_ID;
  }
};
size_t GetTypeID::idCounter = 0;


//! The data handler to be use inside the Entity to access components
struct CompData
{
  size_t typeID;
  size_t factoryID;
  size_t compIndex;

  CompData(size_t a, size_t b, size_t c)
    :
    typeID{a},
    factoryID{b},
    compIndex{c}
  {

  }

  CompData(const CompData & elem)
  {
    typeID = elem.typeID;
    factoryID = elem.factoryID;
    compIndex = elem.compIndex;
  }

};