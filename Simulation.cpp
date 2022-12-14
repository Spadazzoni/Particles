#include "Particle.hpp"
#include "TCanvas.h"
#include "TFile.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TH3F.h"
#include "TRandom.h"

void Main() {
  R__LOAD_LIBRARY(ParticleType_cpp.so);
  R__LOAD_LIBRARY(ResonanceType_cpp.so);
  R__LOAD_LIBRARY(Particle_cpp.so);
  // Setting the seed for PRNG
  gRandom->SetSeed();
  std::vector<Particle> EventParticles;
  std::vector<Particle> ResonanceEvents;
  // Preparing and Filling fParticleType
  char* ppion = new char('p');   // positive pion
  char* npion = new char('n');   // negative pion
  char* pkaon = new char('k');   // positive kaon
  char* nkaon = new char('q');   // negative kaon
  char* pprot = new char('+');   // positive proton
  char* nprot = new char('-');   // negative proton
  char* decayk = new char('d');  // resonance
  char* ntot[7] = {ppion, npion, pkaon, nkaon, pprot, nprot, decayk};
  double mass[7] = {0.13957, 0.13957, 0.49367, 0.49367,
                    0.93827, 0.93827, 0.89166};
  double charge[7] = {1, -1, 1, -1, 1, -1, 0};
  double res[7] = {0., 0., 0., 0., 0., 0., 0.050};
  for (int i = 0; i < 7; ++i) {
    Particle::AddParticleType(ntot[i], mass[i], charge[i], res[i]);
  }
  // Creating Histograms for further use
  TH1F* types =
      new TH1F("types", "Abundancies of generated Particles", 7, 0, 7);
  TH2F* angles = new TH2F("angles", "Distribution of azimutal and polar angles",
                          100, 0, 2 * M_PI, 100, 0, M_PI);
  TH1F* pav = new TH1F("pav", "Average Impulse Distribution", 1000, 0, 7);
  TH1F* impulse =
      new TH1F("impulse", "Distribution of trasverse impulse", 1000, 0, 5);
  TH1F* energy = new TH1F("energy", "Distribution of energy", 1000, 0, 7);
  TH1F* invmass0 =
      new TH1F("inv mass0", "Invariant Mass Distribution", 1000, 0, 7);
  TH1F* invmass1 =
      new TH1F("inv mass1", "Invariant Mass Distribution (opposite charges)",
               100, 0.75, 1.05);
  TH1F* invmass2 =
      new TH1F("inv mass2", "Invariant Mass Distribution (same charges)", 100,
               0.75, 1.05);
  TH1F* invmass3 = new TH1F(
      "inv mass3", "Invariant Mass Distribution (p+/k- or p-/k+)", 1000, 0, 7.);
  TH1F* invmass4 = new TH1F(
      "inv mass4", "Invariant Mass Distribution (p+/k+ or p-/k-)", 1000, 0, 7.);
  TH1F* invmass5 = new TH1F("inv mass5", "Invariant Mass Distribution (decay)",
                            1000, 0, 2);  // togli
  TH1* htot[11] = {types,    angles,   pav,      impulse,  energy,  invmass0,
                   invmass1, invmass2, invmass3, invmass4, invmass5};
  for (int i = 5; i < 11; ++i) {
    htot[i]->Sumw2();
  }
  // Generation cycle
  for (int i = 0; i < 1E5; ++i) {
    for (int j = 0; j < 1E2; ++j) {
      Particle p{*ppion};
      double phi = gRandom->Uniform(0, 2 * M_PI);
      double theta = gRandom->Uniform(0, M_PI);
      double average_p = gRandom->Exp(1.);
      double px = average_p * sin(theta) * cos(phi);
      double py = average_p * sin(theta) * sin(phi);
      double pz = average_p * cos(theta);
      double x = gRandom->Rndm();
      double y = gRandom->Rndm();
      if (x < 0.4) {
        p = Particle{*ppion, px, py, pz};
      } else {
        if (x < 0.8) {
          p = Particle{*npion, px, py, pz};
        } else {
          if (x < 0.85) {
            p = Particle{*pkaon, px, py, pz};
          } else {
            if (x < 0.9) {
              p = Particle{*nkaon, px, py, pz};
            } else {
              if (x < 0.945) {
                p = Particle{*pprot, px, py, pz};
              } else {
                if (x < 0.99) {
                  p = Particle{*nprot, px, py, pz};
                } else {
                  p = Particle{*decayk, px, py, pz};
                  if (y < 0.5) {
                    Particle dec1ppion{*ppion};
                    Particle dec2nkaon{*nkaon};
                    p.Decay2body(dec1ppion, dec2nkaon);
                    ResonanceEvents.push_back(dec1ppion);
                    ResonanceEvents.push_back(dec2nkaon);
                    invmass5->Fill(dec1ppion.InvMass(dec2nkaon));
                  } else {
                    Particle dec1npion{*npion};
                    Particle dec2pkaon{*pkaon};
                    p.Decay2body(dec1npion, dec2pkaon);
                    ResonanceEvents.push_back(dec1npion);
                    ResonanceEvents.push_back(dec2pkaon);
                    invmass5->Fill(dec1npion.InvMass(dec2pkaon));
                  }
                }
              }
            }
          }
        }
      }
      EventParticles.push_back(p);
      types->Fill(p.GetfIndex());  // tipi di particelle generate
      angles->Fill(phi, theta);    // distribuzione degli angoli
      pav->Fill(average_p);        // distribuzione dell'impulso medio
      impulse->Fill(sqrt(pow(px, 2) +
                         pow(py, 2)));  // distribuzione dell'impulso trasverso
      energy->Fill(p.GetParticleEnergy());
    }
    int nresonance = ResonanceEvents.size();
    for (int m = 0; m < nresonance; ++m) {
      EventParticles.push_back(ResonanceEvents[m]);
    }
    int size = EventParticles.size();
    for (int k = 0; k < size - 1; ++k) {
      if (EventParticles[k].GetfIndex() == 6) {
        continue;
      }
      for (int l = k + 1; l < size; ++l) {
        if (EventParticles[l].GetfIndex() == 6 || k == l) {
          continue;
        }
        double m = EventParticles[k].InvMass(EventParticles[l]);
        invmass0->Fill(m);
        if (EventParticles[k].GetCharge() * EventParticles[l].GetCharge() ==
            -1) {
          invmass1->Fill(m);
        }
        if (EventParticles[k].GetCharge() * EventParticles[l].GetCharge() ==
            1) {
          invmass2->Fill(m);
        }
        if ((EventParticles[k].GetfIndex() == 0 &&
             EventParticles[l].GetfIndex() == 3) ||
            (EventParticles[k].GetfIndex() == 1 &&
             EventParticles[l].GetfIndex() == 2)) {
          invmass3->Fill(m);
        }
        if ((EventParticles[k].GetfIndex() == 0 &&
             EventParticles[l].GetfIndex() == 2) ||
            (EventParticles[k].GetfIndex() == 1 &&
             EventParticles[l].GetfIndex() == 3)) {
          invmass4->Fill(m);
        }
      }
    }
    EventParticles.clear();
    ResonanceEvents.clear();
  }
  // Saving the Histograms
  TFile* f = new TFile("Particles.root", "RECREATE");
  for (int i = 0; i < 11; ++i) {
    htot[i]->Write();
  }
  f->Close();
  // Cleaning up
  delete ppion;
  delete npion;
  delete pkaon;
  delete nkaon;
  delete pprot;
  delete nprot;
  delete decayk;
}