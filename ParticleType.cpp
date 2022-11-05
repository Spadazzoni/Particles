#include "ParticleType.hpp"

ParticleType::ParticleType(char* fName, double fMass, int fCharge)
    : fName_(fName), fMass_(fMass), fCharge_(fCharge) {}
char* ParticleType::GetfName() const { return fName_; }
double ParticleType::GetfMass() const { return fMass_; }
int ParticleType::GetfCharge() const { return fCharge_; }
void ParticleType::Print() {
  std::cout << "Name: " << *fName_ << '\n';
  std::cout << "Mass: " << fMass_ << '\n';
  std::cout << "Charge: " << fCharge_ << '\n';
}
double ParticleType::GetfWidth() const {
  return 0.;
}