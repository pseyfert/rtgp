#include "gpcanvas.h"
#include "gphist.h"
#include <iostream>
#include <fstream>
//#include <libintl.h>
#include <TROOT.h>
#include <stdio.h>
#include <stdlib.h>
#include <Rtypes.h>
/**
 * Implementation file for class : gpcanvas
 * 
 * 2010-11-21 : Paul Seyfert
 * 2011-01-31 : Paul Seyfert change terminal to »dumb« output
 */
#include <TStyle.h>

/**
 * Write a macro file from TCanvas
 * and store the contained histogramms
 */
bool gpcanvas::save() {
  bool retval = true;
  if (histogramms.empty()) {
    std::cout << gettext("There is nothing to do.") << std::endl;
    return retval;
  }
  printf(gettext("Creating the macro %s.gp\n"),macroname.c_str());
  std::ofstream macro(Form("%s.gp",macroname.c_str()));
  // do some magic
	// 
//  macro << "set terminal epslatex color" << std::endl;
  macro << "set terminal dumb" << std::endl;
//  macro << "set output \"" << macroname.c_str() << ".tex\"" << std::endl;
  macro << "set xlabel \"" << (*histogramms.begin())->xtitle() << "\"" << std::endl;
  macro << "set ylabel \"" << (*histogramms.begin())->ytitle() << "\"" << std::endl;
  macro << "set boxwidth 0.9 absolute" << std::endl;
  macro << "set style fill solid 1.00 border -1\n";
  macro << "set style histogram errorbars gap 1" << std::endl;
  macro << "set datafile missing '-'" << std::endl;
  macro << "set style data histograms" << std::endl;
  double up,low;
  (*histogramms.begin())->getyrange(low,up);
  if (low!=up)
    macro << "set yrange [" << low << ":" << up << "]\n";
  macro << "unset key" << std::endl;
  macro << "set datafile commentschar \"#\"" << std::endl;

  low = (*histogramms.begin())->thehist->GetBinLowEdge((*histogramms.begin())->thehist->GetXaxis()->GetFirst());
  up = (*histogramms.begin())->thehist->GetXaxis()->GetBinUpEdge((*histogramms.begin())->thehist->GetXaxis()->GetLast());

//  macro << "plot [" << low << ":" << up << "] ";  
  macro << "plot ";
	
  for (std::vector<gphist*>::iterator it = histogramms.begin();
       histogramms.end() != it;
       ++it) {
    macro << "'" << (*it)->filename() << "' using 2:3:xtic(1) title \"" << (*it)->title() << "\"";
    retval = retval && ((*it)->save());
    if (--histogramms.end()!=it)
      macro << ",\\\n";
  }

  macro.close();

  saved = true;

  char buf[256];
	sprintf(buf, "gnuplot %s.gp",macroname.c_str());
	system(buf);
	printf("%s\n",buf);

  return retval;
}

gpcanvas::gpcanvas(TCanvas* rootcanvas) : macroname(rootcanvas->GetName()) {
  rootcanvas->Update();
  TList* list = rootcanvas->GetListOfPrimitives();
  TObjLink* lnk = list->FirstLink();
  TObject* element;
//  std::cout << "===============================\n";
  while (lnk) {
    element=lnk->GetObject();
//    std::cout << element->GetName() << std::endl;
    if (element->InheritsFrom("TH1")) {
      gphist* gpelement = new gphist((TH1*)element);
      histogramms.push_back(gpelement);
    }
//    std::cout << lnk << "\t" << lnk->Next() << "\n";
    lnk = lnk->Next();
  }
//  std::cout << "===============================\n";
  saved = false;
}

gpcanvas::~gpcanvas() {
  std::cout << gettext("Deleting gpcanvas ") << macroname << std::endl;
//  std::cout << "saving it" << std::endl;
  if (!saved) save();

  for (std::vector<gphist*>::iterator it = histogramms.begin();
       histogramms.end() != it;
       ++it) {
    delete *it;
  }
  histogramms.clear();
}

ClassImp(gpcanvas);
