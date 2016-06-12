//____________________________________________________________________________
/*!

\program gtestRewght

\brief   A simple program to illustrate how to use the GENIE event reweighting.

\syntax  gtestRewght -f filename [-n nev]

         where 
         [] is an optional argument
         -f specifies a GENIE event file (GHEP format)
         -n specifies the number of events to process (default: all)

\author  Costas Andreopoulos <costas.andreopoulos \at stfc.ac.uk>
         University of Liverpool & STFC Rutherford Appleton Lab

\created May 19, 2010

\cpright Copyright (c) 2003-2016, GENIE Neutrino MC Generator Collaboration
         For the full text of the license visit http://copyright.genie-mc.org
         or see $GENIE/LICENSE
*/
//____________________________________________________________________________


#include <string>
#include <vector>

#include <TSystem.h>
#include <TFile.h>
#include <TTree.h>

#include "EVGCore/EventRecord.h"
#include "Ntuple/NtpMCFormat.h"
#include "Ntuple/NtpMCTreeHeader.h"
#include "Ntuple/NtpMCEventRecord.h"
#include "Messenger/Messenger.h"
#include "ReWeight/GReWeightI.h"
#include "ReWeight/GSystSet.h"
#include "ReWeight/GReWeight.h"
#include "ReWeight/GReWeightNuXSecCCQE.h"
#include "ReWeight/GReWeightNuXSecCCQEvec.h"
#include "ReWeight/GReWeightNuXSecCCRES.h"
#include "ReWeight/GReWeightNuXSecNCRES.h"
#include "ReWeight/GReWeightNuXSecDIS.h"
#include "ReWeight/GReWeightNuXSecCOH.h"
#include "ReWeight/GReWeightNonResonanceBkg.h"
#include "ReWeight/GReWeightFGM.h"
#include "ReWeight/GReWeightDISNuclMod.h"
#include "ReWeight/GReWeightResonanceDecay.h"
#include "ReWeight/GReWeightFZone.h"
#include "ReWeight/GReWeightINuke.h"
#include "ReWeight/GReWeightAGKY.h"
#include "Utils/CmdLnArgParser.h"

#include "Conventions/Units.h"
#include "libxml/parser.h"
#include "Utils/AppInit.h"
#include "EVGCore/EventRecord.h"
#include "GHEP/GHepStatus.h"
#include "GHEP/GHepParticle.h"
#include "GHEP/GHepUtils.h"
#include "Utils/AppInit.h"
#include "Utils/RunOpt.h"

using std::string;
using std::vector;

using namespace genie;
using namespace genie::rew;

void GetCommandLineArgs (int argc, char ** argv);

int    gOptNEvt;
string gOptInpFilename;

