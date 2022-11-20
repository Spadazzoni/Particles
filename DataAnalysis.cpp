#include <fstream>

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

void Data() {
  gStyle->SetOptStat(2210);
  gStyle->SetOptFit(1111);
  gStyle->SetStatW(0.3);
  gStyle->SetStatH(0.15);
  gStyle->SetStatX(0.9);
  gStyle->SetStatY(0.9);
  TFile *file = new TFile("Particles.root");
  TH1 *htot[13];
  TString s[13] = {"types",     "angles",    "pav",       "impulse",
                   "energy",    "inv mass0", "inv mass1", "inv mass2",
                   "inv mass3", "inv mass4", "inv mass5", "inv mass1c",
                   "inv mass2c"};
  TString names[7] = {"#pi+", "#pi-", "k+", "k-", "p+", "p-", "k*"};
  TString Xtitles[4] = {"Particles", "Polar Angle (rad)",
                        "Azimuthal Angle (rad)", "Mean Impulse (Gev)"};
  for (int i = 0; i < 11; ++i) {
    htot[i] = (TH1 *)file->Get(s[i]);
  }
  htot[11] = (TH1 *)file->Get(s[11]);
  htot[12] = (TH1 *)file->Get(s[12]);
  TF1 *f1 = new TF1("f1", "[0]", 0, M_PI);      // theta
  TF1 *f2 = new TF1("f2", "[0]", 0, 2 * M_PI);  // phi
  TH1D *AngleX = ((TH2F *)file->Get(s[1]))->ProjectionX("AngleX", 0, 100);
  TH1D *AngleY = ((TH2F *)file->Get(s[1]))->ProjectionY("AngleY", 0, 100);
  AngleX->Fit("f2", "Q0");
  AngleY->Fit("f1", "Q0");
  TF1 *f3 = new TF1("f3", "[0]*e^(-[1]*x)", 0, 7);
  htot[2]->Fit("f3", "Q0");
  TH1F *SumCharges = new TH1F(*(TH1F *)file->Get(s[6]));
  TH1F *SumParticles = new TH1F(*(TH1F *)file->Get(s[8]));
  SumCharges->Add(htot[6], htot[7], 1, -1);
  SumCharges->SetTitle("Opposite charges minus Same charges");
  SumParticles->Add(htot[8], htot[9], 1, -1);
  SumParticles->SetTitle("p+/k- and p-/k+ minus p+/k+ and p-/k-");
  TF1 *f4 = new TF1("f4", "gaus", 0, 10);
  TF1 *f5 = new TF1("f5", "gaus", 0, 7);
  SumCharges->Fit("f4", "Q0");
  SumParticles->Fit("f5", "Q0");
  TF1 *f6 = new TF1("f6", "gaus", 0.6, 1.2);
  TF1 *ftot[6] = {f2, f1, f3, f6, f4, f5};
  TH1 *HDraw[7] = {htot[0],  AngleX,     AngleY,      htot[2],
                   htot[10], SumCharges, SumParticles};
  htot[10]->Fit("f6", "Q0");
  std::ofstream txt("./HistoData.txt", std::ofstream::out);
  if (txt.is_open()) {
    txt << "=============================================" << '\n';
    txt << "|| ANALYZING GENERATED PARTICLE HISTOGRAMS ||" << '\n';
    txt << "=============================================" << '\n';
    txt << '\n';
    txt << '\n';
    txt << "->HISTOS ENTRIES: " << '\n';
    for (int i = 0; i < 11; ++i) {
      txt << htot[i]->GetTitle() << " has " << htot[i]->GetEntries()
          << " entries." << '\n';
    }
    txt << '\n';
    txt << '\n';
    txt << "->NUMBER OF OCCURRENCIES FOR EACH PARTICLE: " << '\n';
    for (int i = 0; i < 7; ++i) {
      txt << names[i] << " was generated " << htot[0]->GetBinContent(i + 1)
          << " +/- " << htot[0]->GetBinError(i + 1) << " times." << '\n';
    }
    txt << '\n';
    txt << '\n';
    txt << "->ABUNDANCIES (%): " << '\n';
    for (int i = 0; i < 7; ++i) {
      txt << names[i] << " " << ((htot[0]->GetBinContent(i + 1)) / 1E7) * 100
          << " %" << '\n';
    }
    txt << '\n';
    txt << '\n';
    txt << "->POLAR ANGLES DISTRIBUTIONS: " << '\n';
    txt << "-->PHI:" << '\n';
    txt << "Mean Entries per Bin: " << f2->GetParameter(0) << " +/- "
        << f2->GetParError(0) << '\n';
    txt << "Mean: " << AngleX->GetMean() << " +/- " << AngleX->GetMeanError()
        << "  rad" << '\n';
    txt << "Chi square: " << f2->GetChisquare() << '\n';
    txt << "Degrees of Freedom: " << f2->GetNDF() << '\n';
    txt << " Reduced Chi square: " << f2->GetChisquare() / f2->GetNDF() << '\n';
    txt << "-->THETA:" << '\n';
    txt << "Mean Entries per Bin: " << f1->GetParameter(0) << " +/- "
        << f1->GetParError(0) << '\n';
    txt << "Mean: " << AngleY->GetMean() << " +/- " << AngleY->GetMeanError()
        << "  rad" << '\n';
    txt << "Chi square: " << f1->GetChisquare() << '\n';
    txt << "Degrees of Freedom: " << f1->GetNDF() << '\n';
    txt << "Reduced Chi square: " << f1->GetChisquare() / f1->GetNDF() << '\n';
    txt << '\n';
    txt << '\n';
    txt << "->AVERAGE IMPULSE DISTRIBUTION: " << '\n';
    txt << "Multiplicative coefficient of exponential fit: "
        << f3->GetParameter(0) << " +/- " << f3->GetParError(0) << "  GeV"
        << '\n';
    txt << "Mean: " << f3->GetParameter(1) << " +/- " << f3->GetParError(1)
        << '\n';
    txt << "Chi square: " << f3->GetChisquare() << '\n';
    txt << "Degrees of Freedom: " << f3->GetNDF() << '\n';
    txt << "Reduced Chi square: " << f3->GetChisquare() / f3->GetNDF() << '\n';
    txt << "Fit probability: " << f3->GetProb() << '\n';
    txt << '\n';
    txt << '\n';
    txt << "INVARIANT MASS BETWEEN DECAY PRODUCTS OF SAME K*: " << '\n';
    txt << "Mean: " << f6->GetParameter(1) << " +/- " << f6->GetParError(1)
        << "  GeV/c^2" << '\n';
    txt << "RMS: " << f6->GetParameter(2) << " +/- " << f6->GetParError(2)
        << "  GeV/c^2" << '\n';
    txt << "Width: " << f6->GetParameter(0) << " +/- " << f6->GetParError(0)
        << "  GeV/c^2" << '\n';
    txt << "Reduced Chi square: " << f6->GetChisquare() / f6->GetNDF() << '\n';
    txt << '\n';
    txt << '\n';
    txt << "->INVARIANT MASS DISTRIBUTIONS: " << '\n';
    txt << "-->SAME CHARGES AND OPPOSITE CHARGES:" << '\n';
    txt << "Mean: " << f4->GetParameter(1) << " +/- " << f4->GetParError(1)
        << "  GeV/c^2" << '\n';
    txt << "RMS: " << f4->GetParameter(2) << " +/- " << f4->GetParError(2)
        << "  GeV/c^2" << '\n';
    txt << "Width: " << f4->GetParameter(0) << " +/- " << f4->GetParError(0)
        << "  GeV/c^2" << '\n';
    txt << "Reduced Chi Square: " << f4->GetChisquare() / f4->GetNDF() << '\n';
    txt << "Fit probability: " << f4->GetProb() << '\n';
    txt << "-->PARTICLES AND ANTIPARTICLES:" << '\n';
    txt << "Mean: " << f5->GetParameter(1) << " +/- " << f5->GetParError(1)
        << "  GeV/c^2" << '\n';
    txt << "RMS: " << f5->GetParameter(2) << " +/- " << f5->GetParError(2)
        << "  GeV/c^2" << '\n';
    txt << "Width: " << f5->GetParameter(0) << " +/- " << f5->GetParError(0)
        << "  GeV/c^2" << '\n';
    txt << "Reduced Chi Square: " << f5->GetChisquare() / f5->GetNDF() << '\n';
    txt << "Fit probability: " << f4->GetProb() << '\n';
    txt << '\n';
    txt << '\n';
    txt << "//FILE END//";
    txt.close();
  } else {
    std::cout << "Cannot find or open file." << '\n';
  }
  TCanvas *c0 = new TCanvas("c0", "MyCanvas0", 200, 10, 800, 400);
  TCanvas *c1 = new TCanvas("c1", "MyCanvas1", 200, 10, 800, 400);
  TCanvas *c2 = new TCanvas("c2", "MyCanvas2", 200, 10, 800, 400);
  c0->Print("Invariant mass distributions histograms.pdf[");
  c0->Divide(2, 3);
  c1->Print("ParticlesHistos.pdf[");
  c2->Print("InvMass.pdf[");
  c2->Divide(2, 2);
  c1->Divide(2, 2);
  c0->cd();
  for (int i = 0; i < 6; ++i) {
    c0->cd(i + 1);
    if (i == 1 || i == 2) {
      htot[i + 10]->GetXaxis()->SetTitle("Invariant Mass (GeV/c^{2})");
      htot[i + 10]->GetYaxis()->SetTitle("Entries");
      htot[i + 10]->SetLineColor(kBlack);
      htot[i + 10]->SetTitleSize(0.055, "x");
      htot[i + 10]->SetTitleSize(0.055, "y");
      htot[i + 10]->GetXaxis()->SetTitleOffset(0.9);
      htot[i + 10]->GetYaxis()->SetTitleOffset(0.7);
      htot[i + 10]->GetYaxis()->SetLabelSize(0.055);
      htot[i + 10]->GetXaxis()->SetLabelSize(0.055);
      htot[i + 10]->SetFillColor(40);
      htot[i + 10]->DrawCopy();
    } else {
      htot[i + 5]->GetXaxis()->SetTitle("Invariant Mass (GeV/c^{2})");
      htot[i + 5]->GetYaxis()->SetTitle("Entries");
      htot[i + 5]->SetTitleSize(0.055, "x");
      htot[i + 5]->SetTitleSize(0.055, "y");
      htot[i + 5]->Sumw2(kFALSE);
      htot[i + 5]->GetXaxis()->SetTitleOffset(0.82);
      htot[i + 5]->GetYaxis()->SetTitleOffset(0.7);
      htot[i + 5]->GetYaxis()->SetLabelSize(0.055);
      htot[i + 5]->GetXaxis()->SetLabelSize(0.055);
      htot[i + 5]->SetLineColor(kBlack);
      htot[i + 5]->SetFillColor(40);
      htot[i + 5]->DrawCopy();
    }
  }
  c1->cd();
  for (int j = 0; j < 4; ++j) {
    c1->cd(j + 1);
    HDraw[j]->GetXaxis()->SetTitle(Xtitles[j]);
    HDraw[j]->GetYaxis()->SetTitle("Entries");
    HDraw[j]->SetLineColor(kBlack);
    HDraw[j]->SetFillColor(40);
    HDraw[j]->DrawCopy();
    if (j != 0) {
      ftot[j - 1]->SetLineColor(kAquamarine);
      ftot[j - 1]->Draw("same");
    }
  }
  c2->cd();
  for (int i = 4; i < 7; ++i) {
    c2->cd(i - 3);
    HDraw[i]->GetXaxis()->SetTitle("Invariant Mass (GeV/c^{2})");
    HDraw[i]->GetYaxis()->SetTitle("Entries");
    HDraw[i]->SetLineColor(kBlack);
    HDraw[i]->SetFillColor(40);
    HDraw[i]->DrawCopy("hist");
    ftot[i - 1]->SetLineColor(kAquamarine);
    ftot[i - 1]->Draw("same");
  }
  c0->Print("Invariant mass distributions histograms.pdf");
  c0->Print("Invariant mass distributions histograms.pdf]");
  c1->Print("ParticlesHistos.pdf");
  c1->Print("ParticlesHistos.pdf]");
  c2->Print("InvMass.pdf");
  c2->Print("InvMass.pdf]");
}
