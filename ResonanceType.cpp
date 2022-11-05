#include "ResonanceType.hpp"

ResonanceType::ResonanceType(char* fName, double fMass, int fCharge,
                             double fWidth)
    : ParticleType(fName, fMass, fCharge), fWidth_(fWidth) {}
double ResonanceType::GetfWidth() const { return fWidth_; }
void ResonanceType::Print() {
    ParticleType::Print();
    std::cout << "Width: " << fWidth_ << '\n';
}