
#ifndef GRAPHS_HPP
#define GRAPHS_HPP

#include "TAxis.h"
#include "TCanvas.h"
#include "TGraph.h"
#include <string>

inline void Graphs(int number) {
  std::string name_file1 =
      "Simulations/" + std::to_string(number) + "_Evolution_INFECTED.dat";
  std::string name_file2 =
      "Simulations/" + std::to_string(number) + "_Evolution_REMOVED.dat";
  std::string name_file3 =
      "Simulations/" + std::to_string(number) + "_Evolution_SUSCEPTIBLE.dat";
  char *file1 = &name_file1[0];
  char *file2 = &name_file2[0];
  char *file3 = &name_file3[0];
  TGraph *graphi = new TGraph(file1, "%lg %lg");
  TGraph *graphs = new TGraph(file3, "%lg %lg");
  TGraph *graphr = new TGraph(file2, "%lg %lg");
  std::string title = "SIMULAZIONE" + std::to_string(number);
  char *tit = &title[0];
  TCanvas *c = new TCanvas("Evoluzione", tit);
  c->Divide(3, 1);
  c->cd(1);
  graphs->SetTitle("SUSCETTIBILI");
  graphs->GetXaxis()->SetTitle("Tempo");
  graphs->GetYaxis()->SetTitle("#Persone suscettibili");
  graphs->Draw();
  c->cd(2);
  graphi->SetTitle("INFETTI");
  graphi->GetXaxis()->SetTitle("Tempo");
  graphi->GetYaxis()->SetTitle("#Persone infette");
  graphi->Draw();
  c->cd(3);
  graphr->SetTitle("RIMOSSI");
  graphr->GetXaxis()->SetTitle("Tempo");
  graphr->GetYaxis()->SetTitle("#Persone rimosse");
  graphr->Draw();
  std::string fname = "pdf_Simulations/Evolution_" + std::to_string(number) + ".pdf";
  char *final = &fname[0];
  c->Print(final);
}
#endif