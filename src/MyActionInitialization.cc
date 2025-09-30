//
/// \file MyB1/src/MyActionInitialization.cc
/// \brief Implementation of the MyB1::MyActionInitialization class

#include "MyActionInitialization.hh"

#include "MyEventAction.hh"
#include "MyPrimaryGeneratorAction.hh"
#include "MyRunAction.hh"
#include "MySteppingAction.hh"

namespace MyExampleB1
{

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void MyActionInitialization::BuildForMaster() const
{
  auto RunAction = new MyRunAction;
  SetUserAction(RunAction);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void MyActionInitialization::Build() const
{
  SetUserAction(new MyPrimaryGeneratorAction);

  auto RunAction = new MyRunAction;
  SetUserAction(RunAction);

  auto EventAction = new MyEventAction(RunAction);
  SetUserAction(EventAction);

  SetUserAction(new MySteppingAction(EventAction));
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

}  // namespace MyExampleB1
