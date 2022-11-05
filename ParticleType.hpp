#include <iostream>

#ifndef PARTICLE_TYPE_HPP
#define PARTICLE_TYPE_HPP

class ParticleType {
 public:
  ParticleType(char*, double, int);
  char* GetfName() const;
  double GetfMass() const;
  int GetfCharge() const;
  virtual void Print();
  virtual double GetfWidth() const;

 private:
  char* const fName_;
  double const fMass_;
  int const fCharge_;
};

#endif