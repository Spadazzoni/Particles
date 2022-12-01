#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <vector>

#include "ResonanceType.hpp"

#ifndef PARTICLE_HPP
#define PARTICLE_HPP

class Particle {
 public:
  // Default Constructor
  Particle(char);
  Particle(char, double, double, double);
  // Returns the Index of the Particle
  int GetfIndex() const;
  // Adds a new ResonanceType to fParticleType
  void static AddParticleType(char *, double, int, double);
  // Add a a new ParticleType to fParticleType
  void static AddParticleType(char *, double, int);
  // Sets the Index of the Particle
  void SetfIndex(int);
  // Sets the Index of the Particle
  void SetfIndex(char *);
  void static Print();
  void PrintParticle();
  // Returns the x component of Impulse
  double GetfPx() const;
  // Returns the y component of Impulse
  double GetfPy() const;
  // Returns the z component of Impulse
  double GetfPz() const;
  // Returns the Mass of the Particle
  double GetMass() const;
  // Returns the Charge of the Particle
  int GetCharge() const;
  // Returns the Energy of the Particle
  double GetParticleEnergy() const;
  // Computes Invariant Mass
  double InvMass(Particle &) const;
  // Sets the components of Impulse
  void SetP(double, double, double);
  int static GetSize();
  void static PrintSize();
  // Handles the Decay of a Particle
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
