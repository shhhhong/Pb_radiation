//
/// \file MyExampleB1/src/MyRunAction.cc
/// \brief Implementation of the MyExampleB1::MyRunAction class

#include "MyRunAction.hh"

#include "MyDetectorConstruction.hh"
#include "MyPrimaryGeneratorAction.hh"

#include "G4AccumulableManager.hh"
#include "G4LogicalVolume.hh"
#include "G4ParticleDefinition.hh"
#include "G4ParticleGun.hh"
#include "G4Run.hh"
#include "G4RunManager.hh"
#include "G4SystemOfUnits.hh"
#include "G4UnitsTable.hh"
#include "G4AnalysisManager.hh"
#include "G4ProductionCutsTable.hh"

namespace MyExampleB1
{

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

MyRunAction::MyRunAction()
{
  // add new units for dose
  //
  const G4double milligray = 1.e-3 * gray;
  const G4double microgray = 1.e-6 * gray;
  const G4double nanogray = 1.e-9 * gray;
  const G4double picogray = 1.e-12 * gray;

  new G4UnitDefinition("milligray", "milliGy", "Dose", milligray);
  new G4UnitDefinition("microgray", "microGy", "Dose", microgray);
  new G4UnitDefinition("nanogray", "nanoGy", "Dose", nanogray);
  new G4UnitDefinition("picogray", "picoGy", "Dose", picogray);

  auto man = G4AnalysisManager::Instance();
  man->SetVerboseLevel(1);
  man->SetActivation(true);

  // Create directories
  man->CreateNtuple("steps","full step");
  man->CreateNtupleIColumn("eventID");      // 0
  man->CreateNtupleIColumn("trackID");      // 1
  man->CreateNtupleIColumn("parentID");     // 2
  man->CreateNtupleSColumn("particle");     // 3
  man->CreateNtupleIColumn("stepNo");       // 4
  man->CreateNtupleDColumn("preX_mm");      // 5
  man->CreateNtupleDColumn("preY_mm");      // 6
  man->CreateNtupleDColumn("preZ_mm");      // 7
  man->CreateNtupleDColumn("postX_mm");     // 8
  man->CreateNtupleDColumn("postY_mm");     // 9
  man->CreateNtupleDColumn("postZ_mm");     // 10
  man->CreateNtupleDColumn("dEStep_MeV");   // 11
  man->CreateNtupleDColumn("stepLen_mm");   // 12
  man->CreateNtupleDColumn("kinE_MeV");     // 13
  man->CreateNtupleSColumn("volPre");       // 14
  man->CreateNtupleSColumn("volPost");      // 15
  man->CreateNtupleSColumn("process");      // 16
  man->FinishNtuple();

  // Register accumulable to the accumulable manager
  G4AccumulableManager* accumulableManager = G4AccumulableManager::Instance();
  accumulableManager->Register(fEdep);
  accumulableManager->Register(fEdep2);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

// MyRunAction::~MyRunAction() {
//   delete G4AnalysisManager::Instance();
// }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void MyRunAction::BeginOfRunAction(const G4Run*)
{
  // inform the runManager to save random number seed
  G4RunManager::GetRunManager()->SetRandomNumberStore(false);
  
  G4cout << "=== Dump Production Cuts Table -songzhihao ===" << G4endl;
  G4ProductionCutsTable::GetProductionCutsTable()->DumpCouples();

  // reset accumulables to their initial values
  G4AccumulableManager* accumulableManager = G4AccumulableManager::Instance();
  accumulableManager->Reset();

  auto man = G4AnalysisManager::Instance();
  man->SetDefaultFileType("root");
  man->SetFileName("steps");
  man->OpenFile();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void MyRunAction::EndOfRunAction(const G4Run* run)
{
  G4int nofEvents = run->GetNumberOfEvent();
  if (nofEvents == 0) return;

  // Merge accumulables
  G4AccumulableManager* accumulableManager = G4AccumulableManager::Instance();
  accumulableManager->Merge();

  // Compute dose = total energy deposit in a run and its variance
  //
  G4double edep = fEdep.GetValue();
  G4double edep2 = fEdep2.GetValue();

  G4double rms = edep2 - edep * edep / nofEvents;
  if (rms > 0.)
    rms = std::sqrt(rms);
  else
    rms = 0.;

  const auto detConstruction = static_cast<const MyDetectorConstruction*>(
    G4RunManager::GetRunManager()->GetUserDetectorConstruction());
  G4double mass = detConstruction->GetScoringVolume()->GetMass();
  G4double dose = edep / mass;
  G4double rmsDose = rms / mass;

  // Run conditions
  //  note: There is no primary generator action object for "master"
  //        run manager for multi-threaded mode.
  const auto generatorAction = static_cast<const MyPrimaryGeneratorAction*>(
    G4RunManager::GetRunManager()->GetUserPrimaryGeneratorAction());
  G4String runCondition;
  if (generatorAction) {
    // const G4ParticleGun* particleGun = generatorAction->GetParticleGun();
    // runCondition += particleGun->GetParticleDefinition()->GetParticleName();
    // runCondition += " of ";
    // G4double particleEnergy = particleGun->GetParticleEnergy();
    // runCondition += G4BestUnit(particleEnergy, "Energy");
    const G4GeneralParticleSource* gps = generatorAction->GetGPS();
    runCondition += gps->GetParticleDefinition()->GetParticleName();
    runCondition += " of ";
    runCondition += G4BestUnit(gps->GetParticleEnergy(), "Energy");
  }
  
  auto man = G4AnalysisManager::Instance();
  man->Write();
  man->CloseFile();

  // Print
  //
  if (IsMaster()) {
    G4cout << G4endl << "--------------------End of Global Run-----------------------";
  }
  else {
    G4cout << G4endl << "--------------------End of Local Run------------------------";
  }

  G4cout << G4endl << " The run consists of " << nofEvents << " " << runCondition << G4endl
         << " Cumulated dose per run, in scoring volume : " << G4BestUnit(dose, "Dose")
         << " rms = " << G4BestUnit(rmsDose, "Dose") << G4endl
         << "------------------------------------------------------------" << G4endl << G4endl;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void MyRunAction::AddEdep(G4double edep)
{
  fEdep += edep;
  fEdep2 += edep * edep;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

}  // namespace MyExampleB1

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......