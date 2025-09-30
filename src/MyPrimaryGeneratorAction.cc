//
/// \file MyExampleB1/src/MyPrimaryGeneratorAction.cc
/// \brief Implementation of the MyExampleB1::MyPrimaryGeneratorAction class

#include "MyPrimaryGeneratorAction.hh"

#include "G4Tubs.hh"
#include "G4Box.hh"
#include "G4LogicalVolume.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "G4SystemOfUnits.hh"
#include "Randomize.hh"

namespace MyExampleB1
{

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

MyPrimaryGeneratorAction::MyPrimaryGeneratorAction()
{
  G4int n_particle = 1;
  //fParticleGun = new G4ParticleGun(n_particle);
  fGPS = new G4GeneralParticleSource();
  fGPS->SetNumberOfParticles(n_particle); 
  

  // In order to avoid dependence of MyPrimaryGeneratorAction
  // on MyMyDetectorConstruction class we get Envelope volume
  // from G4LogicalVolumeStore.
  //fEnvelopeTubs = nullptr;
  
  // default particle kinematic
  G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
  G4String particleName;
  G4ParticleDefinition* particle = particleTable->FindParticle(particleName = "gamma");
  fGPS->GetCurrentSource()->SetParticleDefinition(particle);

  // Get the current particle source position distribution
  auto pos = fGPS->GetCurrentSource()->GetPosDist();
  // Set the position distribution type to a single point
  pos->SetPosDisType("Point");
  // Set the centre of the position distribution
  // (the default is (0,0,0))
  pos->SetCentreCoords(G4ThreeVector(0,0,-60*cm));

  // +z direction
  auto ang = fGPS->GetCurrentSource()->GetAngDist();
  ang->SetAngDistType("direction");
  ang->SetParticleMomentumDirection(G4ThreeVector(0,0,1));
  

  // Get the current particle source energy distribution
  auto ene = fGPS->GetCurrentSource()->GetEneDist();
  ene->SetEnergyDisType("Mono");
  ene->SetMonoEnergy(0.662 * MeV);


}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

MyPrimaryGeneratorAction::~MyPrimaryGeneratorAction()
{
  //delete fParticleGun;
  delete fGPS;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void MyPrimaryGeneratorAction::GeneratePrimaries(G4Event* event)
{
  // this function is called at the begining of ecah event
  //

  // In order to avoid dependence of MyPrimaryGeneratorAction
  // on MyMyDetectorConstruction class we get Envelope volume
  // from G4LogicalVolumeStore.

  G4double envSizeXY = 0;
  G4double envSizeZ = 0;

  if (!fEnvelopeBox) {
    G4LogicalVolume* envLV = G4LogicalVolumeStore::GetInstance()->GetVolume("Envelope");
    if (envLV) fEnvelopeBox = dynamic_cast<G4Box*>(envLV->GetSolid());
  }

  if (fEnvelopeBox) {
    envSizeXY = fEnvelopeBox->GetXHalfLength() * 2.;
    envSizeZ = fEnvelopeBox->GetZHalfLength() * 2.;
  }
  else {
    G4ExceptionDescription msg;
    msg << "Envelope volume of box shape not found.\n";
    msg << "Perhaps you have changed geometry.\n";
    msg << "The gun will be place at the center.";
    G4Exception("MyPrimaryGeneratorAction::GeneratePrimaries()", "MyCode0002", JustWarning, msg);
  }

  fGPS->GeneratePrimaryVertex(event);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

}  // namespace MyExampleB1
