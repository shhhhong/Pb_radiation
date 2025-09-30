//
/// \file MyExampleB1/include/MySteppingAction.hh
/// \brief Definition of the MyExampleB1::MySteppingAction class

#ifndef MySteppingAction_h
#define MySteppingAction_h 1

#include "G4UserSteppingAction.hh"

class G4LogicalVolume;
class G4Step;

namespace MyExampleB1
{

class MyEventAction;

/// Stepping action class

class MySteppingAction : public G4UserSteppingAction
{
  public:
    MySteppingAction(MyEventAction* MyEventAction);
    ~MySteppingAction() override = default;

    // method from the base class
    void UserSteppingAction(const G4Step*) override;

  private:
    MyEventAction* fMyEventAction = nullptr;
    G4LogicalVolume* fScoringVolume = nullptr;
};

}  // namespace MyExampleB1

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
