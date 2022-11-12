#include "Particle.hpp"
#include "TCanvas.h"
#include "TF1.h"
#include "TFile.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TH3F.h"
#include "TRandom.h"
#include "TString.h"
#include "TStyle.h"

/*Double_t Exp(Double_t *x, Double_t *par) {
  Float_t xx = x[0];
  Double_t val = par[0] * exp(par[1] * xx);
  return val;
}*/

void Data() {
  gStyle->SetOptStat(2210);
  gStyle->SetOptFit(1111);
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
  // htot[7]-htot[6] e htot[9]-htot[8]
  TH1F *SumCharges = new TH1F(*(TH1F *)file->Get(s[6]));
  TH1F *SumParticles = new TH1F(*(TH1F *)file->Get(s[8]));
  SumCharges->Add(htot[6], htot[7], 1, -1);
  SumCharges->SetTitle("Opposite charges minus Same charges");
  SumParticles->Add(htot[8], htot[9], 1, -1);
  SumParticles->SetTitle("p+/k- and p-/k+ minus p+/k+ and p-/k-");
  std::cout << "_________________________________" << '\n';
  std::cout << "Invariant mass: " << '\n';
  std::cout << "_________________________________" << '\n';
  std::cout << "X value for Y max (charges): "
            << 0.01 * SumCharges->GetMaximumBin() << '\n';
  std::cout << "X value for Y max (particles): "
            << 0.01 * SumParticles->GetMaximumBin() << '\n';
  TF1 *f4 = new TF1("f4", "gaus", 0, 10);
  TF1 *f5 = new TF1("f5", "gaus", 0, 3);
  SumCharges->Fit("f4", "Q0");
  SumParticles->Fit("f5", "Q0");
  std::cout << "-> CHARGES" << '\n';
  std::cout << "Mean: " << f4->GetParameter(1) << " +/- " << f4->GetParError(1)
            << '\n';
  std::cout << "RMS: " << f4->GetParameter(2) << " +/- " << f4->GetParError(2)
            << '\n';
  std::cout << "Chi Square: " << f4->GetChisquare() / f4->GetNDF() << '\n';
  std::cout << "Fit probability: " << f4->GetProb() << '\n';
  std::cout << "-> PARTICLES" << '\n';
  std::cout << "Mean: " << f5->GetParameter(1) << " +/- " << f5->GetParError(1)
            << '\n';
  std::cout << "RMS: " << f5->GetParameter(2) << " +/- " << f5->GetParError(2)
            << '\n';
  std::cout << "Chi Square: " << f5->GetChisquare() / f5->GetNDF() << '\n';
  std::cout << "Fit probability: " << f4->GetProb() << '\n';
  TCanvas *c0 = new TCanvas("c0", "MyCanvas0", 200, 10, 800, 400);
  c0->Divide(3, 2);
  for (int i = 0; i < 6; ++i) {
    c0->cd(i + 1);
    htot[i + 5]->DrawCopy();
  }
  TCanvas *c1 = new TCanvas("c1", "MyCanvas1", 200, 10, 1000, 400);
  c1->Divide(2,1);
  c1->cd(1);
  SumCharges->Draw();
  c1->cd(2);
  SumParticles->Draw();
}
