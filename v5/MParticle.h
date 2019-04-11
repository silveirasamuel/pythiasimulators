#ifndef M_PARTICLE_H
#define M_PARTICLE_H
#include "TObject.h"
class MParticle : public TObject {
	public:
		Double_t px;
		Double_t py;
		Double_t pz;
		Double_t e;
		Double_t mass;
		Int_t id;
		Int_t pdgcode;
		Int_t mother1;
		Int_t mother2;
		Int_t daughter1;
		Int_t daughter2;
		Bool_t isFinal;
		ClassDef(MParticle,1);
};
#endif
