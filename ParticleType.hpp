#include <iostream>

#ifndef PARTICLE_TYPE_HPP
#define PARTICLE_TYPE_HPP

class ParticleType {
 public:
  ParticleType(char*, double, int);
  // Returns the Name of the ParticleType
  char* GetfName() const;
  // Returns the Mass of the ParticleType
  double GetfMass() const;
  // Returns the Charge of the ParticleType
  int GetfCharge() const;
  // Prints the values of Name, Mass and Charge
  virtual void Print();
  // Returns the Width of the ParticleType
  virtual double GetfWidth() const;

 private:
  char* const fName_;
  double const fMass_;
  int const fCharge_;
};

#endif