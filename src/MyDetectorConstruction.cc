//
/// \file MyB1/src/MyDetectorConstruction.cc
/// \brief Implementation of the MyExampleB1::MyDetectorConstruction class

#include "MyDetectorConstruction.hh"

#include "G4Tubs.hh"
#include "G4Box.hh"
#include "G4Cons.hh"
#include "G4LogicalVolume.hh"
#include "G4NistManager.hh"
#include "G4PVPlacement.hh"
#include "G4SystemOfUnits.hh"
#include "G4Trd.hh"
#include "G4UserLimits.hh"
#include "G4Region.hh"
#include "G4ProductionCuts.hh"
#include "G4UnitsTable.hh"


namespace MyExampleB1
{

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4VPhysicalVolume* MyDetectorConstruction::Construct()
{
  // Get nist material manager
  G4NistManager* nist = G4NistManager::Instance();

  // Envelope parameters
  //
  G4double base_size = 5 * cm;
  G4double env_sizeXY = base_size;
  G4double env_sizeZ = base_size;

  // G4Material* env_mat = nist->FindOrBuildMaterial("G4_Pb");

  // Elements
  G4Element* Bi = nist->FindOrBuildElement("Bi");
  G4Element* Ge = nist->FindOrBuildElement("Ge");
  G4Element* O  = nist->FindOrBuildElement("O");

  // Build BGO (Bi4Ge3O12)
  G4Material* env_mat = new G4Material("BGO", 7.13*g/cm3, 3);
  env_mat->AddElement(Bi, 4);
  env_mat->AddElement(Ge, 3);
  env_mat->AddElement(O, 12);

  // Option to switch on/off checking of volumes overlaps
  //
  G4bool checkOverlaps = true;

  //
  // World
  //
  G4double world_sizeXY = 20 * env_sizeXY;
  G4double world_sizeZ = 20 * env_sizeZ;
  G4Material* world_mat = nist->FindOrBuildMaterial("G4_AIR");

  auto solidWorld =
    new G4Box("World",  // its name
              0.5 * world_sizeXY, 0.5 * world_sizeXY, 0.5 * world_sizeZ);  // its size

  auto logicWorld = new G4LogicalVolume(solidWorld,  // its solid
                                        world_mat,  // its material
                                        "World");  // its name

  auto physWorld = new G4PVPlacement(nullptr,  // no rotation
                                     G4ThreeVector(),  // at (0,0,0)
                                     logicWorld,  // its logical volume
                                     "World",  // its name
                                     nullptr,  // its mother  volume
                                     false,  // no boolean operation
                                     0,  // copy number
                                     checkOverlaps);  // overlaps checking

  //
  // Envelope
  //
  auto solidEnv = new G4Box("Envelope",  // its name
                            0.5 * env_sizeXY, 0.5 * env_sizeXY, 0.5 * env_sizeZ);  // its size
  
  auto logicEnv = new G4LogicalVolume(solidEnv,  // its solid
                                      env_mat,  // its material
                                      "Envelope");  // its name

  new G4PVPlacement(nullptr,  // no rotation
                    G4ThreeVector(),  // at (0,0,0)
                    logicEnv,  // its logical volume
                    "Envelope",  // its name
                    logicWorld,  // its mother  volume
                    false,  // no boolean operation
                    0,  // copy number
                    checkOverlaps);  // overlaps checking


  // print material parameters
  G4cout << "=== marterial parameters ===" << G4endl;
  G4cout << "env_sizeXY = " << G4BestUnit(env_sizeXY, "Length") << G4endl;
  G4cout << "env_sizeZ      = " << G4BestUnit(env_sizeZ, "Length") << G4endl;

                  
  // Set Shape2 as scoring volume
  //
  fScoringVolume = logicEnv;

  //
  // always return the physical World
  //
  return physWorld;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

}  // namespace MyExampleB1
