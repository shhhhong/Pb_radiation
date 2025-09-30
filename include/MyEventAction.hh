//
/// \file MyExampleB1/include/MyEventAction.hh
/// \brief Definition of the MyExampleB1::MyEventAction class

#ifndef MyEventAction_h
#define MyEventAction_h 1

#include "G4UserEventAction.hh"
#include "globals.hh"

class G4Event;

namespace MyExampleB1
{

class MyRunAction;

/// Event action class

class MyEventAction : public G4UserEventAction
{
  public:
    MyEventAction(MyRunAction* MyRunAction);
    ~MyEventAction() override = default;

    void BeginOfEventAction(const G4Event* event) override;
    void EndOfEventAction(const G4Event* event) override;

    void AddEdep(G4double edep) { fEdep += edep; }

  private:
    MyRunAction* fMyRunAction = nullptr;
    G4double fEdep = 0.;
};

}  // namespace MyExampleB1

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
