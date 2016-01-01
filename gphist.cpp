#include "gpcanvas.h"
#include "gphist.h"
#include <iostream>
//#include <libintl.h>
#include <TROOT.h>
#include <stdio.h>
#include <stdlib.h>
#include <Rtypes.h>
#include <tuple>
#include <vector>
#include "gnuplot-iostream/gnuplot-iostream.h"
/**
 * Implementation file for class : gpcanvas
 * 
 * 2010-11-21 : Paul Seyfert
 * 2011-01-31 : Paul Seyfert change terminal to »dumb« output
 */
#include <TStyle.h>

bool gphist::save(Gnuplot& gp) {
  bool retval = true;
  if (saved) {
    std::cout << gettext("There is nothing to do.") << std::endl;
    return retval;
  }
  // do some magic
  //data << "# " << gettext("There are ") << thehist->GetXaxis()->GetNbins() << gettext(" data points to be written.") << std::endl;
  std::cout << "plotting  with errorbars: " << errorbars << std::endl;

  if (!errorbars) {
    std::vector<std::tuple<float,float> > thedata;
    for  (int i = thehist->GetXaxis()->GetFirst() ; i <= thehist->GetXaxis()->GetLast() ; ++i) {
      thedata.push_back(std::make_tuple(thehist->GetBinCenter(i),thehist->GetBinContent(i)));
    }
    gp.send1d(thedata);
  } else {
    std::vector<std::tuple<float,float,float> > thedata;
    for  (int i = thehist->GetXaxis()->GetFirst() ; i <= thehist->GetXaxis()->GetLast() ; ++i) {
      if (errorbars) {
        thedata.push_back(std::make_tuple(thehist->GetBinCenter(i),thehist->GetBinContent(i),thehist->GetBinError(i)));
      } else {
        thedata.push_back(std::make_tuple(thehist->GetBinCenter(i),thehist->GetBinContent(i),0.f));
      }
    }
    gp.send1d(thedata);
  }

  saved = true;
  return retval;  
}

/// taken from original CERN ROOT implementation
bool gphist::getyrange(double& low, double& up) {
  low = thehist->GetMinimum();
  up = thehist->GetMaximum();
  double lowe = low;
  double upe = up;
  for (int i = 1 ; i <= thehist->GetXaxis()->GetNbins() ; ++i) {
    if (thehist->GetBinContent(i)+thehist->GetBinError(i) > upe)
      upe = thehist->GetBinContent(i)+thehist->GetBinError(i);
    if (thehist->GetBinContent(i)-thehist->GetBinError(i) < lowe)
      lowe = thehist->GetBinContent(i)-thehist->GetBinError(i);
  }
  if (-1111!=thehist->GetMinimumStored() && -1111!=thehist->GetMaximumStored())
    return true;
  if (-1111==thehist->GetMinimumStored())
    low = lowe - gStyle->GetHistTopMargin()*(upe-lowe);
  if (-1111==thehist->GetMaximumStored())
    up = upe + gStyle->GetHistTopMargin()*(upe-lowe);
  if (lowe>0) lowe = 0;
  return true;
}

gphist::~gphist() {
  if (!saved) {
    Gnuplot gp;
    save(gp);
  }
}

gphist::gphist(TH1* roothist) : errorbars(false) {
  if (roothist->InheritsFrom("TH2") || roothist->InheritsFrom("TH3") || roothist->InheritsFrom("TProfile"))
    std::cout << gettext("These classes have not been completely implemented yet. Expect unexpected results.") << std::endl;
  thehist = roothist;
  saved = false;
}

ClassImp(gphist);
