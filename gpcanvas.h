#ifndef GPCAN
#define GPCAN
#include <string>
#include <vector>
#include <TCanvas.h>
#include "gphist.h"


//#include <Rtypes.h>

/** @class gpcanvas
 *
 * \brief Data structure for converting CERN ROOT TCanvases to gnuplot macros
 * 
 * @author Paul Seyfert
 * @date   2010-11-21
 *
 * \brief  Call gnuplot from inside the code and change default terminal to »dumb«
 *
 * @author Paul Seyfert
 * @date   2011-01-31
 */


class gpcanvas {
 public:
  bool   with_errorbars;
  gpcanvas() : with_errorbars(false) {saved = false;}
  gpcanvas(TCanvas* rootcanvas);
  virtual ~gpcanvas();
  bool                 save();
 private:
  std::vector<gphist*>  histogramms;
  bool                 saved;
#ifndef _LIBINTL_H
	char* gettext(char* s) {return s;}
#endif
 ClassDef(gpcanvas,1);
};

#endif
