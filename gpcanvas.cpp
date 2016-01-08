#include "gpcanvas.h"
#include "gphist.h"
#include <iostream>
#include <fstream>
//#include <libintl.h>
#include "THistPainter.h"
#include "Hoption.h"
#include <TROOT.h>
#include <stdio.h>
#include <stdlib.h>
#include <Rtypes.h>
#include <sys/ioctl.h>
#include <unistd.h>

/**
 * Implementation file for class : gpcanvas
 * 
 * 2010-11-21 : Paul Seyfert
 * 2011-01-31 : Paul Seyfert change terminal to »dumb« output
 */
#include <TStyle.h>

#include "gnuplot-iostream/gnuplot-iostream.h"

/**
 * Write a macro file from TCanvas
 * and store the contained histogramms
 */
bool gpcanvas::save() {
  Gnuplot gp;
  bool retval = true;
  if (histogramms.empty()) {
    std::cout << gettext("There is nothing to do.") << std::endl;
    return retval;
  }
  //http://stackoverflow.com/questions/23369503/get-size-of-terminal-window-rows-columns
  struct winsize w;
  ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
  unsigned int num_of_rows = w.ws_row;
  unsigned int num_of_cols = w.ws_col;
  // do some magic
	// 
//  macro << "set terminal epslatex color" << std::endl;
  gp << "set terminal dumb size " << num_of_cols << "," << num_of_rows << std::endl;
  gp << "set xlabel \"" << (*histogramms.begin())->xtitle() << "\"" << std::endl;
  gp << "set ylabel \"" << (*histogramms.begin())->ytitle() << "\"" << std::endl;
  gp << "set style fill solid 1.00 border -1\n";
  double up,low;
  (*histogramms.begin())->getyrange(low,up);
  if (low!=up)
    gp << "set yrange [" << low << ":" << up << "]\n";
  gp << "unset key" << std::endl;

  low = (*histogramms.begin())->thehist->GetBinLowEdge((*histogramms.begin())->thehist->GetXaxis()->GetFirst());
  up = (*histogramms.begin())->thehist->GetXaxis()->GetBinUpEdge((*histogramms.begin())->thehist->GetXaxis()->GetLast());

  gp << "plot";
  for (auto it : histogramms) {
    if (it->errorbars) {
      gp << " '-' using 1:2:3 with boxerrorbars title \"" << it->title() << "\"";
    } else {
      gp << " '-' using 1:2 with boxes title \"" << it->title() << "\"";
    }
    if (histogramms.back()!=it) {
      gp << ", ";
    } else {
      gp << "\n";
    }
  }
  for (auto it : histogramms)
    it->save(gp);

  saved = true;

  char buf[256];
	printf("%s\n",buf);

  return retval;
}

gpcanvas::gpcanvas(TCanvas* rootcanvas) {
  rootcanvas->Update();
  TList* list = rootcanvas->GetListOfPrimitives();
  TObjLink* lnk = list->FirstLink();
  TObject* element;
//  std::cout << "===============================\n";
  while (lnk) {
    element=lnk->GetObject();
    Option_t* option = lnk->GetOption();
    std::cout << " element " << element->GetName() << " has options " << option << std::endl;
//    std::cout << element->GetName() << std::endl;
    if (element->InheritsFrom("TH1")) {
      gphist* gpelement = new gphist((TH1*)element);
      THistPainter* painter = static_cast<THistPainter*>(static_cast<TH1*>(element)->GetPainter());
      Hoption_t* hoption = painter->parseresult(option);
      gpelement->parent = this;
      std::cout << hoption->Error << std::endl;
      gpelement->errorbars = hoption->Error;
      histogramms.push_back(gpelement);
    }
//    std::cout << lnk << "\t" << lnk->Next() << "\n";
    lnk = lnk->Next();
  }
//  std::cout << "===============================\n";
  saved = false;
}

gpcanvas::~gpcanvas() {
  if (!saved) save();

  for (std::vector<gphist*>::iterator it = histogramms.begin();
       histogramms.end() != it;
       ++it) {
    delete *it;
  }
  histogramms.clear();
}

ClassImp(gpcanvas);
