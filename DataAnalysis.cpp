#include "Particle.hpp"
#include "TCanvas.h"
#include "TF1.h"
#include "TFile.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TH3F.h"
#include "TRandom.h"
#include "TString.h"

/*Double_t Exp(Double_t *x, Double_t *par) {
  Float_t xx = x[0];
  Double_t val = par[0] * exp(par[1] * xx);
  return val;
}*/

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
    std::cout << names[i] << " was generated " << htot[0]->GetBinContent(i + 1)
              << " +/- " << htot[0]->GetBinError(i + 1) << " times." << '\n';
  }
  std::cout << "_________________________________" << '\n';
  std::cout << "Percentage of Particles generated: " << '\n';
  std::cout << "_________________________________" << '\n';
  for (int i = 0; i < 7; ++i) {
    std::cout << names[i] << " "
              << ((htot[0]->GetBinContent(i + 1)) / 1E7) * 100 << " %" << '\n';
  }
  TF1 *f1 = new TF1("f1", "[0]", 0, M_PI);
  TF1 *f2 = new TF1("f2", "[0]", 0, 2 * M_PI);
  TH1D *AngleX = ((TH2F *)file->Get(s[1]))->ProjectionX("AngleX", 0, 100);
  TH1D *AngleY = ((TH2F *)file->Get(s[1]))->ProjectionY("AngleY", 0, 100);
  AngleX->Fit("f2", "Q0");
  AngleY->Fit("f1", "Q0");
  std::cout << "_________________________________" << '\n';
  std::cout << "Fit parameters and chi square of angles distribution: " << '\n';
  std::cout << "_________________________________" << '\n';
  std::cout << "Theta: " << f1->GetParameter(0) << " +/- " << f1->GetParError(0)
            << '\n';
  std::cout << "Phi: " << f2->GetParameter(0) << " +/- " << f2->GetParError(0)
            << '\n';
  std::cout << "Chi square (x): " << f2->GetChisquare() / f2->GetNDF() << '\n';
  std::cout << "Chi square (y): " << f1->GetChisquare() / f1->GetNDF() << '\n';
  TF1 *f3 = new TF1("f3", "[0]*e^([1]*x)", 0, 7);
  htot[2]->Fit("f3", "Q0");
  std::cout << "_________________________________" << '\n';
  std::cout << "Fit parameters and chi square of average impulse distribution: "
            << '\n';
  std::cout << "_________________________________" << '\n';
  std::cout << "Par1: " << f3->GetParameter(0) << " +/- " << f3->GetParError(0)
            << '\n';
  std::cout << "Par2: " << f3->GetParameter(1) << " +/- " << f3->GetParError(1)
            << '\n';
  std::cout << "Chi square: " << f3->GetChisquare() / f3->GetNDF() << '\n';
  std::cout << "Fit probability: " << f3->GetProb() << '\n';
  //htot[7]-htot[6] e htot[9]-htot[8]
  TH1F *SumCharges = new TH1F(*(TH1F *)file->Get(s[6]));
  TH1F *SumParticles = new TH1F(*(TH1F *)file->Get(s[8]));
  SumCharges->Add(htot[6],htot[7],1,-1);
  SumCharges->SetTitle("Opposite charges minus Same charges");
  SumParticles->Add(htot[8],htot[9],1,-1);
  SumParticles->SetTitle("p+/k- and p-/k+ minus p+/k+ and p-/k-");
  SumCharges->Draw();
  //SumParticles->Draw();
}
