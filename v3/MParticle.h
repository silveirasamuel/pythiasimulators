#ifndef M_PARTICLE_H
#define M_PARTICLE_H

class MParticle : public TObject {
	public:
		Double_t px,py,pz,e,mass;
		Int_t id;
		Int_t pdgcode;
		Int_t mother1;
        Int_t mother2;
		Int_t daughter1;
		Int_t daughter2;
	/*public:
		void setPx(Double_t px);
		void setPy(Double_t py);
		void setPz(Double_t pz);
		void setE(Double_t e);
		void setMass(Double_t mass);
		void setId(Int_t id);
		void setPdgcode(Int_t pdgcode);
		void setMother1(Int_t mother1);
		void setMother2(Int_t mother2);
		void setDaughter1(Int_t daughter1);
		void setDaughter2(Int_t daughter2);*/
				
};
#endif
