//
/// \file MyExampleB1/src/MySteppingAction.cc
/// \brief Implementation of the MyExampleB1::MySteppingAction class

#include "MySteppingAction.hh"


#include "MyDetectorConstruction.hh"
#include "MyEventAction.hh"

#include "G4AnalysisManager.hh"
#include "G4Event.hh"
#include "G4LogicalVolume.hh"
#include "G4RunManager.hh"
#include "G4Step.hh"
#include "G4SystemOfUnits.hh"
#include "G4Material.hh"
#include "G4MaterialCutsCouple.hh"
#include "G4Region.hh"
#include "G4Track.hh"
#include "G4VProcess.hh"

namespace MyExampleB1
{

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

MySteppingAction::MySteppingAction(MyEventAction* MyEventAction) : fMyEventAction(MyEventAction) {}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void MySteppingAction::UserSteppingAction(const G4Step* step)
{
  
  if (!fScoringVolume) {
    const auto detConstruction = static_cast<const MyDetectorConstruction*>(
      G4RunManager::GetRunManager()->GetUserDetectorConstruction());
    fScoringVolume = detConstruction->GetScoringVolume();
  }
  
// Get pre/post step points, track information, and basic quantities
  const auto pre  = step->GetPreStepPoint();
  const auto post = step->GetPostStepPoint();
  const auto trk  = step->GetTrack();

  const G4int evID     = G4RunManager::GetRunManager()->GetCurrentEvent()->GetEventID();
  const G4int trackID  = trk->GetTrackID();
  const G4int parentID = trk->GetParentID();
  const G4String pName = trk->GetDefinition()->GetParticleName();
  const G4int stepNo   = trk->GetCurrentStepNumber();

  // Positions (full step: both pre- and post-step points)
  const auto prePos  = pre->GetPosition();   // mm (G4 units)
  const auto postPos = post->GetPosition();  // mm

  // Energy, step length, and process information
  const G4double dEStep  = step->GetTotalEnergyDeposit(); // MeV
  const G4double stepLen = step->GetStepLength();         // mm
  const G4double kinE    = post->GetKineticEnergy();      // MeV (post 点的动能更稳定)
  const auto proc        = post->GetProcessDefinedStep();
  const G4String procName = proc ? proc->GetProcessName() : "NA";

  // 5) Volume names (check both pre- and post-step to avoid crash if particle leaves the world volume)
  const auto preTH = pre->GetTouchableHandle();
  const auto prePV = preTH ? preTH->GetVolume() : nullptr;
  const G4String volPre  = prePV ? prePV->GetName() : "OutOfWorld";

  const auto postTH = post->GetTouchableHandle();
  const auto postPV = postTH ? postTH->GetVolume() : nullptr;
  const G4String volPost = postPV ? postPV->GetName() : "OutOfWorld";

  // Write out the full step information (record all steps, including world/absorber boundaries)
  auto man = G4AnalysisManager::Instance();
  
    // Fill ntuple
    man->FillNtupleIColumn(0,  evID);
    man->FillNtupleIColumn(1,  trackID);
    man->FillNtupleIColumn(2,  parentID);
    man->FillNtupleSColumn(3,  pName);
    man->FillNtupleIColumn(4,  stepNo);

    man->FillNtupleDColumn(5,  prePos.x()/mm);
    man->FillNtupleDColumn(6,  prePos.y()/mm);
    man->FillNtupleDColumn(7,  prePos.z()/mm);

    man->FillNtupleDColumn(8,  postPos.x()/mm);
    man->FillNtupleDColumn(9,  postPos.y()/mm);
    man->FillNtupleDColumn(10, postPos.z()/mm);

    man->FillNtupleDColumn(11, dEStep/MeV);
    man->FillNtupleDColumn(12, stepLen/mm);
    man->FillNtupleDColumn(13, kinE/MeV);

    man->FillNtupleSColumn(14, volPre);
    man->FillNtupleSColumn(15, volPost);
    man->FillNtupleSColumn(16, procName);

    man->AddNtupleRow();
  

  // get volume of the current step
  G4LogicalVolume* volume =
  step->GetPreStepPoint()->GetTouchableHandle()->GetVolume()->GetLogicalVolume();

  // check if we are in scoring volume
  if (volume != fScoringVolume) return;

  // collect energy deposited in this step
  G4double edepStep = step->GetTotalEnergyDeposit();
  if (fMyEventAction) {                      
  fMyEventAction->AddEdep(edepStep);}

}

}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

// namespace MyExampleB1
