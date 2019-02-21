//Samuel PP Silveira, Fundação Universidade Federal do ABC, 2018

#include "Pythia8/Pythia.h"
#include "TTree.h"
#include "TFile.h"
#include "TLorentzVector.h"
#include <vector>
#include "TClonesArray.h"
#include "MEvent.h"
#include "MParticle.h"
#include "TROOT.h"
#include <sys/stat.h>
#include <ctime>
#include <sstream>
#include <cstdio>
using namespace Pythia8;
using namespace std;

void writeTree(string inputFile, int nEvents){
	mkdir("OUTPUT", S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
	time_t theTime = time(NULL);
	struct tm *aTime = localtime(&theTime);
	stringstream date;
	int day = aTime->tm_mday;
	int month = aTime->tm_mon + 1; // Month is 0 – 11, add 1 to get a jan-dec 1-12 concept
	int year = aTime->tm_year + 1900; // Year is # years since 1900
	int hour = aTime->tm_hour;
	int min = aTime->tm_min;
	date<<"OUTPUT/"<<year<<"_"<<month;
	mkdir((date.str()).c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
	date<<"/"<<hour<<"_"<<min<<"/";
	mkdir((date.str()).c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
	

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
    tree.Branch("particles",particleArray);
    for (int iEvent = 0; iEvent<nEvents;++iEvent){
        
        pythia.next();
        fEventHeader->id = (iEvent);
        fEventHeader->nParticles = (pythia.event.size());
        particleArray->Clear();
        int ipart=0;
        for(Int_t i=0;i<pythia.event.size();i++){
            if(pythia.event[i].id() == -24){
            	if(pythia.event[pythia.event[i].mother1()].id()){
            		if(pythia.event[pythia.event[i].daughter1()].id() == 3 || pythia.event[pythia.event[i].daughter1()].id() == -4){
            			if(pythia.event[pythia.event[i].daughter2()].id() == 3 || pythia.event[pythia.event[i].daughter2()].id() == -4){
            		
				            MParticle *mypart = (MParticle*)(particleArray->New(ipart++));
				            mypart->id = i;
				            mypart->pdgcode = pythia.event[i].id();
				            mypart->px = pythia.event[i].px(); 
				            mypart->py = (pythia.event[i].py());
				            mypart->pz = (pythia.event[i].pz());
				            mypart->e = (pythia.event[i].e());
				            mypart->mass = (pythia.event[i].m());
				            mypart->mother1 = (pythia.event[i].mother1());
				            mypart->mother2 = (pythia.event[i].mother2());
				            mypart->daughter1 = (pythia.event[i].daughter1());
				            mypart->daughter2 = (pythia.event[i].daughter2());
            			}
            		}
            	}
           	}
        }

        tree.Fill();
        pythia.event.list();
    }
    

    tree.Print();
    tree.Scan();
    tree.Write();
    pythia.stat();  
    date<<inputFile<<"tree.root";
    rename((inputFile+"tree.root").c_str(), date.str().c_str());     
}

int main(int argc, char* argv[]){
    writeTree(argv[1],strtol(argv[2],NULL,10));
    return 0;
}

