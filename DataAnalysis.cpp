#include "Particle.hpp"
#include "TCanvas.h"
#include "TFile.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TH3F.h"
#include "TRandom.h"
#include "TString.h"
#include "TF1.h"

void Data() {
  TFile *file = new TFile("Particles.root");
  TH1 *htot[11];
  TString s[11] = {"types",     "angles",    "pav",       "impulse",
                   "energy",    "inv mass0", "inv mass1", "inv mass2",
                   "inv mass3", "inv mass4", "inv mass5"};
  TString names[7] = {"#pi+", "#pi-", "k+", "k-", "p+", "p-", "k*"};
  std::cout << "Histos Entries: " << '\n';
  std::cout << "_________________________________" << '\n';
  for (int i = 0; i < 11; ++i) {
    htot[i] = (TH1 *)file->Get(s[i]);
    std::cout << htot[i]->GetTitle() << " has " << htot[i]->GetEntries()
              << " entries." << '\n';
  }
  std::cout << "_________________________________" << '\n';
  std::cout << "Types of Particles generated: " << '\n';
  std::cout << "_________________________________" << '\n';
  for (int i = 0; i < 7; ++i) {
    std::cout << names[i] << " was generated " << htot[0]->GetBinContent(i+1) << " +/- " << htot[0]->GetBinError(i+1)
              << " times." << '\n';
  }
    std::cout << "_________________________________" << '\n';
  std::cout << "Percentage of Particles generated: " << '\n';
  std::cout << "_________________________________" << '\n';
    for (int i = 0; i < 7; ++i) {
    std::cout << names[i] << " " << ((htot[0]->GetBinContent(i+1))/1E7)*100 << " %" << '\n';
  }
  TF1 *f1 = new TF1("f1","[0]",0,M_PI);
  TF1 *f2 = new TF1("f2","[0]",0,2*M_PI);
  TH1D *AngleX = ((TH2F *)file->Get(s[1]))->ProjectionX("AngleX",0,100);
  TH1D *AngleY = ((TH2F *)file->Get(s[1]))->ProjectionY("AngleY",0,100);
  AngleX->Fit("f1");
  AngleY->Fit("f2");
}
