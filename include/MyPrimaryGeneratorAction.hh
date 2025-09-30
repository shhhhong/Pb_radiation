//
/// \file MyExampleB1/include/MyPrimaryGeneratorAction.hh
/// \brief Definition of the MyExampleB1::MyPrimaryGeneratorAction class

#ifndef MyPrimaryGeneratorAction_h
#define MyPrimaryGeneratorAction_h 1

#include "G4VUserPrimaryGeneratorAction.hh"
#include "G4GeneralParticleSource.hh"


class G4ParticleGun;
class G4Event;
class G4Box;
class G4Tubs;

namespace MyExampleB1
{

/// The primary generator action class with particle gun.
///
/// The default kinematic is a 6 MeV gamma, randomly distribued
/// in front of the phantom across 80% of the (X,Y) phantom size.

class MyPrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction
{
  public:
    MyPrimaryGeneratorAction();
    ~MyPrimaryGeneratorAction() override;

    // method from the base class
    void GeneratePrimaries(G4Event*) override;

    // method to access particle gun
   // const G4ParticleGun* GetParticleGun() const { return fParticleGun; }
    const G4GeneralParticleSource* GetGPS() const { return fGPS; }

  private:
   // G4ParticleGun* fParticleGun = nullptr;  // pointer a to G4 gun class
    G4Box* fEnvelopeBox = nullptr;
    G4GeneralParticleSource* fGPS;  // 核心：GPS

};

}  // namespace MyExampleB1

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
