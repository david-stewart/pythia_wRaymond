#include "Pyth6.h"
#include <iostream>
#include <fstream>
#include "TApplication.h"
#include "TPythia6.h"
#include "TFile.h"
#include "TError.h"
#include "TTree.h"
#include "TClonesArray.h"
#include "TMCParticle.h"
#include "TH1.h"
#include "TF1.h"
#include "TStyle.h"
#include "TLatex.h"
#include "TCanvas.h"
#include "Riostream.h"
#include <cstdlib>
#include <time.h>
#include "TParticlePDG.h"
#include "TDatabasePDG.h"
#include "StThreeVectorF.hh"
#include <iomanip>
#include <iomanip>

#define  NMAX_JETS 50

// see https://root.cern.ch/root/html/tutorials/pythia/pythiaExample.C.html
#include "fastjet/config.h"
#include "fastjet/PseudoJet.hh"
#include "fastjet/ClusterSequence.hh"
#include "fastjet/ClusterSequenceArea.hh"
#include "fastjet/Selector.hh"

/* #include "fastjet/tools/Subtractor.hh" */
#include "fastjet/tools/JetMedianBackgroundEstimator.hh"
using namespace std;
using namespace fastjet;


ClassImp(Pyth6)
Pyth6::~Pyth6() {};

