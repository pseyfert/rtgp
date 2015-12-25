#ifndef GPHIST_H
#define GPHIST_H

#include <string>
#include <Rtypes.h>
#include <TH1.h>
namespace gnuplotio {class Gnuplot;}


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
  std::string          filename() {return dataname;}
  bool                 getyrange(double& low, double& up);
  const char*          xtitle() {return thehist->GetXaxis()->GetTitle();}
  const char*          ytitle() {return thehist->GetYaxis()->GetTitle();}
  const char*          title()  {return thehist->GetTitle();}
  friend class gpcanvas;
 private:
  TH1*                 thehist;
  std::string          dataname;
  bool                 saved;
#ifndef _LIBINTL_H
  char* gettext(char* s) {return s;}
#endif
 ClassDef(gphist,1);
};

#endif
