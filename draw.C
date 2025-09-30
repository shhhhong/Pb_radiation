void draw(Int_t targetTrack = 2, TString pName = "e-")
{
  ///////////////////////////////////////////////////////
  // Draw track of a given trackID in XZ plane
    // targetTrack: trackID to be drawn
    // pName: particle name, e.g. "e-", "gamma", "proton"
  //////////////////////////////////////////////////////


  // open the file and get the tree
  TFile *f = new TFile("steps.root");
  TTree *t = (TTree*)f->Get("steps");

  // set branch addresses
  Int_t trackID, parentID;
  Double_t preX, preZ, postX, postZ;
  Char_t particle[20];

  t->SetBranchAddress("trackID",&trackID);
  t->SetBranchAddress("parentID",&parentID);
  t->SetBranchAddress("particle",&particle);
  t->SetBranchAddress("preX_mm",&preX);
  t->SetBranchAddress("preZ_mm",&preZ);
  t->SetBranchAddress("postX_mm",&postX);
  t->SetBranchAddress("postZ_mm",&postZ);


  std::vector<double> xs, zs;

  Long64_t nentries = t->GetEntries();
  for(Long64_t i=0;i<nentries;i++){
    t->GetEntry(i);
    // select steps of the given trackID and particle name, exclude primary (parentID=0)
    // if parentID>0, it is a secondary particle
    // if parentID=0, it is a primary particle
    // if parentID<0, it is a special particle (e.g. geantino)
    // here we only draw secondary particles
    // you can modify the condition as needed
    // e.g. to draw primary particle, use "parentID==0"
    if(TString(particle) == pName && trackID == targetTrack && parentID > 0){
      xs.push_back(preX);
      zs.push_back(preZ);
      xs.push_back(postX);
      zs.push_back(postZ);
    }
  }

  // draw
  auto c = new TCanvas("c","XZ track",800,600);
  TGraph *gr = new TGraph(xs.size(), &xs[0], &zs[0]);
  gr->SetTitle(Form("%s trackID=%d in XZ plane;X [mm];Z [mm]",
                  pName.Data(), targetTrack));
  gr->SetLineColor(kBlue);
  gr->SetLineWidth(2);
  gr->Draw("AL");
}