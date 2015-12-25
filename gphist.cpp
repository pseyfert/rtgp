#include "gpcanvas.h"
#include "gphist.h"
#include <iostream>
#include <fstream>
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
  std::cout << gettext("Creating the data ") << dataname << "" << std::endl;
  std::ofstream data(dataname.c_str());
  // do some magic
  data << "# " << gettext("There are ") << thehist->GetXaxis()->GetNbins() << gettext(" data points to be written.") << std::endl;
  std::vector<std::tuple<float,float,float> > thedata;
  //  for (int i = 1 ; i <= thehist->GetXaxis()->GetNbins() ; ++i) {
  for  (int i = thehist->GetXaxis()->GetFirst() ; i <= thehist->GetXaxis()->GetLast() ; ++i) {
//    std::cout << "." << std::endl;
    data << thehist->GetBinCenter(i) << "\t" << thehist->GetBinContent(i) << "\t" << thehist->GetBinError(i) << std::endl;
    thedata.push_back(std::make_tuple(thehist->GetBinCenter(i),thehist->GetBinContent(i),thehist->GetBinError(i)));
  }
  gp.send1d(thedata);

  data.close();
//  std::cout << gettext("data created") << std::endl;
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
  std::cout << gettext("Deleting gphist ") << dataname << std::endl;
  if (!saved) {
    Gnuplot gp;
    save(gp);
  }
}

gphist::gphist(TH1* roothist) : dataname(roothist->GetName()) {
  std::cout << gettext("opened histogram ") << dataname << std::endl;
  if (roothist->InheritsFrom("TH2") || roothist->InheritsFrom("TH3") || roothist->InheritsFrom("TProfile"))
    std::cout << gettext("These classes have not been completely implemented yet. Expect unexpected results.") << std::endl;
  thehist = roothist;
  saved = false;
}

ClassImp(gphist);
