void EdepSpectrum() {
    TFile *f = TFile::Open("steps.root");
    TTree *t = (TTree*)f->Get("steps");

    double dEStep;
    int eventID;
    t->SetBranchAddress("dEStep_MeV",&dEStep);
    t->SetBranchAddress("eventID",&eventID);

    // accumulate edep per event
    std::map<int,double> eventEdep;
    Long64_t n = t->GetEntries();
    for(Long64_t i=0;i<n;i++){
        t->GetEntry(i);
        eventEdep[eventID] += dEStep;
    }

    // fill data to histogram
    TH1D *hEdep = new TH1D("hEdep","Deposited Energy per Event;Edep [MeV];Counts",200,0,7);
    for(auto &kv : eventEdep){
        hEdep->Fill(kv.second);
    }

    hEdep->Draw();
}