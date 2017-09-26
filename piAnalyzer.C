#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <map>
#include "TString.h"
#include "TFile.h"
#include "TTree.h"
#include "TH1F.h"
#include "TEfficiency.h"

using namespace std;

void piAnalyzer(TString inilename="pipH.ginuke.root")
{
  TString weightfilename = inilename.Copy();
  TString outfilename = inilename.Copy();
  outfilename.ReplaceAll("ginuke","hists");
  cout << "Input file: " << inilename << endl;
  cout << "Output file: " << outfilename << endl;

  TFile * inile = new TFile(inilename);
  TTree * tree = (TTree *) inile->Get("ginuke");
  //tree->Print();
 
  TFile * outfile = new TFile(outfilename,"RECREATE");

  //////////////////////////////////////////////

  Int_t probe, tgt;

  tree->SetBranchAddress("probe",&probe);
  tree->SetBranchAddress("tgt",&tgt);

  Double_t ke, e, p;
  
  tree->SetBranchAddress("ke",&ke);
  tree->SetBranchAddress("e",&e);
  tree->SetBranchAddress("p",&p);

  Int_t np, nn, npip, npim, npi0;

  tree->SetBranchAddress("np",&np);
  tree->SetBranchAddress("nn",&nn);
  tree->SetBranchAddress("npip",&npip);
  tree->SetBranchAddress("npim",&npim);
  tree->SetBranchAddress("npi0",&npi0);

  Int_t nh;

  tree->SetBranchAddress("nh",&nh);

  Int_t pdgh[250];

  tree->SetBranchAddress("pdgh",&pdgh);

  Double_t Eh[250];
  Double_t pxh[250];
  Double_t pyh[250];
  Double_t pzh[250];

  tree->SetBranchAddress("Eh",&Eh);
  tree->SetBranchAddress("pxh",&pxh);
  tree->SetBranchAddress("pyh",&pyh);
  tree->SetBranchAddress("pzh",&pzh);


  //////////////////////////////////////////////

  TH1::SetDefaultSumw2();

  vector<string> cases;

  cases.push_back("all");
  cases.push_back("collision");
  cases.push_back("elastic");
  cases.push_back("inelastic");
  cases.push_back("single_qx");
  cases.push_back("double_qx");
  cases.push_back("absorption");
  cases.push_back("other");

  map<string,TH1F*> hists_p;
  map<string,TEfficiency*> effs_p;
  for (auto &c: cases)
  {
    string name("p_");
    name.append(c);
    TH1F* tmp = new TH1F(name.c_str(),"",50,0.01,5.01);
    hists_p[c] = tmp;

    name = string("p_efftocollision_");
    name.append(c);
    TEfficiency* tmpEff = new TEfficiency(name.c_str(),"",50,0.01,5.01);
    effs_p[c] = tmpEff;
  }

  //////////////////////////////////////////////

  Int_t npents = tree->GetEntries();
  for(Int_t ipent=0; ipent < npents; ipent++)
  {
    tree->GetEntry(ipent);
    hists_p["all"]->Fill(p);
    if (nh == 1 && Eh[0] == e)
    {
      continue;
    }
    hists_p["collision"]->Fill(p);

    //cout << "###########################################" << endl;
    //cout << "pent: " << ipent << endl;
    //cout << "ke: " << ke << endl;
    //cout << "e: " << e << endl;
    //cout << "nh: " << nh << endl;

    //for (Int_t ih=0; ih < nh; ih++)
    //{
    //  cout << "  ih:   " << ih << endl;
    //  cout << "  pdgh: " << pdgh[ih] << endl;
    //  cout << "  Eh:   " << Eh[ih] << endl;
    //  cout << "  pxh:  " << pxh[ih] << endl;
    //  cout << "  pyh:  " << pyh[ih] << endl;
    //  cout << "  pzh:  " << pzh[ih] << endl;
    //}

    bool iselastic = false;
    bool isinelastic = false;
    bool issingle_qx = false;
    bool isdouble_qx = false;
    bool isabsorption = false;
    bool isother = false;

    if (nh == 1 && pdgh[0]==probe) // elastic
    {
      iselastic = true;
      hists_p["elastic"]->Fill(p);
    }
    else // inelastic
    {
      isinelastic = true;
      hists_p["inelastic"]->Fill(p);
      if (probe == 211) // pip
      {
        if (npip == 0 && npim == 0 && npi0 == 0)
        {
          isabsorption = true;
          hists_p["absorption"]->Fill(p);
        }
        else if (npip == 0 && npim == 0 && npi0 == 1)
        {
          issingle_qx = true;
          hists_p["single_qx"]->Fill(p);
        }
        else if (npip == 0 && npim == 1 && npi0 == 0)
        {
          isdouble_qx = true;
          hists_p["double_qx"]->Fill(p);
        }
        else
        {
          isother = true;
          hists_p["other"]->Fill(p);
        }
      }
      else if (probe == -211) // pim
      {
        if (npip == 0 && npim == 0 && npi0 == 0)
        {
          isabsorption = true;
          hists_p["absorption"]->Fill(p);
        }
        else if (npip == 0 && npim == 0 && npi0 == 1)
        {
          issingle_qx = true;
          hists_p["single_qx"]->Fill(p);
        }
        else if (npip == 1 && npim == 0 && npi0 == 0)
        {
          isdouble_qx = true;
          hists_p["double_qx"]->Fill(p);
        }
        else
        {
          isother = true;
          hists_p["other"]->Fill(p);
        }
      }
    } // else

    effs_p["collision"]->Fill(true,p);
    effs_p["elastic"]->Fill(iselastic,p);
    effs_p["inelastic"]->Fill(isinelastic,p);
    effs_p["absorption"]->Fill(isabsorption,p);
    effs_p["single_qx"]->Fill(issingle_qx,p);
    effs_p["double_qx"]->Fill(isdouble_qx,p);
    effs_p["other"]->Fill(isother,p);

  }

  //////////////////////////////////////////////

  inile->Close();
  
  outfile->cd();
  for (auto &c: cases)
  {
    hists_p[c]->Write();
    effs_p[c]->Write();
  }
  outfile->Close();
}
