#ifndef ALIANALYSISTASKGAMMAHADRON_H
#define ALIANALYSISTASKGAMMAHADRON_H

// $Id$
#include "AliAnalysisTaskEmcal.h"
#include "AliEventPoolManager.h"
#include <THn.h>
//#include "AliPool.h"

class TH1;
class TH2;
class TH3;
class THnSparse;
class AliVVZERO;
class AliEvtPoolManager;

using std::vector;

class AliAnalysisTaskGammaHadron : public AliAnalysisTaskEmcal {
 public:
	AliAnalysisTaskGammaHadron();
	AliAnalysisTaskGammaHadron(Bool_t Input_Gamma_Or_Pi0,Bool_t Input_SameEventAnalysis);
  virtual ~AliAnalysisTaskGammaHadron();

  void                        UserCreateOutputObjects();

  //Set things for the analyis
  void                        SetCellEnergyCut(Float_t cut)                        { fCellEnergyCut      = cut      ; }
  void                        SetMaxCellsInCluster(Int_t b)                        { fMaxCellsInCluster  = b        ; }
  void                        SetParticleLevel(Bool_t s)                           { fParticleLevel      = s        ; }
  void                        SetMC(Bool_t m)                                      { fIsMC               = m        ; }
  void                        SetAdditionalCentEst(const char* meth2)              { fCentMethod_alt     = meth2    ; }
  void                        SetAODfilterBits(Int_t b0 = 0, Int_t b1 = 0)         { fAODfilterBits[0]   = b0  ; fAODfilterBits[1] = b1  ; }
  void                        SetEffHistGamma(THnF *h)                             { fHistEff_Gamma      = h        ; }
  void                        SetEffHistHadron(THnF *h)                            { fHistEff_Hadron     = h        ; }

 protected:

  void                        ExecOnce()         										  ;
  void                        InitEventMixer()											  ;
  Bool_t                      RetrieveEventObjects()                                        ;
  TObjArray*                  CloneToCreateTObjArray(AliParticleContainer* tracks)          ;
  Bool_t                      Run()                                                         ;
  Bool_t                      FillHistograms()                                              ;
  Int_t                       CorrelateClusterAndTrack(AliParticleContainer* tracks,TObjArray* bgTracks,Bool_t SameMix, Double_t Weight);
  Int_t                       CorrelatePi0AndTrack(AliParticleContainer* tracks,TObjArray* bgTracks,Bool_t SameMix, Double_t Weight);
  void                        Fill_GH_Hisograms(Int_t identifier,TLorentzVector ClusterVec,AliVParticle* TrackVec, Double_t ClusterEcut, Double_t TrackPcut, Double_t Anglecut, Double_t Weight);
  Bool_t                      AccClusterForAna(AliVCluster* cluster)                        ;
  Double_t                    DeltaPhi(TLorentzVector ClusterVec,AliVParticle* TrackVec)    ;
  Double_t                    GetEff(TLorentzVector ParticleVec)                            ;
  Bool_t                      fGamma_Or_Pi0;            // This tells me whether the correltation and the filling of histograms is done for gamma or pi0
  Bool_t                      fSameEventAnalysis;       // This tells me whether the analysis is done for same event fSameEventAnalysis==1 or mixed events
  Bool_t                      Debug;					   // Can be set for debugging

  // Input histograms
  THnF                      *fHistEff_Gamma;            // input efficiency for trigger particles
  THnF                      *fHistEff_Hadron;           // input efficiency for associate particles

  // Constants
  Double_t                    fRtoD;                     // conversion of rad to degree
  static const Int_t          fN_Identifier=3;           // number of different versions of the same histogram type, can later be used for centrality or mixed event eg.
  static const Int_t          fN_DPhistos=31;            // =  nbins[0];
  vector<Int_t>               fVector_G_Bins;            // vector that contains the bins of the G historgram
  vector<Double_t>            fVector_ZT_Bins;           // vector that contains the bins of the Zt historgram
  vector<Double_t>            fVector_XI_Bins;           // vector that contains the bins of the Xi historgram
  Double_t                    fZT_Step;                  // Bin width for the zT histograms
  Double_t                    fXI_Step;                  // Bin width for the Xi histograms

  TAxis*                      fMixBCent;                 // Number of centrality bins for the mixed event
  TAxis*                      fMixBZvtx;                 // Number of vertex bins for the mixed event
  TString                     fCentMethod_alt;           // alternative centrality selection method
  Double_t                    fCent_alt;                 // alternative centrality
  AliEventPoolManager        *fPoolMgr;                  //! event mixer
  Int_t                       fTrackDepth;               //  #tracks to fill pool
  Int_t                       fPoolSize;                 //  Maximum number of events

  // Cuts
  Float_t                     fCellEnergyCut;            // Energy cell cut
  Int_t                       fMaxCellsInCluster;        // Maximum number (approx) of cells in a cluster

  // MC stuff
  Bool_t                      fParticleLevel;            // Set particle level analysis
  Bool_t                      fIsMC;                     // Trigger, MC analysis
  UInt_t                      fAODfilterBits[2];         // AOD track filter bit map

  // Other stuff
  TList                      *fOutputList1;            //! Output list
  TList                      *fOutputList2;            //! Output list
  TList                      *fOutputList3;            //! Output list
  TList                      *fOutputList_Gamma;       //! Output list
  TList                      *fOutputList_xi;          //! Output list
  TList                      *fOutputList_zeta;        //! Output list

  // Histograms -
  TH1  					    *fHistNoClus_pt_Trigger;   //! No of calorimeter Clusters as a function of p_T
  TH1  					    *fHistNoClus_pt;           //! No of calorimeter Clusters as a function of p_T
  TH1					   **fHistNoClus_ptH;          //! No of calorimeter Clusters as a function of p_T with a hadron in the second hemisphere
  TH1 					    *fHistpi0;                 //!
  TH2					   **fHist_dEta_dPhi_G[3];     //! No of g-h pairs in the deta eta delta phi plane for certain gamma energies
  TH2					   **fHist_dEta_dPhi_ZT[3];    //! No of g-h pairs in the deta eta delta phi plane for certain zT values
  TH2					   **fHist_dEta_dPhi_XI[3];    //! No of g-h pairs in the deta eta delta phi plane for certain Xi values

  TH1					  **fHistpt_assHadron[3];      //! pt distributions of the associated hadron in a certain p_t bin of the gamma
  TH1					  **fHist_DP_gh[3];            //! delta phi g-h distribution fro a given p_t gamma bin
  TH2                	   *fHPoolReady;               //! Check how many Jobs start mixing
  //
  //

 private:
  AliAnalysisTaskGammaHadron(const AliAnalysisTaskGammaHadron&);            // not implemented
  AliAnalysisTaskGammaHadron &operator=(const AliAnalysisTaskGammaHadron&); // not implemented

  ClassDef(AliAnalysisTaskGammaHadron, 7) // Class to analyse gamma hadron correlations
};
#endif
