//Samuel PP Silveira, Universidade Federal do ABC, 2018
#include "TROOT.h"
#include "TFile.h"
#include "TTree.h"
#include "TCanvas.h"
#include "TH2.h"
#include "TBrowser.h"
#include <string>
#include <iostream>
using namespace std;
int treereader(string treeName, int pId){
	string name = "Meson code: " + to_string(pId);
	TFile *f = new TFile(treeName.c_str());
	if(f==NULL){
		cout<<"File not found!";
		return 1;
	}
	TTree *tree = (TTree*)f->Get("tree");
	Double_t pT, eta;
	Int_t id;
	tree->SetBranchAddress("pT",&pT);
	tree->SetBranchAddress("eta",&eta);
	tree->SetBranchAddress("id",&id);
	//creating the histogram
	TCanvas *c1= new TCanvas("Distribution histogram", name.c_str(), 1200,600);
	c1->Divide(1,2);
	TH1F *pTx = new TH1F("pTx","pTx distribution", 100,0,200);
	TH1F *etax = new TH1F("etax","etax distribution", 100,-5,5);
	//reading and filling
	for(Int_t i=0;i<tree->GetEntries();i++){
			tree->GetEntry(i);
			if(id==pId){
				pTx->Fill(pT);
				etax->Fill(eta);
			}
	}
	c1->cd(1);
	c1->SetLogy();
	pTx->Draw();
	
	c1->cd(2);
	etax->Draw();
	string dest = "mesonshist/"+to_string(pId)+".png";
	c1->SaveAs(dest.c_str());

	
	return 0;
}

