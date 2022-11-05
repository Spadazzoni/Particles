#include "Particle.hpp"

std::vector<ParticleType*> Particle::fParticleType;
int Particle::FindParticle(char fName) {
  auto res = std::find_if(fParticleType.begin(), fParticleType.end(),
                          [&](auto i) { return *i->GetfName() == fName; });
  if (res == fParticleType.end()) {
    return 11;
  } else {
    return std::distance(fParticleType.begin(), res);
  }
}
Particle::Particle(char fName) {
  if (FindParticle(fName) == 11) {
    std::cout << "Cannot find the Particle. ";
  } else {
    fIndex = FindParticle(fName);
  }
}
Particle::Particle(char fName, double fPx, double fPy, double fPz)
    : fPx_(fPx), fPy_(fPy), fPz_(fPz) {
  if (FindParticle(fName) == 11) {
    std::cout << "Cannot find the Particle. ";
  } else {
    fIndex = FindParticle(fName);
  }
}
void Particle::AddParticleType(char* fName, double fMass, int fCharge,
                               double fWidth) {
  if (FindParticle(*fName) >= 0 && FindParticle(*fName) <= 9) {
    std::cout << "This Particle already exists. ";
    return;
  } else {
    if (fParticleType.size() >= fMaxNumParticleType) {
      std::cout << "Maximum number of Particles reached. ";
      return;
    } else {
      ParticleType* p = new ResonanceType(fName, fMass, fCharge, fWidth);
      fParticleType.push_back(p);
    }
  }
}
void Particle::AddParticleType(char* fName, double fMass, int fCharge) {
  if (FindParticle(*fName) >= 0 && FindParticle(*fName) <= 9) {
    std::cout << "This Particle already exists. ";
    return;
  } else {
    if (fParticleType.size() >= fMaxNumParticleType) {
      std::cout << "Maximum number of Particles reached. ";
      return;
    } else {
      ParticleType* p = new ParticleType(fName, fMass, fCharge);
      fParticleType.push_back(p);
    }
  }
}
int Particle::GetfIndex() const { return fIndex; }
void Particle::SetfIndex(int i) { fIndex = i; }
void Particle::SetfIndex(char* c) { fIndex = FindParticle(*c); }
void Particle::Print() {
  int n = fParticleType.size();
  for (int i = 0; i < n; ++i) {
    std::cout << "Particle n. " << i << " : " << '\n';
    fParticleType[i]->Print();
  }
}
void Particle::PrintParticle() {
  std::cout << "Index: " << fIndex << '\n';
  std::cout << "Name: " << *fParticleType[fIndex]->GetfName() << '\n';
  std::cout << "Impulse x: " << fPx_ << '\n';
  std::cout << "Impulse y: " << fPy_ << '\n';
  std::cout << "Impulse z: " << fPz_ << '\n';
}
double Particle::GetfPx() const { return fPx_; }
double Particle::GetfPy() const { return fPy_; }
double Particle::GetfPz() const { return fPz_; }
double Particle::GetMass() const { return fParticleType[fIndex]->GetfMass(); }
double Particle::GetParticleEnergy() const {
  return sqrt(pow(GetMass(), 2) + pow(fPx_, 2) + pow(fPy_, 2) + pow(fPz_, 2));
}
double Particle::InvMass(Particle& p) const {
  return sqrt(pow(p.GetParticleEnergy() + GetParticleEnergy(), 2) -
              (pow(p.GetfPx() + GetfPx(), 2) + pow(p.GetfPy() + GetfPy(), 2) +
               pow(p.GetfPz() + GetfPz(), 2)));
}
void Particle::SetP(double px, double py, double pz) {
  fPx_ = px;
  fPy_ = py;
  fPz_ = pz;
}
void Particle::PrintSize() { std::cout << fParticleType.size() << '\n'; }
int Particle::GetSize() { return fParticleType.size(); }
int Particle::Decay2body(Particle& dau1, Particle& dau2) const {
  if (GetMass() == 0.0) {
    printf("Decayment cannot be preformed if mass is zero\n");
    return 1;
  }
  double massMot = GetMass();
  double massDau1 = dau1.GetMass();
  double massDau2 = dau2.GetMass();
  if (fIndex > -1) {  // add width effect
    // gaussian random numbers
    float x1, x2, w, y1;
    double invnum = 1. / RAND_MAX;
    do {
      x1 = 2.0 * rand() * invnum - 1.0;
      x2 = 2.0 * rand() * invnum - 1.0;
      w = x1 * x1 + x2 * x2;
    } while (w >= 1.0);
    w = sqrt((-2.0 * log(w)) / w);
    y1 = x1 * w;
    massMot += fParticleType[fIndex]->GetfWidth() * y1;
  }
  if (massMot < massDau1 + massDau2) {
    printf(
        "Decayment cannot be preformed because mass is too low in this "
        "channel\n");
    return 2;
  }
  double pout =
      sqrt(
          (massMot * massMot - (massDau1 + massDau2) * (massDau1 + massDau2)) *
          (massMot * massMot - (massDau1 - massDau2) * (massDau1 - massDau2))) /
      massMot * 0.5;
  double norm = 2 * M_PI / RAND_MAX;
  double phi = rand() * norm;
  double theta = rand() * norm * 0.5 - M_PI / 2.;
  dau1.SetP(pout * sin(theta) * cos(phi), pout * sin(theta) * sin(phi),
            pout * cos(theta));
  dau2.SetP(-pout * sin(theta) * cos(phi), -pout * sin(theta) * sin(phi),
            -pout * cos(theta));
  double energy =
      sqrt(fPx_ * fPx_ + fPy_ * fPy_ + fPz_ * fPz_ + massMot * massMot);
  double bx = fPx_ / energy;
  double by = fPy_ / energy;
  double bz = fPz_ / energy;
  dau1.Boost(bx, by, bz);
  dau2.Boost(bx, by, bz);
  return 0;
}
void Particle::Boost(double bx, double by, double bz) {
  double energy = GetParticleEnergy();
  // Boost this Lorentz vector
  double b2 = bx * bx + by * by + bz * bz;
  double gamma = 1.0 / sqrt(1.0 - b2);
  double bp = bx * fPx_ + by * fPy_ + bz * fPz_;
  double gamma2 = b2 > 0 ? (gamma - 1.0) / b2 : 0.0;
  fPx_ += gamma2 * bp * bx + gamma * bx * energy;
  fPy_ += gamma2 * bp * by + gamma * by * energy;
  fPz_ += gamma2 * bp * bz + gamma * bz * energy;
}
int Particle::GetCharge() const {return fParticleType[fIndex]->GetfCharge();}
