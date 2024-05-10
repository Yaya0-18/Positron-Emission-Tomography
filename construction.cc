#include "construction.hh"

MyDetectorConstruction::MyDetectorConstruction()
{   
    nCols = 100; nRows=100;

    fMessenger = new G4GenericMessenger(this, "/detector/","Detector Construction");

    fMessenger->DeclareProperty("nCols", nCols, "Number of columns");
    fMessenger->DeclareProperty("nRows", nRows, "Number of rows");
    
    DefineMaterial();

    isScintillator = true;

    //define the size of world volume 
        //determine the dimension for x y and z
            xWorld = 0.5*m ;
            yWorld = 0.5*m  ;
            zWorld = 0.5*m ;

}

MyDetectorConstruction::~MyDetectorConstruction()
{}

void MyDetectorConstruction::DefineMaterial()
{
    G4NistManager *nist = G4NistManager::Instance();
////////////////////////////////////////////////////////////////////////////////////////////////////

//Material
    //Aerogel material 

        //starting with defining its components:

            //defining the silica

            SiO2 = new G4Material("SiO2",2.201 *g/cm3 , 2);
            SiO2->AddElement(nist->FindOrBuildElement("Si"),1);
            SiO2->AddElement(nist->FindOrBuildElement("O"), 2);

            //defining the water material 
            H2O = new G4Material("H2O",1.000 *g/cm3 , 2);
            H2O->AddElement(nist->FindOrBuildElement("H"),2);
            H2O->AddElement(nist->FindOrBuildElement("O"),1);  

            //defining the carboon element
            C = nist->FindOrBuildElement("C");

            //creating the Aerogel
            Aerogel = new G4Material("Aerogel",0.200*g/cm3,3);
            Aerogel->AddMaterial(SiO2,62.5*perCent);
            Aerogel->AddMaterial(H2O,37.4*perCent);
            Aerogel->AddElement(C,0.1*perCent);

        //Adding the optical parameter to the material

            //Defining parameters
                G4double energy[2] = {1.239841939*eV/0.9,1.239841939*eV/0.2};
                G4double rindexAerogel[2]= {1.1,1.1};

            //Add refractive index to the material
                G4MaterialPropertiesTable *mptAerogel = new G4MaterialPropertiesTable();
                mptAerogel->AddProperty("RINDEX",energy,rindexAerogel,2);
                Aerogel->SetMaterialPropertiesTable(mptAerogel);

    //World Material 
        //Defining the world Material
            worldMat = nist->FindOrBuildMaterial("G4_AIR");
        //Adding the optical parameter to the material

            //Defining parameters
                G4double rindexWorld[2]= {1.0,1.0};

            //Add refractive index to the material
                G4MaterialPropertiesTable *mptWorld = new G4MaterialPropertiesTable();
                mptWorld->AddProperty("RINDEX",energy,rindexWorld,2);
                worldMat->SetMaterialPropertiesTable(mptWorld);


//NaI
        //Defining the NaI Material
            //Defining the Elements fo the material 
                Na = nist->FindOrBuildElement("Na");
                I  = nist->FindOrBuildElement("I");
            //Creating the material 
                NaI = new G4Material("NaI",3.67*g/cm3 , 2);
                NaI->AddElement(Na,1);
                NaI->AddElement(I,1);

        //Adding the optical parameter to the material

            //Defining parameters
                G4double rindexNaI[2]= {1.893,1.893};
                G4double fraction[2] ={1.0,1.0};

            //Add refractive index to the material
                G4MaterialPropertiesTable *mptNaI = new G4MaterialPropertiesTable();
                mptNaI->AddProperty("RINDEX",energy,rindexNaI,2);
                //it tells how many photons per each wavelength we created
                mptNaI->AddProperty("SCINTILLATIONCOMPONENT1",energy,fraction,2);
                //tell us how many photon per energy loss of the particle we create
                mptNaI->AddConstProperty("SCINTILLATIONYIELD",38./keV);
                mptNaI->AddConstProperty("RESOLUTIONSCALE" ,1.0);
                //when particle loses energy in scintillator then the photon emitted to the exp func 
                mptNaI->AddConstProperty("SCINTILLATIONTIMECONSTANT1",250*ns);
                //This have something to do with the distribution of the photon
                mptNaI->AddConstProperty("SCINTILLATIONYIELD1",1.);
                
                NaI->SetMaterialPropertiesTable(mptNaI);

    //Mirror

        //Defining parameters
                G4double reflectivity[2]= {1.0,1.0};
        //Creating the material 
            mirrorSurface = new G4OpticalSurface("mirrorSurface");
        //add extras to mirror type ,finish and mYodel
            mirrorSurface->SetType(dielectric_metal);
            mirrorSurface->SetFinish(ground);
            mirrorSurface->SetModel(unified);
            //Add material proprties table 
                G4MaterialPropertiesTable *mptMirror = new G4MaterialPropertiesTable();
                // says what is the fraction of the light already reflected
                mptMirror->AddProperty("REFLECTIVITY",energy ,reflectivity,2);

                mirrorSurface->SetMaterialPropertiesTable(mptMirror);


}

void MyDetectorConstruction::ConstructScintillator()
{
    solidScintillator = new G4Box("solidScintillator",5*cm,5*cm,6*cm);
    logicScintillator = new G4LogicalVolume(solidScintillator , NaI ,"logicScintillator");
    G4LogicalSkinSurface *skin = new G4LogicalSkinSurface("skin",logicWorld ,mirrorSurface);
    //creating the detector 
    solidDetector = new G4Box("solidDetector", 1*cm , 5*cm , 6*cm);
    logicDetector = new G4LogicalVolume(solidDetector,worldMat,"logicDetector");

    fScoringVolume = logicScintillator;
    for(G4int i=0 ; i <6 ; i++)
    {
        

        for(G4int j = 0 ; j <16 ; j++)
        {   
            //Scintillators
                //We create rotations now 
                G4Rotate3D rotZ(j*22.5*deg, G4ThreeVector(0,0,1));
                G4Translate3D transXScint(G4ThreeVector(5./tan(22.5/2*deg)*cm +5.*cm ,0*cm,-40*cm +i*15*cm));
                
                //Now combine rotation and translation 
                G4Transform3D transformScint = (rotZ)*(transXScint);

            physScintillator  = new G4PVPlacement(transformScint,logicScintillator,"physScintillator",logicWorld , false , 0 , true);

            //Detectors
                G4Translate3D transXDet(G4ThreeVector(5./tan(22.5/2*deg)*cm +6.*cm +5.*cm,0*cm,-40*cm +i*15*cm));
                //Now combine rotation and translation 
                G4Transform3D transformDet= (rotZ)*(transXDet);


            physDetector = new G4PVPlacement(transformDet,logicDetector,"physDetector",logicWorld , false , 0 , true);

        }
    }
        

}

G4VPhysicalVolume *MyDetectorConstruction::Construct()
{
   
//Boxes
    //Creating the room / the mother box which contain Everything


        solidWorld = new G4Box("solidWorld",xWorld,yWorld,zWorld);
        logicWorld = new G4LogicalVolume(solidWorld,worldMat,"logicWorld");
        physWorld = new G4PVPlacement(0,G4ThreeVector(0.,0.,0.),logicWorld,"physWorld",0,false,0,true);

    if(isScintillator)
        ConstructScintillator();

    return physWorld;
}

void MyDetectorConstruction::ConstructSDandField()
{

    MySensitiveDetector *sensDet = new MySensitiveDetector("SensitiveDetector");

    
    logicDetector->SetSensitiveDetector(sensDet);
        
}