#ifndef GPHIST_H
#define GPHIST_H

#include <string>
#include <Rtypes.h>
#include <TH1.h>
namespace gnuplotio {class Gnuplot;}
class gpcanvas;


/** @class gphist
 *
 * \brief Data structure for converting CERN ROOT TH1 to ascii files to be read by gnuplot
 * 
 * @author Paul Seyfert
 * @date   2010-11-21
 */


class gphist {
 public:
  gphist() {};
  gphist(TH1* roothist);
  virtual ~gphist();
  bool                 save(gnuplotio::Gnuplot& gp);
  bool                 getyrange(double& low, double& up);
  const char*          xtitle() {return thehist->GetXaxis()->GetTitle();}
  const char*          ytitle() {return thehist->GetYaxis()->GetTitle();}
  const char*          title()  {return thehist->GetTitle();}
  friend class gpcanvas;
 private:
  gpcanvas* parent;
  TH1*                 thehist;
  bool                 saved;
  bool                 errorbars;
#ifndef _LIBINTL_H
  char* gettext(char* s) {return s;}
#endif
 ClassDef(gphist,1);
};

#endif