Pyth6::Pyth6(string argc_v) : 
    inp{argc_v}, 
    b_jets { "JtJet", NMAX_JETS} 
{

  TPythia6* pythia = new TPythia6;
  /* ZZPUT inp{"some silly syring"}; */
  
  // set the hard scattering
  /* cout << " a0 " << endl; */
  /* pythia->Pytune(370); */
  /* cout << " a1 " << endl; */
  
  pythia->SetMSTJ(11,5);
  pythia->SetPARJ(1,0.085);
  pythia->SetPARJ(2,0.20);
  pythia->SetPARJ(3,0.92);
  pythia->SetPARJ(4,0.043);
  pythia->SetPARJ(6,1.0);
  pythia->SetPARJ(7,1.0);
  pythia->SetPARJ(11,0.35);
  pythia->SetPARJ(12,0.40);
  pythia->SetPARJ(13,0.54);
  pythia->SetPARJ(21,0.33);
  pythia->SetPARJ(25,0.70);
  pythia->SetPARJ(26,0.135);
  pythia->SetPARJ(41,0.45);
  pythia->SetPARJ(42,1.0);
  pythia->SetPARJ(45,0.86);
  pythia->SetPARJ(46,1.0);
  pythia->SetPARJ(47,1.0);

  pythia->SetCKIN(3, inp.pthat_min);
  pythia->SetCKIN(4, inp.pthat_max);
  pythia->SetMRPY(1, inp.seed);

  // ... and initialise it to run p+p at sqrt(200) GeV in CMS
  pythia->Initialize("cms", "p", "p", 200);

  // Make a tree in that file ...
  TTree* tree = new TTree("jet_tree", "Pythia 6 tree");
  TH1F* h_jets = new TH1F("jets","all detector level jets",45, 0, 45);

  // Make the jet pt histogram

  // ... and register a the cache of pythia on a branch (It's a
  // TClonesArray of TMCParticle objects. )
  TClonesArray* particles = (TClonesArray*)pythia->GetListOfParticles();

  tree->Branch("event", &mEvent);
  tree->Branch("jets",  &b_jets);

  // Now we make some events


  //initialize fastjet
  float ghost_maxrap = 4.0;
  double jet_R_background{0.4};
  JetDefinition jet_def_bkgd(kt_algorithm, jet_R_background); // <--
  AreaDefinition area_def_bkgd(active_area_explicit_ghosts,GhostedAreaSpec (ghost_maxrap,1,0.01));
  Selector selector_rm2 = SelectorAbsEtaMax(1.0) * (!SelectorNHardest(2)); // <--
  JetMedianBackgroundEstimator* bge_rm2 = new JetMedianBackgroundEstimator (selector_rm2, jet_def_bkgd, area_def_bkgd); // <--

  JetDefinition jet_def(antikt_algorithm, 0.4);
  AreaDefinition area_def(active_area_explicit_ghosts,GhostedAreaSpec(4.0,1,0.01));

  cout << "starting pythia loop " << endl;
  cout << "MSUB(96) " << pythia->GetMSUB(96) << endl;
  cout << "BRAT(96) " << pythia->GetBRAT(96) << endl;
  cout << "CKIN(96) " << pythia->GetCKIN(96) << endl;
  cout << "RMSS(96) " << pythia->GetRMSS(96) << endl;
  cout << "RRPY(96) " << pythia->GetRRPY(96) << endl;
  cout << "SMW(96) " << pythia->GetSMW(96) << endl;
  cout << "SMZ(96) " << pythia->GetSMZ(96) << endl;

  cout << " MSTJ(11,5)     " << pythia->GetMSTJ(11) << endl;
  cout << " PARJ(1,0.085)  " << pythia->GetPARJ(1) << endl;
  cout << " PARJ(2,0.20)   " << pythia->GetPARJ(2) << endl;
  cout << " PARJ(3,0.92)   " << pythia->GetPARJ(3) << endl;
  cout << " PARJ(4,0.043)  " << pythia->GetPARJ(4) << endl;
  cout << " PARJ(6,1.0)    " << pythia->GetPARJ(6) << endl;
  cout << " PARJ(7,1.0)    " << pythia->GetPARJ(7) << endl;
  cout << " PARJ(11,0.35)  " << pythia->GetPARJ(11) << endl;
  cout << " PARJ(12,0.40)  " << pythia->GetPARJ(12) << endl;
  cout << " PARJ(13,0.54)  " << pythia->GetPARJ(13) << endl;
  cout << " PARJ(21,0.33)  " << pythia->GetPARJ(21) << endl;
  cout << " PARJ(25,0.70)  " << pythia->GetPARJ(25) << endl;
  cout << " PARJ(26,0.135  " << pythia->GetPARJ(26) << endl;
  cout << " PARJ(41,0.45)  " << pythia->GetPARJ(41) << endl;
  cout << " PARJ(42,1.0)   " << pythia->GetPARJ(42) << endl;
  cout << " PARJ(45,0.86)  " << pythia->GetPARJ(45) << endl;
  cout << " PARJ(46,1.0)   " << pythia->GetPARJ(46) << endl;
  cout << " PARJ(47,1.0)   " << pythia->GetPARJ(47) << endl;

  for (Int_t iEv = 0; iEv < inp.nEvents; iEv++) {
    /* cout << " |<- E" << endl; */
    // Show how far we got every 100'th event.
    if (iEv % 500 == 0) {
        cout << "Event # " << iEv << endl;
        inp.f_log << " # Starting event " << setw(7) << right << iEv << left << "   time elapsed: " << inp.time_ellapsed() << endl;
    }

    // Make one event.
    pythia->GenerateEvent();
    b_jets.Clear();

    mEvent.n_c = 0;
    mEvent.n_n = 0;

    mEvent.ptsum_c = 0;
    mEvent.ptsum_n = 0;

    mEvent.ptmax_c = 0;
    mEvent.ptmax_n = 0;


    vector<PseudoJet> jet_particles;
    /* TMCParticle* p; */
    /* TIter piter(particles); */
    /* int c {0}; */

    TDatabasePDG pdb;
    for (int i{1}; i < particles->GetEntries()+1; ++i) {
        if (pythia->GetK(i,1) == 0 || pythia->GetK(i,1) > 9) continue;
        StThreeVector<double> tr { pythia->GetP(i,1),
                            pythia->GetP(i,2),
                            pythia->GetP(i,3) };
        double eta { tr.pseudoRapidity() };
        double pt  { tr.perp()};
        if (pt < 2 || TMath::Abs(eta) > 1 ) continue;

        double phi { tr.phi() };

        // neutral particles
        if ( (pdb.GetParticle(pythia->GetK(i,2)))->Charge() ) {
                /* pdg->Charge() == 0) { */
            ++mEvent.n_n;
            mEvent.ptsum_n += pt;
            if (pt > mEvent.ptmax_n) {
                mEvent.ptmax_n = pt;
                mEvent.eta_n = eta;
                mEvent.phi_n = phi;
            }
            // NOTE: jets are not just charged at this point!
            /* continue; */
        } else {
            // charged particles
            ++mEvent.n_c;
            mEvent.ptsum_c += pt;
            if (pt > mEvent.ptmax_c) {
                mEvent.ptmax_c = pt;
                mEvent.eta_c = eta;
                mEvent.phi_c = phi;
            }
        }

        jet_particles.push_back(PseudoJet{
                pythia->GetP(i,1), pythia->GetP(i,2),pythia->GetP(i,2),pythia->GetP(i,4)});
                /* p->GetPx(), p->GetPy(), p->GetPz(), p->GetEnergy()} ); */
    }

    ClusterSequenceArea cs{jet_particles, jet_def, area_def};
    vector<PseudoJet> jets = sorted_by_pt ( (!SelectorIsPureGhost())(cs.inclusive_jets()) ) ;
    bge_rm2->set_particles(jet_particles);
    mEvent.rho = bge_rm2->rho();


    /* cout << "           <- jets " << endl; */
    int njets{0};
    Selector pt_min { SelectorPtMin(0.2) };
    for (auto jet : jets) {
        h_jets->Fill(jet.pt());

        JtJet* b_jet = (JtJet*) b_jets.ConstructedAt(njets);
        ++njets;
        b_jet->phi = jet.phi_std();
        b_jet->eta = jet.eta();
        b_jet->pt  = jet.pt();
        b_jet->pt_corr  = (jet - jet.area_4vector()*mEvent.rho).pt();
        b_jet->area = jet.area();
        b_jet->nch  = ( (pt_min)(jet.constituents()) ).size();
    }
    mEvent.njets = njets;
    // Now we're ready to fill the tree, and the event is over.
    /* cout << "                 ----fill----  " << endl; */
    tree->Fill();
  }
  cout << "Finished running pythia" << endl;
};
