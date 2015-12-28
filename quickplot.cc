#include "TFile.h"
#include "gpcanvas.h"
#include "gphist.h"
#include "TH1.h"
#include "TCanvas.h"
#include "TROOT.h"
#include <string>

int main(int argc, char** argv) {
  if (argc!=2) return 22;
  gROOT->SetBatch(kTRUE);
  std::string fullpath(argv[1]);
  std::string filename;
  std::string delimiter = ":";

  size_t pos = 0;
  std::string token;
  pos = fullpath.find(delimiter);
  if (pos==std::string::npos) return 123;
  filename = fullpath.substr(0, pos);
  fullpath.erase(0, pos + delimiter.length());
  std::string histname = fullpath;

  TFile* inf = TFile::Open(filename.c_str());
  if (NULL==inf) return 333;
  if (inf->IsZombie()) return 444;

  TObject* obj = inf->Get(histname.c_str());
  if (NULL==obj) return 333;
  if (!obj->InheritsFrom("TH1")) return 88;

  TCanvas* c = new TCanvas();
  TH1* hist = static_cast<TH1*>(obj);

  if (NULL==hist) return 99;

  hist->Draw();

  gpcanvas gp(c);
  return (int)gp.save();
}

