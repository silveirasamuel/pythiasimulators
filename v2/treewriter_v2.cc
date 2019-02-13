//Samuel PP Silveira, Fundação Universidade Federal do ABC, 2018

#include "Pythia8/Pythia.h"
#include "TTree.h"
#include "TFile.h"
#include "TLorentzVector.h"
#include <stack>
#include "TClonesArray.h"
#include "MEvent.h"
#include "MParticle.h"
#include "TROOT.h"
using namespace Pythia8;
using namespace std;

void writeTree(string inputFile, int nEvents){
	Pythia pythia;
    pythia.readFile(inputFile);
    pythia.init();
		
    pythia.settings.listChanged();
    pythia.particleData.listChanged();
    gROOT->ProcessLine(".L MParticle.h");
    gROOT->ProcessLine(".L MEvent.h");
	TFile file((inputFile+"tree.root").c_str(),"recreate");
	
    if (TClass::GetClass("MEvent")) 
    	TClass::GetClass("MEvent")->IgnoreTObjectStreamer();
    MEvent *fEventHeader = new MEvent();
    if (TClass::GetClass("MParticle"))
        TClass::GetClass("MParticle")->IgnoreTObjectStreamer();
    TClonesArray *particleArray = new TClonesArray("MParticle");
	TTree tree("tree",(inputFile+"tree").c_str());
    tree.Branch("eventHeader",fEventHeader);

    for (int iEvent = 0; iEvent<nEvents;++iEvent){
		
        pythia.next();
        /*fEventHeader->setId(iEvent);
        fEventHeader->setNParticles(pythia.event.size());*/
        fEventHeader->id = (iEvent);
        fEventHeader->nParticles = (pythia.event.size());
        for(Int_t i=0;i<pythia.event.size();i++){
            int ipart=0;
            MParticle *mypart = static_cast<MParticle*>(particleArray->New(ipart++));
            /*mypart->setId(i);
            mypart->setPx(pythia.event[i].px()); 
            mypart->setPy(pythia.event[i].py());
            mypart->setPz(pythia.event[i].pz());
            mypart->setE(pythia.event[i].e());
            mypart->setMass(pythia.event[i].m());
            mypart->setMother1(pythia.event[i].mother1());
            mypart->setMother2(pythia.event[i].mother2());
            mypart->setDaughter1(pythia.event[i].daughter1());
            mypart->setDaughter2(pythia.event[i].daughter2());*/
            mypart->id = i;
            mypart->px = pythia.event[i].px(); 
            mypart->py = (pythia.event[i].py());
            mypart->pz = (pythia.event[i].pz());
            mypart->e = (pythia.event[i].e());
            mypart->mass = (pythia.event[i].m());
            mypart->mother1 = (pythia.event[i].mother1());
            mypart->mother2 = (pythia.event[i].mother2());
            mypart->daughter1 = (pythia.event[i].daughter1());
            mypart->daughter2 = (pythia.event[i].daughter2());
            tree.Fill();
        }
	
        //pythia.event.list();
        particleArray->Clear();
    }
	

	tree.Print();
	tree.Scan();
	tree.Write();
    //pythia.stat();       
}

int main(int argc, char* argv[]){
	writeTree(argv[1],strtol(argv[2],NULL,10));
    return 0;
}

