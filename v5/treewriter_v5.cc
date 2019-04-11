//Samuel PP Silveira, Fundação Universidade Federal do ABC, 2019

#include "Pythia8/Pythia.h"
#include "TTree.h"
#include "TFile.h"
#include "TLorentzVector.h"
#include "TClonesArray.h"
#include "MParticle.h"
#include "TROOT.h"
#include <sys/stat.h>
#include <ctime>
#include <sstream>
#include <cstdio>
using namespace Pythia8;
using namespace std;
#define CHARM 4
#define STRANGE 3
#define WBOSON 24
stringstream buildDir(){
    /*this block just set the output configurations, such as directory and name*/

    mkdir("OUTPUT", S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
    /*getting the localtime*/
    time_t theTime = time(NULL);
    struct tm *aTime = localtime(&theTime);
    stringstream date;
    int day = aTime->tm_mday;
    int month = aTime->tm_mon + 1; // Month is 0 – 11, add 1 to get a jan-dec 1-12 concept
    int year = aTime->tm_year + 1900; // Year is # years since 1900
    int hour = aTime->tm_hour;
    int min = aTime->tm_min;
    int sec = aTime->tm_sec;
    
    /*creating directory OUTPUT/year_month/day_hourminsec */
    date<<"OUTPUT/"<<year<<"_"<<month;
    mkdir((date.str()).c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
    date<<"/"<<day<<"_"<<hour<<min<<sec<<"/";
    mkdir((date.str()).c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);

    return date;
}

void writeTree(string inputFile, int nEvents){
    
    stringstream date = buildDir();

    /*Beginning the simulation*/
    Pythia pythia;
    pythia.readFile(inputFile);
    pythia.init();
    pythia.settings.listChanged();
    pythia.particleData.listChanged();

    /*Loading the classes, this is extremely important, otherwise everything is gonna crash*/
    gROOT->ProcessLine(".L MParticle.h");
    
    TFile file((inputFile+"tree.root").c_str(),"recreate");
    
    /*Instanciating an array for events and linking each event with a particle list*/
    TClonesArray* particlesArray = new TClonesArray("MParticle");
    TTree tree("tree",(inputFile+"tree").c_str());

    Int_t iEvent;
    Int_t nParticlesEvent;
    /*Adding the Event and Particle branches*/
    tree.Branch("iEvent",&iEvent,"iEvent/I");
    tree.Branch("nParticlesEvent",&nParticlesEvent,"nParticlesEvent/I");
    tree.Branch("particlesArray",particlesArray);

    int fNparticles;
    for (int eventI = 0; eventI<nEvents;++eventI){ /*iterating each event*/
        pythia.next();
        particlesArray->Clear();
        iEvent = eventI;
        nParticlesEvent = pythia.event.size();
        fNparticles = 0;
        for(Int_t i=0;i<pythia.event.size();++i){/*iterating each particle*/
           if(pythia.event[i].isFinal()|| pythia.event[i].idAbs() == STRANGE || pythia.event[i].idAbs() == CHARM){                
                    MParticle *mypart = (MParticle*)(particlesArray->New(fNparticles++));
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
                    mypart->isFinal = pythia.event[i].isFinal();
                    continue;
            }
            if (pythia.event[i].idAbs() == WBOSON){
                if(pythia.event[pythia.event[i].daughter1()].idAbs() == CHARM || pythia.event[pythia.event[i].daughter1()].idAbs() == STRANGE){
                    if(pythia.event[pythia.event[i].daughter2()].idAbs() == CHARM||pythia.event[pythia.event[i].daughter2()].idAbs() == STRANGE){
                        MParticle *mypart = (MParticle*)(particlesArray->New(fNparticles++));
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
                        mypart->isFinal = pythia.event[i].isFinal();
                        continue;
                    }
                }
            }
            
        }
        
        tree.Fill();
    }
    
    tree.Write();
    tree.Scan();
    tree.Print();
    /*moving to the right directory*/
    date<<inputFile<<"tree.root";
    rename((inputFile+"tree.root").c_str(), date.str().c_str());    
}

int main(int argc, char* argv[]){
    writeTree(argv[1],strtol(argv[2],NULL,10));
    return 0;
}

