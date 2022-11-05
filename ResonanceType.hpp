#include "ParticleType.hpp"

#ifndef RESONANCE_TYPE_HPP
#define RESONANCE_TYPE_HPP

class ResonanceType : public ParticleType {
 public:
  ResonanceType(char*, double, int, double);
  double GetfWidth() const override;
  void Print() override;

 private:
  double const fWidth_;
};

#endif