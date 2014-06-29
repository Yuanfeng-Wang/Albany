/*
 * Aeras_Eta.hpp
 *
 *  Created on: May 28, 2014
 *      Author: swbova
 */

#ifndef AERAS_ETA_HPP_
#define AERAS_ETA_HPP_

namespace Aeras {

template<typename EvalT>
class Eta {
public:
  typedef typename EvalT::ScalarT     ScalarT;
  typedef typename EvalT::MeshScalarT MeshScalarT;

  static const Eta<EvalT> &self(const ScalarT ptop=0,
                                const ScalarT p0=0,
                                const int     L=0) {
    static const Eta swc(ptop,p0,L);
    return swc;
  }

  ScalarT   eta(const int level) const { 
    const int L = numLevels - level - 1;
    const ScalarT e = Etatop + (1-Etatop)*(ScalarT(L)+.5)/numLevels; 
    return e;
  }
  ScalarT delta(const int level) const { 
    const int L = numLevels - level - 1;
    const double etap = L + .5;
    const double etam = L - .5;
    const ScalarT DeltaEta = eta(etap) - eta(etam);
    return  DeltaEta;
  }
  ScalarT     p0() const { return P0;  }
  ScalarT   ptop() const { return Ptop;}
  ScalarT etatop() const { return Etatop;}

  ScalarT     W(const int level) const { return  (eta(level)-Etatop)/(1-Etatop); }
  ScalarT     A(const int level) const { return   eta(level)*(1-W(level));       }
  ScalarT     B(const int level) const { return   eta(level)*   W(level);        }

  ScalarT     B(const double half_step) const { return  eta(half_step)*(eta(half_step)-Etatop)/(1-Etatop);}

private:
  const ScalarT P0;
  const ScalarT Ptop;
  const ScalarT Etatop;
  const int     numLevels;

  Eta(const ScalarT ptop, const ScalarT p0, const int L) :
    P0(p0),
    Ptop(ptop),
    Etatop(ptop/p0),
    numLevels(L)
  {}
  ScalarT   eta(const double half_step) const {
    const ScalarT L = numLevels - half_step - 1;
    ScalarT e;
    if      (L         <  -.25) e = Etatop;
    else if (numLevels < L+.75) e = 1;
    else                        e = Etatop + (1-Etatop)*(ScalarT(L)+.5)/numLevels;
    return e;
  }

};
}


#endif /* AERAS_ETA_HPP */