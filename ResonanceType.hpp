#include "ParticleType.hpp"

#ifndef RESONANCE_TYPE_HPP
#define RESONANCE_TYPE_HPP

class ResonanceType : public ParticleType {
 public:
  ResonanceType(char*, double, int, double);
  // Returns the width of the ResonanceType
  double GetfWidth() const override;
  // Prints Name, Mass, Charge and Width of the ResonanceType
  void Print() override;

 private:
  double const fWidth_;
};

#endif