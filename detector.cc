#include "detector.hh"

MySensitiveDetector::MySensitiveDetector(G4String name) : G4VSensitiveDetector(name)
{ 
    quEff = new G4PhysicsFreeVector();

    std::ifstream datafile;
    datafile.open("eff.dat");
    
    while(1)
    {
        G4double wlen,queff;
        datafile >> wlen >> queff;

        if(datafile.eof())
            break;
        
        G4cout << wlen << " " << queff << std::endl;
        quEff->InsertValues(wlen,queff/100.);

    }
    
    datafile.close();
    //quEff->SetSpline(false);
}

MySensitiveDetector::~MySensitiveDetector()
{ }

G4bool MySensitiveDetector::ProcessHits(G4Step *aStep, G4TouchableHistory *ROhist)
{
    G4Track *track = aStep->GetTrack();

    //to avoid photon enter from outside detector like a strange value for z
    track->SetTrackStatus(fStopAndKill);
    //sensitive volume
    //define 2 step point start and end
    G4StepPoint *preStepPoint  = aStep->GetPreStepPoint();
    G4StepPoint *postStepPoint = aStep->GetPostStepPoint();

    //get access to position and momentum of the photon
    G4ThreeVector posPhoton = preStepPoint->GetPosition();
    G4ThreeVector momPhoton = preStepPoint->GetMomentum();

    G4double time = preStepPoint->GetGlobalTime();

    //declare variable for wavelength
    G4double wlen = (1.239841939*eV/momPhoton.mag())*1E+03;

    //G4cout << "photon position" << posPhoton << G4endl;
    
    const G4VTouchable *touchable = aStep->GetPreStepPoint()->GetTouchable();

    G4int copyNo = touchable->GetCopyNumber();

    //G4cout << "Copy Number : " << copyNo << G4endl;
    
    G4VPhysicalVolume *physVol = touchable->GetVolume();
    G4ThreeVector posDetector = physVol->GetTranslation();


    #ifndef G4MULTITHREADED
        //G4cout << " Detector position :" << posDetector <<G4endl;
    #endif

    G4int evt = G4RunManager::GetRunManager()->GetCurrentEvent()->GetEventID();
    G4AnalysisManager *man =  G4AnalysisManager::Instance();

    //for tuble 0 photon (MC)
    man->FillNtupleIColumn(0,0,evt);
    man->FillNtupleDColumn(0,1,posPhoton[0]);
    man->FillNtupleDColumn(0,2,posPhoton[1]);
    man->FillNtupleDColumn(0,3,posPhoton[2]);
    man->FillNtupleDColumn(0,4,wlen);
    //man->FillNtupleDColumn(0,5,time);
    man->AddNtupleRow(0);


    
    if(G4UniformRand() < quEff->Value(wlen))
    {
        // for tuble 1 
        man->FillNtupleIColumn(1,0,evt);
        man->FillNtupleDColumn(1,1,posDetector[0]);
        man->FillNtupleDColumn(1,2,posDetector[1]);
        man->FillNtupleDColumn(1,3,posDetector[2]);
        man->AddNtupleRow(1);
    }
    
    return true;
 }