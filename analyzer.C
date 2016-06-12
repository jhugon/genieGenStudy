#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <map>
#include "TString.h"
#include "TFile.h"
#include "TTree.h"
#include "TH1F.h"

using namespace std;

Float_t SCALEREGIONEMIN=5.0;
Float_t SCALEREGIONEMAX=5.5;

void analyzer(TString infilename="numH.gst.root")
{
  TString weightfilename = infilename.Copy();
  TString outfilename = infilename.Copy();
  weightfilename.ReplaceAll("gst","rewght");
  outfilename.ReplaceAll("gst","hists");
  cout << "Input file: " << infilename << endl;
  cout << "Weight file: " << weightfilename << endl;
  cout << "Output file: " << outfilename << endl;

  TFile * infile = new TFile(infilename);
  TTree * tree = (TTree *) infile->Get("gst");
  tree->AddFriend("weights",weightfilename);
  //tree->Print();
 
  TFile * outfile = new TFile(outfilename,"RECREATE");

  //////////////////////////////////////////////

  Int_t neu, fspl, tgt, Z, A, hitnuc, hitqrk, resid;

  tree->SetBranchAddress("neu",&neu);
  tree->SetBranchAddress("fspl",&fspl);
  tree->SetBranchAddress("tgt",&tgt);
  tree->SetBranchAddress("Z",&Z);
  tree->SetBranchAddress("A",&A);
  tree->SetBranchAddress("hitnuc",&hitnuc);
  tree->SetBranchAddress("hitqrk",&hitqrk);
  tree->SetBranchAddress("resid",&resid);

  Bool_t sea, qel, mec, res, dis, coh, dfr, imd, imdanh, singlek, nuel, em, cc, nc, charm;
  
  tree->SetBranchAddress("sea",&sea);
  tree->SetBranchAddress("qel",&qel);
  tree->SetBranchAddress("mec",&mec);
  tree->SetBranchAddress("res",&res);
  tree->SetBranchAddress("dis",&dis);
  tree->SetBranchAddress("coh",&coh);
  tree->SetBranchAddress("dfr",&dfr);
  tree->SetBranchAddress("imd",&imd);
  tree->SetBranchAddress("imdanh",&imdanh);
  tree->SetBranchAddress("singlek",&singlek);
  tree->SetBranchAddress("nuel",&nuel);
  tree->SetBranchAddress("em",&em);
  tree->SetBranchAddress("cc",&cc);
  tree->SetBranchAddress("nc",&nc);
  tree->SetBranchAddress("charm",&charm);

  Double_t xs, ys, ts, Q2s, Ws, x, y, t, Q2, W, EvRF, Ev, En, El, sumKEf, calresp0; 

  tree->SetBranchAddress("xs",&xs);
  tree->SetBranchAddress("ys",&ys);
  tree->SetBranchAddress("ts",&ts);
  tree->SetBranchAddress("Q2s",&Q2s);
  tree->SetBranchAddress("Ws",&Ws);
  tree->SetBranchAddress("x",&x);
  tree->SetBranchAddress("y",&y);
  tree->SetBranchAddress("t",&t);
  tree->SetBranchAddress("Q2",&Q2);
  tree->SetBranchAddress("W",&W);
  tree->SetBranchAddress("EvRF",&EvRF);
  tree->SetBranchAddress("Ev",&Ev);
  tree->SetBranchAddress("En",&En);
  tree->SetBranchAddress("El",&El);
  tree->SetBranchAddress("sumKEf",&sumKEf);
  tree->SetBranchAddress("calresp0",&calresp0);

  Int_t nfp, nfn, nfpip, nfpim, nfpi0, nfkp, nfkm, nfk0, nfem, nfother;

  tree->SetBranchAddress("nfp",&nfp);
  tree->SetBranchAddress("nfn",&nfn);
  tree->SetBranchAddress("nfpip",&nfpip);
  tree->SetBranchAddress("nfpim",&nfpim);
  tree->SetBranchAddress("nfpi0",&nfpi0);
  tree->SetBranchAddress("nfkp",&nfkp);
  tree->SetBranchAddress("nfkm",&nfkm);
  tree->SetBranchAddress("nfk0",&nfk0);
  tree->SetBranchAddress("nfem",&nfem);
  tree->SetBranchAddress("nfother",&nfother);

  Int_t nip, nin, nipip, nipim, nipi0, nikp, nikm, nik0, niem, niother;

  tree->SetBranchAddress("nip",&nip);
  tree->SetBranchAddress("nin",&nin);
  tree->SetBranchAddress("nipip",&nipip);
  tree->SetBranchAddress("nipim",&nipim);
  tree->SetBranchAddress("nipi0",&nipi0);
  tree->SetBranchAddress("nikp",&nikp);
  tree->SetBranchAddress("nikm",&nikm);
  tree->SetBranchAddress("nik0",&nik0);
  tree->SetBranchAddress("niem",&niem);
  tree->SetBranchAddress("niother",&niother);

  Float_t xsec;
  tree->SetBranchAddress("weights.xsec",&xsec);

  //////////////////////////////////////////////

  TH1::SetDefaultSumw2();

  vector<string> cases;

  cases.push_back("all");
  //cases.push_back("sea");
  cases.push_back("qel");
  //cases.push_back("mec");
  cases.push_back("res");
  cases.push_back("dis");
  cases.push_back("coh");
  cases.push_back("dfr");
  cases.push_back("imd");
  //cases.push_back("imdanh");
  //cases.push_back("singlek");
  cases.push_back("nuel");
  cases.push_back("em");
  cases.push_back("cc");
  cases.push_back("nc");
  cases.push_back("charm");
  cases.push_back("1pi");
  cases.push_back("2pi");
  cases.push_back("3pi");
  cases.push_back("1piinc");
  cases.push_back("1pi0inc");

  map<string,TH1F*> hists_Ev;
  for (auto &c: cases)
  {
    string name("Ev_");
    name.append(c);
    TH1F* tmp = new TH1F(name.c_str(),"",150,0.01,15.01);
    hists_Ev[c] = tmp;
  }

  //////////////////////////////////////////////
  vector<Float_t> xsecNormVals;
  UInt_t xsecNormEvents=0;
  //////////////////////////////////////////////

  UInt_t nEvents = tree->GetEntries();
  for(UInt_t iEvent=0; iEvent < nEvents; iEvent++)
  {
    //cout << "###########################################" << endl;
    //cout << "Event: " << iEvent << endl;
    tree->GetEntry(iEvent);

    //cout << "xsec: " << xsec << endl;
    //cout << "Ev: " << Ev << endl;
    //cout << "cc:  " << cc << endl;
    //cout << "nc:  " << nc << endl;
    //cout << "qel:  " << qel << endl;
    //cout << ":  " << qel << endl;
    //cout << "qel:  " << qel << endl;
    //cout << "qel:  " << qel << endl;

    if (true)
    {
      hists_Ev["all"]->Fill(Ev);
    }
    if (qel && cc)
    {
      hists_Ev["qel"]->Fill(Ev);
    }
    if (res && cc)
    {
      hists_Ev["res"]->Fill(Ev);
    }
    if (dis && cc)
    {
      hists_Ev["dis"]->Fill(Ev);
    }
    if (coh && cc)
    {
      hists_Ev["coh"]->Fill(Ev);
    }
    if (dfr && cc)
    {
      hists_Ev["dfr"]->Fill(Ev);
    }
    if (imd && cc)
    {
      hists_Ev["imd"]->Fill(Ev);
    }
    if (nuel)
    {
      hists_Ev["nuel"]->Fill(Ev);
    }
    if (em && cc)
    {
      hists_Ev["em"]->Fill(Ev);
    }
    if (cc)
    {
      hists_Ev["cc"]->Fill(Ev);
    }
    if (nc)
    {
      hists_Ev["nc"]->Fill(Ev);
    }
    if (charm && cc)
    {
      hists_Ev["charm"]->Fill(Ev);
    }
    if (nfpip+nfpim == 1)
    {
      hists_Ev["1pi"]->Fill(Ev);
    }
    if (nfpip+nfpim == 2)
    {
      hists_Ev["2pi"]->Fill(Ev);
    }
    if (nfpip+nfpim == 3)
    {
      hists_Ev["3pi"]->Fill(Ev);
    }
    if (nfpip+nfpim >= 1)
    {
      hists_Ev["1piinc"]->Fill(Ev);
    }
    if (nfpi0 >= 1)
    {
      hists_Ev["1pi0inc"]->Fill(Ev);
    }

    if (nc && qel && Ev > SCALEREGIONEMIN && Ev < SCALEREGIONEMAX)
    {
      if (xsecNormEvents < 100)
      {
        xsecNormVals.push_back(xsec);
      }
      xsecNormEvents++;
    }

  }

  ////////////////////////////////////////////////
  //// Need to scale histogram to NC QE, 
  //// since it's always there.

  sort(xsecNormVals.begin(),xsecNormVals.end());
  Float_t medianXsec = xsecNormVals[xsecNormVals.size()/2];
  Float_t scaleFactor = medianXsec/xsecNormEvents;
  cout << "Scale Factor: " << scaleFactor << endl;
  for (auto &c: cases)
  {
    hists_Ev[c]->Scale(scaleFactor);
  }

  //////////////////////////////////////////////

  infile->Close();
  
  outfile->cd();
  for (auto &c: cases)
  {
    hists_Ev[c]->Write();
  }
  outfile->Close();
}
