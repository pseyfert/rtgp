#include "gpcanvas.h"
#include "gphist.h"
#include <iostream>
#include <fstream>
//#include <libintl.h>
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
  struct winsize w;
  ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
  unsigned int num_of_rows = w.ws_row;
  unsigned int num_of_cols = w.ws_col;
  printf(gettext("Creating the macro %s.gp\n"),macroname.c_str());
  std::ofstream macro(Form("%s.gp",macroname.c_str()));
  // do some magic
	// 
//  macro << "set terminal epslatex color" << std::endl;
  gp << "set terminal dumb size " << num_of_cols << "," << num_of_rows << std::endl;
//  macro << "set output \"" << macroname.c_str() << ".tex\"" << std::endl;
  gp << "set xlabel \"" << (*histogramms.begin())->xtitle() << "\"" << std::endl;
  gp << "set ylabel \"" << (*histogramms.begin())->ytitle() << "\"" << std::endl;
  gp << "set boxwidth 0.9 absolute" << std::endl;
  gp << "set style fill solid 1.00 border -1\n";
  gp << "set style histogram errorbars gap 1" << std::endl;
  gp << "set datafile missing '-'" << std::endl;
  gp << "set style data histograms" << std::endl;
  double up,low;
  (*histogramms.begin())->getyrange(low,up);
  if (low!=up)
    gp << "set yrange [" << low << ":" << up << "]\n";
  gp << "unset key" << std::endl;
  gp << "set datafile commentschar \"#\"" << std::endl;

  low = (*histogramms.begin())->thehist->GetBinLowEdge((*histogramms.begin())->thehist->GetXaxis()->GetFirst());
  up = (*histogramms.begin())->thehist->GetXaxis()->GetBinUpEdge((*histogramms.begin())->thehist->GetXaxis()->GetLast());

//  macro << "plot [" << low << ":" << up << "] ";  
  macro << "plot ";
	
  for (std::vector<gphist*>::iterator it = histogramms.begin();
       histogramms.end() != it;
       ++it) {
    macro << "'" << (*it)->filename() << "' using 2:3:xtic(1) title \"" << (*it)->title() << "\"";
    retval = retval && ((*it)->save(gp));
    if (--histogramms.end()!=it)
      macro << ",\\\n";
  }

  macro.close();

  saved = true;

  char buf[256];
	sprintf(buf, "gnuplot %s.gp",macroname.c_str());
	//system(buf);
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
