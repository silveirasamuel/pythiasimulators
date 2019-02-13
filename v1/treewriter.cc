//Samuel PP Silveira, Fundação Universidade Federal do ABC, 2018

#include "Pythia8/Pythia.h"
#include "TTree.h"
#include "TFile.h"
#include "TLorentzVector.h"
#include <stack>
using namespace Pythia8;
using namespace std;
class MParticle{
	public:
 		//TLorentzVector *lorvec;
		Double_t px,py,pz,e;
		Int_t id;
		Int_t pdgcode;
		Int_t mother;
		Int_t daughter1;
		Int_t daughter2;
		MParticle(Int_t id, Int_t pdgcode, Int_t mother, Int_t daughter1,Int_t daughter2, Double_t px, Double_t py, Double_t pz, Double_t e){
			this->id = id;
			this->pdgcode = pdgcode;
			this->mother = mother;
			this->daughter1 = daughter1;
			this->daughter2 = daughter2;
			this->px = px;
			this->py = py;
			this->pz = pz;
			this->e = e;
		}
};

class MEvent{
	public:
		int id;
		vector <MParticle> mparticle;
		MEvent(int id, vector<MParticle> mparticle){
			this->id = id;
			this->mparticle = mparticle;
		}
		MEvent(){}
};

void writeTree(string inputFile, int nEvents){
	Pythia pythia;
        pythia.readFile(inputFile);
        pythia.init();

        pythia.settings.listChanged();
        pythia.particleData.listChanged();

	TFile file((inputFile+"tree.root").c_str(),"recreate");
	Double_t px,py,pz,e,m,scale,pol,xProd,yProd,zProd,tProd,tau,pT,eta,charge;
	Int_t pdg,status,mother1,mother2,daughter1,daughter2,col,acol,spinType,id;
	
	TTree tree("tree",(inputFile+"tree").c_str());
	tree.Branch("pdg",&pdg);
	tree.Branch("status",&status);
	tree.Branch("mother1",&mother1);
	tree.Branch("mother2",&mother2);
	tree.Branch("daughter1",&daughter1);
	tree.Branch("daughter2",&daughter2);
	tree.Branch("col",&col);
	tree.Branch("acol",&acol);
	tree.Branch("px",&px);
	tree.Branch("py",&py);
	tree.Branch("pz",&pz);
	tree.Branch("e",&e);
	tree.Branch("m",&m);
	tree.Branch("scale",&scale);
	tree.Branch("pol",&pol);
	tree.Branch("xProd",&xProd);
	tree.Branch("yProd",&yProd);
	tree.Branch("zProd",&zProd);
	tree.Branch("tProd",&tProd);
	tree.Branch("tau",&tau);
	tree.Branch("spinType",&spinType);
	tree.Branch("charge",&charge);
	tree.Branch("pT",&pT);
	tree.Branch("eta",&eta);
	tree.Branch("id",&id);
	Hist allpT("All particles transverse momentum", 100.0,0.,200.);
	Hist allEta("All particles pseudorapidity", 100,-5.,5.);

        for (int iEvent = 0; iEvent<nEvents;++iEvent){
                pythia.next();
                for(int i=0;i<pythia.event.size();i++){
			px = pythia.event[i].px();
			py = pythia.event[i].py();
			pz = pythia.event[i].pz();
			e = pythia.event[i].e();
			m = pythia.event[i].m();
			scale = pythia.event[i].scale();
			pol = pythia.event[i].pol();
			xProd = pythia.event[i].xProd();
			yProd = pythia.event[i].yProd();
			zProd = pythia.event[i].zProd();
			tProd = pythia.event[i].tProd();
			tau = pythia.event[i].tau();
			pT = pythia.event[i].pT();
			eta = pythia.event[i].eta();
			charge = pythia.event[i].charge();
			pdg = pythia.event[i].id();
			status = pythia.event[i].status();
			mother1 = pythia.event[i].mother1();
			mother2 = pythia.event[i].mother2();
			daughter1 = pythia.event[i].daughter1();
			daughter2 = pythia.event[i].daughter2();
			col = pythia.event[i].col();
			acol = pythia.event[i].acol();
			spinType = pythia.event[i].spinType();
			ostringstream oss;
			oss<<iEvent<<i;
			istringstream iss(oss.str());				
			iss>>id;
			
			tree.Fill();
                }

                pythia.event.list();
        }

	tree.Print();
	tree.Scan();
	tree.Write();
        pythia.stat();       
}

void writeTree2(string inputFile, int nEvents){
    

	Pythia pythia;
        pythia.readFile(inputFile);
        pythia.init();

        pythia.settings.listChanged();
        pythia.particleData.listChanged();

	TFile file((inputFile+"tree.root").c_str(),"recreate");


    	MEvent mevent;
	vector <MParticle> mpartl;
    	// Create a ROOT Tree
	TTree tree("tree",(inputFile+"tree").c_str());
    	//tree.Branch("eventn",&eventn,"id/Int_t:pdgcode:mother:daughter1:daughter2:px/Double_t:py:pz:e");
		
	tree.Branch("eventn",&mevent);
 
	for (int iEvent = 0; iEvent<nEvents;++iEvent){
                pythia.next();
                for(int i=0;i<pythia.event.size();i++){
			ostringstream oss;			
			oss<<iEvent<<i;
			istringstream iss(oss.str());				
			Int_t j;
			iss>>j;
			MParticle lpart(j,pythia.event[i].id(),pythia.event[i].mother1(),pythia.event[i].daughter1(),pythia.event[i].daughter2(), pythia.event[i].px(),pythia.event[i].py(),pythia.event[i].pz(),pythia.event[i].e());
			
			mpartl.push_back(lpart);
			/*ostringstream oss;
			oss<<iEvent<<i;
			istringstream iss(oss.str());				
			iss>>particlen.id;
//			cout<< particlen.id<<endl;			
			particlen.pdgcode =  pythia.event[i].id();  
			particlen.mother =  pythia.event[i].mother1();
			particlen.daughter1 = pythia.event[i].daughter1();
			particlen.daughter2 = pythia.event[i].daughter2();
			particlen.px = pythia.event[i].px();
			particlen.py = pythia.event[i].py();
			particlen.pz = pythia.event[i].pz();
			particlen.e = pythia.event[i].e();
			//particlen.lorvec = new TLorentzVector(pythia.event[i].px(),pythia.event[i].py(),pythia.event[i].pz(),pythia.event[i].e());
			*/
		}
		mevent = MEvent(iEvent,mpartl);
		mpartl.clear();
		pythia.event.list();
		
		tree.Fill();
	}
	
    	// End of the loop

	tree.Print();
	tree.Scan();
	file.Write();
	file.Close();
	pythia.stat();  
    	return;
}


int main(int argc, char* argv[]){
	//writeTree(argv[1],strtol(argv[2],NULL,10));
	writeTree2(argv[1],strtol(argv[2],NULL,10));
        return 0;

}

