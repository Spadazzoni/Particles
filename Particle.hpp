#include <algorithm>
#include <cmath>
#include <vector>
#include <cstdlib>

#include "ResonanceType.hpp"

#ifndef PARTICLE_HPP
#define PARTICLE_HPP

class Particle {
 public:
  Particle(char);  // implementare il ctor di default
  Particle(char, double, double, double);
  int GetfIndex() const;
  void static AddParticleType(char *, double, int, double);
  void static AddParticleType(char *, double, int);
  void SetfIndex(int);
  void SetfIndex(char *);
  void static Print();
  void PrintParticle();
  double GetfPx() const;
  double GetfPy() const;
  double GetfPz() const;
  double GetMass() const;
  int GetCharge() const;
  double GetParticleEnergy() const;
  double InvMass(Particle &) const;
  void SetP(double, double, double);
  int static GetSize();
  void static PrintSize();
  int Decay2body(Particle &, Particle &) const;

 private:
  void Boost(double, double, double);
  std::vector<ParticleType *> static fParticleType;
  int static const fMaxNumParticleType = 10;
  int static fNParticleType;
  int fIndex;
  double fPx_ = 0.;
  double fPy_ = 0.;
  double fPz_ = 0.;
  int static FindParticle(char);
};

#endif
