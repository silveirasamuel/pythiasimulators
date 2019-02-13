//Samuel PP Silveira, Fundação Universidade Federal do ABC, 2018

#include "Pythia8/Pythia.h"
#include "TTree.h"
#include "TFile.h"
#include "TROOT.h"    // to use gROOT point
#include "TObject.h" 
#include "TH2.h"
#include "TBrowser.h"
#include "TRandom.h"
using namespace Pythia8;
void writeTree(){
	Pythia pythia;
        pythia.readString("Top:gg2ttbar = on");
        pythia.init();

        pythia.settings.listChanged();
        pythia.particleData.listChanged();

	TFile file("ggttbartree.root","recreate");
	Double_t pT,eta;
	Int_t id;
	TTree tree("tree","ggttbartree");
	tree.Branch("id",&id);
	tree.Branch("pT",&pT);
	tree.Branch("eta",&eta);

	Hist allpT("All particles transverse momentum", 100.0,0.,200.);
	Hist allEta("All particles pseudorapidity", 100,-5.,5.);

        for (int iEvent = 0; iEvent<3;++iEvent){
                pythia.next();
                int iTopbar = 0;
                for(int i=0;i<pythia.event.size();i++){
			if(pythia.event[i].status()>0){
				id = pythia.event[i].id();
				pT = pythia.event[i].pT();
				eta = pythia.event[i].eta();
				tree.Fill();
			}
                }

                pythia.event.list();
        }

	tree.Print();
	tree.Scan();
	tree.Write();
        pythia.stat();       
}

void readTree(){
	TFile *f = new TFile("ggttbartree.root");
	TTree *tree = (TTree*)f->Get("tree");
	Double_t eta1, pT1;
	tree->SetBranchAddress("pT",&pT1);
	tree->SetBranchAddress("eta",&eta1);
	
	//creating the histogram
	
	TH1F *pTx = new TH1F("pTx","pTx distribution", 100,0,200);
	TH1F *etax = new TH1F("etax","etax distribution", 100,-5,5);
	//reading and filling
	cout<<"Got here"<<endl;
	for(Int_t i=0;i<tree->GetEntries();i++){
		tree->GetEntry(i);
		//cout<<pT_<<endl;
		pTx->Fill(pT1);
		etax->Fill(eta1);
	}

}
int main(){
	readTree();
        return 0;

}

