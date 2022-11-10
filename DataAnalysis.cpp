#include "Particle.hpp"
#include "TCanvas.h"
#include "TFile.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TH3F.h"
#include "TRandom.h"
#include "TString.h"

void DataAnalysis() {
  TFile *file = new TFile("Particles.root");
  TH1 *htot[11];
  TString s[11] = {"types",     "angles",    "pav",       "impulse",
                   "energy",    "inv mass0", "inv mass1", "inv mass2",
                   "inv mass3", "inv mass4", "inv mass5"};
  for (int i = 0; i < 11; ++i) {
    htot[i] = (TH1 *)file->Get(s[i]);
  }
}