//___________________________________________________________________
int main(int argc, char ** argv)
{
  GetCommandLineArgs (argc, argv);

  // open the ROOT file and get the TTree & its header
  TTree *           tree = 0;
  NtpMCTreeHeader * thdr = 0;

  TFile file(gOptInpFilename.c_str(),"READ");

  tree = dynamic_cast <TTree *>           ( file.Get("gtree")  );
  thdr = dynamic_cast <NtpMCTreeHeader *> ( file.Get("header") );

  if(!tree) return 1;

  LOG("test", pNOTICE) << "Input tree header: " << *thdr;

  int nev = (gOptNEvt > 0) ?
        TMath::Min(gOptNEvt, (int)tree->GetEntries()) :
        (int) tree->GetEntries();

  LOG("test", pNOTICE) << "Will process " << nev << " events";

  //
  // Create a GReWeight object and add to it a set of 
  // weight calculators
  //

  GReWeight rw;

  rw.AdoptWghtCalc( "xsec_ccqe",       new GReWeightNuXSecCCQE      );
  rw.AdoptWghtCalc( "xsec_ccqe_vec",   new GReWeightNuXSecCCQEvec   );
  rw.AdoptWghtCalc( "xsec_ccres",      new GReWeightNuXSecCCRES     );
  rw.AdoptWghtCalc( "xsec_ncres",      new GReWeightNuXSecNCRES     );
  rw.AdoptWghtCalc( "xsec_nonresbkg",  new GReWeightNonResonanceBkg );
  rw.AdoptWghtCalc( "xsec_dis",        new GReWeightNuXSecDIS       );
  rw.AdoptWghtCalc( "xsec_coh",        new GReWeightNuXSecCOH       );
  rw.AdoptWghtCalc( "nuclear_qe",      new GReWeightFGM             );
  rw.AdoptWghtCalc( "nuclear_dis",     new GReWeightDISNuclMod      );
  rw.AdoptWghtCalc( "hadro_res_decay", new GReWeightResonanceDecay  );
  rw.AdoptWghtCalc( "hadro_fzone",     new GReWeightFZone           );
  rw.AdoptWghtCalc( "hadro_intranuke", new GReWeightINuke           );
  rw.AdoptWghtCalc( "hadro_agky",      new GReWeightAGKY            );

  //
  // Create a list of systematic params (more to be found at GSyst.h)
  // set non-default values and re-configure.
  // Weight calculators included above must be able to handle the tweaked params.
  // Each tweaking dial t modifies a physics parameter p as:
  // p_{tweaked} = p_{default} ( 1 + t * dp/p )
  // So setting a tweaking dial to +/-1 modifies a physics quantity
  // by +/- 1sigma.
  // Default fractional errors are defined in GSystUncertainty
  // and can be overriden.
  //

  vector<GSyst_t> syst_dials;

//  // NCEL tweaking parameters:
  syst_dials.push_back(kXSecTwkDial_MaNCEL);            ///< tweak Ma NCEL, affects dsigma(NCEL)/dQ2 both in shape and normalization
//  syst_dials.push_back(kXSecTwkDial_EtaNCEL);           ///< tweak NCEL strange axial form factor eta, affects dsigma(NCEL)/dQ2 both in shape and normalization
//  // CCQE tweaking parameters:
//  syst_dials.push_back(kXSecTwkDial_NormCCQE);          ///< tweak CCQE normalization (energy independent)
//  //syst_dials.push_back(kXSecTwkDial_NormCCQEenu);       ///< tweak CCQE normalization (maintains dependence on neutrino energy)
//  syst_dials.push_back(kXSecTwkDial_MaCCQEshape);       ///< tweak Ma CCQE, affects dsigma(CCQE)/dQ2 in shape only (normalized to constant integral)
syst_dials.push_back(kXSecTwkDial_MaCCQE);            ///< tweak Ma CCQE, affects dsigma(CCQE)/dQ2 both in shape and normalization
//  syst_dials.push_back(kXSecTwkDial_VecFFCCQEshape);    ///< tweak elastic nucleon form factors (BBA/default -> dipole) - shape only effect of dsigma(CCQE)/dQ2
//  // Resonance neutrino-production tweaking parameters:
//  syst_dials.push_back(kXSecTwkDial_NormCCRES);         ///< tweak CCRES normalization
//  syst_dials.push_back(kXSecTwkDial_MaCCRESshape);      ///< tweak Ma CCRES, affects d2sigma(CCRES)/dWdQ2 in shape only (normalized to constant integral)
//  syst_dials.push_back(kXSecTwkDial_MvCCRESshape);      ///< tweak Mv CCRES, affects d2sigma(CCRES)/dWdQ2 in shape only (normalized to constant integral)
syst_dials.push_back(kXSecTwkDial_MaCCRES);           ///< tweak Ma CCRES, affects d2sigma(CCRES)/dWdQ2 both in shape and normalization
//  //syst_dials.push_back(kXSecTwkDial_MvCCRES);           ///< tweak Mv CCRES, affects d2sigma(CCRES)/dWdQ2 both in shape and normalization
//  syst_dials.push_back(kXSecTwkDial_NormNCRES);         ///< tweak NCRES normalization
//  syst_dials.push_back(kXSecTwkDial_MaNCRESshape);      ///< tweak Ma NCRES, affects d2sigma(NCRES)/dWdQ2 in shape only (normalized to constant integral)
//  syst_dials.push_back(kXSecTwkDial_MvNCRESshape);      ///< tweak Mv NCRES, affects d2sigma(NCRES)/dWdQ2 in shape only (normalized to constant integral)
syst_dials.push_back(kXSecTwkDial_MaNCRES);           ///< tweak Ma NCRES, affects d2sigma(NCRES)/dWdQ2 both in shape and normalization
//  //syst_dials.push_back(kXSecTwkDial_MvNCRES);           ///< tweak Mv NCRES, affects d2sigma(NCRES)/dWdQ2 both in shape and normalization
//  // Coherent pion production tweaking parameters:
syst_dials.push_back(kXSecTwkDial_MaCOHpi);           ///< tweak Ma for COH pion production
//  syst_dials.push_back(kXSecTwkDial_R0COHpi);           ///< tweak R0 for COH pion production
//  // Non-resonance background tweaking parameters:
//  syst_dials.push_back(kXSecTwkDial_RvpCC1pi);          ///< tweak the 1pi non-RES bkg in the RES region, for v+p CC
//  syst_dials.push_back(kXSecTwkDial_RvpCC2pi);          ///< tweak the 2pi non-RES bkg in the RES region, for v+p CC
//  syst_dials.push_back(kXSecTwkDial_RvpNC1pi);          ///< tweak the 1pi non-RES bkg in the RES region, for v+p NC
//  syst_dials.push_back(kXSecTwkDial_RvpNC2pi);          ///< tweak the 2pi non-RES bkg in the RES region, for v+p NC
//  syst_dials.push_back(kXSecTwkDial_RvnCC1pi);          ///< tweak the 1pi non-RES bkg in the RES region, for v+n CC
//  syst_dials.push_back(kXSecTwkDial_RvnCC2pi);          ///< tweak the 2pi non-RES bkg in the RES region, for v+n CC
//  syst_dials.push_back(kXSecTwkDial_RvnNC1pi);          ///< tweak the 1pi non-RES bkg in the RES region, for v+n NC
//  syst_dials.push_back(kXSecTwkDial_RvnNC2pi);          ///< tweak the 2pi non-RES bkg in the RES region, for v+n NC
//  syst_dials.push_back(kXSecTwkDial_RvbarpCC1pi);       ///< tweak the 1pi non-RES bkg in the RES region, for vbar+p CC
//  syst_dials.push_back(kXSecTwkDial_RvbarpCC2pi);       ///< tweak the 2pi non-RES bkg in the RES region, for vbar+p CC
//  syst_dials.push_back(kXSecTwkDial_RvbarpNC1pi);       ///< tweak the 1pi non-RES bkg in the RES region, for vbar+p NC
//  syst_dials.push_back(kXSecTwkDial_RvbarpNC2pi);       ///< tweak the 2pi non-RES bkg in the RES region, for vbar+p NC
//  syst_dials.push_back(kXSecTwkDial_RvbarnCC1pi);       ///< tweak the 1pi non-RES bkg in the RES region, for vbar+n CC
//  syst_dials.push_back(kXSecTwkDial_RvbarnCC2pi);       ///< tweak the 2pi non-RES bkg in the RES region, for vbar+n CC
//  syst_dials.push_back(kXSecTwkDial_RvbarnNC1pi);       ///< tweak the 1pi non-RES bkg in the RES region, for vbar+n NC
//  syst_dials.push_back(kXSecTwkDial_RvbarnNC2pi);       ///< tweak the 2pi non-RES bkg in the RES region, for vbar+n NC
//  // DIS tweaking parameters - applied for DIS events with (Q2>Q2o); W>Wo), typically Q2o=1GeV^2, Wo=1.7-2.0GeV
//  syst_dials.push_back(kXSecTwkDial_AhtBY);             ///< tweak the Bodek-Yang model parameter A_{ht} - incl. both shape and normalization effect
//  syst_dials.push_back(kXSecTwkDial_BhtBY);             ///< tweak the Bodek-Yang model parameter B_{ht} - incl. both shape and normalization effect 
//  syst_dials.push_back(kXSecTwkDial_CV1uBY);            ///< tweak the Bodek-Yang model parameter CV1u - incl. both shape and normalization effect 
//  syst_dials.push_back(kXSecTwkDial_CV2uBY);            ///< tweak the Bodek-Yang model parameter CV2u - incl. both shape and normalization effect 
//  //syst_dials.push_back(kXSecTwkDial_AhtBYshape);        ///< tweak the Bodek-Yang model parameter A_{ht} - shape only effect to d2sigma(DIS)/dxdy
//  //syst_dials.push_back(kXSecTwkDial_BhtBYshape);        ///< tweak the Bodek-Yang model parameter B_{ht} - shape only effect to d2sigma(DIS)/dxdy
//  //syst_dials.push_back(kXSecTwkDial_CV1uBYshape);       ///< tweak the Bodek-Yang model parameter CV1u - shape only effect to d2sigma(DIS)/dxdy
//  //syst_dials.push_back(kXSecTwkDial_CV2uBYshape);       ///< tweak the Bodek-Yang model parameter CV2u - shape only effect to d2sigma(DIS)/dxdy
//  syst_dials.push_back(kXSecTwkDial_NormDISCC);         ///< tweak the inclusive DIS CC normalization
//  syst_dials.push_back(kXSecTwkDial_RnubarnuCC);        ///< tweak the ratio of \sigma(\bar\nu CC) / \sigma(\nu CC)
//  syst_dials.push_back(kXSecTwkDial_DISNuclMod);        ///< tweak DIS nuclear modification (shadowing, anti-shadowing, EMC)
//  //
//  syst_dials.push_back(kXSecTwkDial_NC);                ///< 
//
//
//  //
//  // Hadronization (free nucleon target)
//  // 
//
//  syst_dials.push_back(kHadrAGKYTwkDial_xF1pi);         ///< tweak xF distribution for low multiplicity (N + pi) DIS f/s produced by AGKY
//  syst_dials.push_back(kHadrAGKYTwkDial_pT1pi);         ///< tweak pT distribution for low multiplicity (N + pi) DIS f/s produced by AGKY
//
//
//  //
//  // Medium-effects to hadronization
//  // 
//
//  syst_dials.push_back(kHadrNuclTwkDial_FormZone);         ///< tweak formation zone
//
//
//  //
//  // Intranuclear rescattering systematics.
//  // There are 2 sets of parameters:
//  // - parameters that control the total rescattering probability); P(total)
//  // - parameters that control the fraction of each process (`fate')); given a total rescat. prob., P(fate|total)
//  // These parameters are considered separately for pions and nucleons.
//  //
//
syst_dials.push_back(kINukeTwkDial_MFP_pi);      ///< tweak mean free path for pions
syst_dials.push_back(kINukeTwkDial_MFP_N);       ///< tweak mean free path for nucleons
//  syst_dials.push_back(kINukeTwkDial_FrCEx_pi);    ///< tweak charge exchange probability for pions, for given total rescattering probability
//  syst_dials.push_back(kINukeTwkDial_FrElas_pi);   ///< tweak elastic         probability for pions, for given total rescattering probability
//  syst_dials.push_back(kINukeTwkDial_FrInel_pi);   ///< tweak inelastic       probability for pions, for given total rescattering probability
//  syst_dials.push_back(kINukeTwkDial_FrAbs_pi);    ///< tweak absorption      probability for pions, for given total rescattering probability
//  syst_dials.push_back(kINukeTwkDial_FrPiProd_pi); ///< tweak pion production probability for pions, for given total rescattering probability
//  syst_dials.push_back(kINukeTwkDial_FrCEx_N);     ///< tweak charge exchange probability for nucleons, for given total rescattering probability
//  syst_dials.push_back(kINukeTwkDial_FrElas_N);    ///< tweak elastic         probability for nucleons, for given total rescattering probability
//  syst_dials.push_back(kINukeTwkDial_FrInel_N);    ///< tweak inelastic       probability for nucleons, for given total rescattering probability
//  syst_dials.push_back(kINukeTwkDial_FrAbs_N);     ///< tweak absorption      probability for nucleons, for given total rescattering probability
//  syst_dials.push_back(kINukeTwkDial_FrPiProd_N);  ///< tweak pion production probability for nucleons, for given total rescattering probability
//
//  //
//  // Nuclear model
//  // 
//
//  syst_dials.push_back(kSystNucl_CCQEPauliSupViaKF);   ///<
//  syst_dials.push_back(kSystNucl_CCQEMomDistroFGtoSF); ///<
//
//  //
//  // Resonance decays
//  // 
//
//  syst_dials.push_back(kRDcyTwkDial_BR1gamma);        ///< tweak Resonance -> X + gamma branching ratio, eg Delta+(1232) -> p gamma
//  syst_dials.push_back(kRDcyTwkDial_BR1eta);          ///< tweak Resonance -> X + eta   branching ratio, eg N+(1440) -> p eta
//  syst_dials.push_back(kRDcyTwkDial_Theta_Delta2Npi);  ///< distort pi angular distribution in Delta -> N + pi
//
//  ///////////////////////////////////////////////////////////

  GSyst dialPrinter;

  GSystSet & syst = rw.Systematics();

  ///////////////////////////////////////////////////////////
  // Outtree

  TFile * outFile = new TFile("test.root","RECREATE");
  TTree * outTree = new TTree("weights","weights");
  TTree * metadataTree = new TTree("metadata","metadata");


  Float_t xsec = -999.;
  std::vector<Float_t> weightsUp;
  std::vector<Float_t> weightsDown;
  std::vector<std::string> dialNames;

  outTree->Branch("xsec",&xsec,"xsec/F");
  outTree->Branch("weightsUp",&weightsUp);
  outTree->Branch("weightsDown",&weightsDown);
  metadataTree->Branch("dialNames",&dialNames);

  for(const GSyst_t &dial: syst_dials)
  {
    dialNames.push_back(dialPrinter.AsString(dial));
    weightsUp.push_back(0.);
    weightsDown.push_back(0.);
  }

  metadataTree->Fill();
  metadataTree->Write();

  ///////////////////////////////////////////////////////////

  //
  // Event loop
  //

  NtpMCEventRecord * mcrec = 0;
  tree->SetBranchAddress("gmcrec", &mcrec);

  for(int iEvent = 0; iEvent < nev; iEvent++) {
    tree->GetEntry(iEvent);

    EventRecord & event = *(mcrec->event);
    LOG("test", pNOTICE) <<"Event: " << iEvent;

    Float_t wght = -1.;
    for(unsigned iDial = 0; iDial < syst_dials.size(); iDial++)
    {
      const GSyst_t dial = syst_dials[iDial];
      LOG("test", pNOTICE) << "Systematic: " << dial << ": "<<dialPrinter.AsString(dial);
      LOG("test", pNOTICE) << "Setting to 1.0";
      syst.Set(dial, +1.0);
      LOG("test", pNOTICE) << "Reconfiguring";
      rw.Reconfigure();
      wght = rw.CalcWeight(event);
      LOG("test", pNOTICE) << "Overall weight = " << wght;
      weightsUp[iDial] = wght;
      syst.Set(dial, -1.0);
      rw.Reconfigure();
      wght = rw.CalcWeight(event);
      LOG("test", pNOTICE) << "Overall weight = " << wght;
      weightsDown[iDial] = wght;
      syst.Set(dial, 0.0);
    }

    xsec = event.XSec()  / (1E-38 * units::cm2); // convert to 1e-38 cm^2
    LOG("test", pNOTICE) << "Cross Section = " << xsec;
    //int diffxsec = int(event.DiffXXSec());

    outTree->Fill();

    mcrec->Clear();
  }

  outTree->Write();
  outFile->Close();
  file.Close();

  LOG("test", pNOTICE)  << "Done!";
  return 0;
}
//___________________________________________________________________
void GetCommandLineArgs(int argc, char ** argv)
{
  LOG("test", pINFO) << "*** Parsing command line arguments";

  RunOpt::Instance()->ReadFromCommandLine(argc,argv);

  CmdLnArgParser parser(argc,argv);

  // get GENIE event sample
  if( parser.OptionExists('f') ) {  
    LOG("test", pINFO) << "Reading event sample filename";
    gOptInpFilename = parser.ArgAsString('f');
  } else {
    LOG("test", pFATAL) 
      << "Unspecified input filename - Exiting";
    exit(1);
  }

  // number of events:
  if( parser.OptionExists('n') ) {  
    LOG("test", pINFO) << "Reading number of events to analyze";
    gOptNEvt = parser.ArgAsInt('n');
  } else {
    LOG("test", pINFO)
       << "Unspecified number of events to analyze - Use all";
    gOptNEvt = -1;
  }

  utils::app_init::MesgThresholds(RunOpt::Instance()->MesgThresholdFiles());
  GHepRecord::SetPrintLevel(RunOpt::Instance()->EventRecordPrintLevel());

}
//_________________________________________________________________________________
