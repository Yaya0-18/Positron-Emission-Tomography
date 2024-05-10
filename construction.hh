#ifndef CONSTRUCTION_HH
#define CONSTRUCTION_HH

#include "G4VUserDetectorConstruction.hh"
#include "G4SystemOfUnits.hh"
#include "G4VPhysicalVolume.hh"
#include "G4LogicalVolume.hh"
#include "G4Box.hh"
#include "G4PVPlacement.hh"
#include "G4NistManager.hh"
#include "G4GenericMessenger.hh"
#include "G4OpticalSurface.hh"
#include "G4LogicalSkinSurface.hh"

#include "detector.hh"

class MyDetectorConstruction : public G4VUserDetectorConstruction
{

public:
    MyDetectorConstruction();
    ~MyDetectorConstruction();

    G4LogicalVolume *GetScoringVolume() const {return fScoringVolume;}

    virtual G4VPhysicalVolume *Construct();
    void ConstructScintillator();

private:
    

    
    //Defining the variables
       //The boxes 
            G4Box *solidWorld, *solidRadiator , *solidDetector, *solidScintillator ;
            
            G4LogicalVolume *logicWorld , *logicRadiator ,*logicDetector,*logicScintillator ; 
            G4VPhysicalVolume *physWorld , *physRadiator , *physDetector , *physScintillator ;
        
        //Material 
            G4Material *SiO2 , *H2O , *Aerogel, *worldMat , *NaI;
            G4Element *C ,*Na , *I ;

        void DefineMaterial();
        virtual void ConstructSDandField();



        //The messenger 
            G4GenericMessenger *fMessenger;
            G4LogicalVolume *fScoringVolume;

        G4double xWorld,yWorld,zWorld;
        // Number of Rows and Columns to the optical detector
        G4int nCols,nRows;
        //bool
        G4bool isScintillator ;

        G4OpticalSurface *mirrorSurface;

    //Functions 

    

  



    



};


#endif