#pragma once

#include "Entity.h"



struct Component
{
  Entity* owner;


  virtual void Initialise() {};
  virtual void Awake() {};
  virtual void EarlyUpdate() {};
  virtual void Update() {};
  virtual void LateUpdate() {};
  virtual void Destroy() {};


};


struct Collider : Component
{

  void Update() { std::cout << " i collide " << owner->name << std::endl; };
};


struct Renderer : Component
{
  int i;
  int j;
  int k;
  void Update() { std::cout << " i render " << owner->name << std::endl; };
};

struct Script : Component
{
  virtual void Update() {};
};


struct ScriptA : Script
{
  void Update() { std::cout << " A script by " << owner->name << std::endl; };
};

struct ScriptB : Script
{
  void Update() { std::cout << " B script by " << owner->name << std::endl; };
};

struct NotInheritFromComponent
{
  Entity* owner;

  void Update() { std::cout << " I have no super class " << owner->name << std::endl; }
};


/* Wanted to include policy design in it, but i still require a super class as container
  if i would like to store all policy into a single container


template <typename t>
struct PolicyDesigner : public t
{
  Entity* owner;

  void Update() { Run(); }
};

*/


static void BasicTest(Entity * firstBorn)
{
  std::cout << "TEST : Add Get Remove" << std::endl;

  std::cout << "Adding 3 components" << std::endl;
  firstBorn->AddComponent<Collider>();
  Renderer* renComp = firstBorn->AddComponent<Renderer>();
  NotInheritFromComponent * noSuperClassComp = firstBorn->AddComponent<NotInheritFromComponent>();

  std::cout << "Getting 1 component" << std::endl;
  Collider * colComp = firstBorn->GetComponent<Collider>();


  std::cout << "Calling components function" << std::endl;
  colComp->Update();
  renComp->Update();
  noSuperClassComp->Update();

  std::cout << std::endl;

  std::cout << "Total Number of Component Entity has : " << firstBorn->Components.size() << std::endl;
  std::cout << "Removing Not inherit from Component..." << std::endl;

  firstBorn->RemoveComponent<NotInheritFromComponent>();

  std::cout << "Total Number of Component Entity has : " << firstBorn->Components.size() << std::endl << std::endl;

  std::cout << "-------------------------------------------------------------------------------------------------" << std::endl;
}


static void GetListTest(Entity* firstBorn, Entity* secondBorn)
{
  std::cout << "TEST : Getting list(Render) " << std::endl;

  auto & Instance = TypeFactory<Renderer>::Instance();
  auto & RendererListReference = Instance.GetComponentList();

  std::cout << "Total amount of renderer : " << RendererListReference.size() << std::endl;
  std::cout << "Adding a new Renderer to entity 2" << std::endl;

  secondBorn->AddComponent<Renderer>();
  std::cout << "Total amount of renderer : " << RendererListReference.size() << std::endl;
  std::cout << std::endl;

  std::cout << "Attempt to add another renderer to entity 1" << std::endl;
  firstBorn->AddComponent<Renderer>();
  std::cout << "Total amount of renderer : " << RendererListReference.size() << std::endl << std::endl;

  std::cout << "-------------------------------------------------------------------------------------------------" << std::endl;
}

static void SuperClassTest(Entity* thirdBorn)
{
  std::cout << "TEST : Allocating two component into a super class factory(Script)" << std::endl;


  auto & Instance = TypeFactory<Script>::Instance();
  auto & ScriptListReference = Instance.GetComponentList();
  std::cout << "Current Scripts stored in factory : " << ScriptListReference.size() << std::endl;

  std::cout << "Adding Scripts inside Entity..." << std::endl;

  thirdBorn->AddComponent<ScriptA, Script>()->Update();
  thirdBorn->AddComponent<ScriptB, Script>()->Update();


  std::cout << "Current Scripts stored in factory : " << ScriptListReference.size() << std::endl << std::endl;

  std::cout << "-------------------------------------------------------------------------------------------------" << std::endl;
}





struct NotInheritComponent
{
  Entity* owner;

  void Update() { std::cout << " no super class " << owner->name << std::endl; }
};

struct history
{
  Entity* owner;

  virtual void Update() { std::cout << " I am the history " << owner->name << std::endl; }
};

struct nickname : public history
{
  std::string nick;

  nickname() :nick{ "tommy" } {};
  void SetNickname(std::string newNic) {
    nick = newNic;
  }
  void Update() { std::cout << " my nickname is " << nick << " " << owner->name << std::endl; }
};

struct school : public history
{

  void Update() { std::cout << " school is DigiPen " << owner->name << std::endl; }
};

void testBasic()
{
  Entity* StudentA = BasicEntityList::AddEntity("StudentA");
  Entity* StudentB = BasicEntityList::AddEntity("StudentB");
  Entity* StudentC = BasicEntityList::AddEntity("StudentC");

  std::cout << "Start Test - Add " << std::endl;
  StudentA->AddComponent<NotInheritComponent>();
  StudentA->AddComponent<nickname>();
  StudentA->AddComponent<school>();

  std::cout << std::endl;
  std::cout << "Start Test - Get " << std::endl;
  StudentA->GetComponent<nickname>()->SetNickname("AAA");
  StudentA->GetComponent<nickname>()->Update();
  StudentA->GetComponent<school>()->Update();
  StudentA->GetComponent<NotInheritComponent>()->Update();

  std::cout << std::endl;
  std::cout << "Start Test - Remove & Get " << std::endl;
  StudentA->RemoveComponent<nickname>();
  if (StudentA->GetComponent<nickname>() == nullptr)
    std::cout << " nickname have been removed " << std::endl;
  StudentA->RemoveComponent<school>();
  StudentA->RemoveComponent<NotInheritComponent>();

  std::cout << std::endl;
  std::cout << "Basic Functionality Test Succeeded " << std::endl;

}

void Test()
{
  /*
  std::cout << "Start " << std::endl;
  std::cout << std::endl;
  testBasic();
  std::cout << std::endl;
  std::cout << "End " << std::endl;
  */

  
  Entity* firstBorn = BasicEntityList::AddEntity("IamNumberOne");
  Entity* secondBorn = BasicEntityList::AddEntity("IamNumberTwo");
  Entity* thirdBorn = BasicEntityList::AddEntity("IamNumberThree");
  Entity* fourthBorn = BasicEntityList::AddEntity("IamNumberFour");

  BasicTest(firstBorn);

  GetListTest(firstBorn, secondBorn);


  SuperClassTest(thirdBorn);
  

}