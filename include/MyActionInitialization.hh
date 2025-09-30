//
/// \file MyExampleB1/include/MyActionInitialization.hh
/// \brief Definition of the MyExampleB1::MyActionInitialization class

#ifndef MyActionInitialization_h
#define MyActionInitialization_h 1

#include "G4VUserActionInitialization.hh"

namespace MyExampleB1
{

/// Action initialization class.

class MyActionInitialization : public G4VUserActionInitialization
{
  public:
    MyActionInitialization() = default;
    ~MyActionInitialization() override = default;

    void BuildForMaster() const override;
    void Build() const override;
};

}  // namespace MyExampleB1

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
