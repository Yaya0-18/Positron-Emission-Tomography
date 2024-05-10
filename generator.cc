#include "generator.hh"

MyPrimaryGenerator::MyPrimaryGenerator()
{
    fparticleGun = new G4ParticleGun(1);

    //define the particle
    G4ParticleTable *particleTable = G4ParticleTable::GetParticleTable();
    
    G4ParticleDefinition *particle = particleTable->FindParticle("geantino");

    //define the place where particle should be created

        //define postion and momentum
        G4ThreeVector pos(0.,0.,0.);
        G4ThreeVector mom(0.,0.,1.);

        fparticleGun->SetParticlePosition(pos);
        fparticleGun->SetParticleMomentumDirection(mom);
        fparticleGun->SetParticleMomentum(0.*GeV);
        fparticleGun->SetParticleDefinition(particle);

}

MyPrimaryGenerator::~MyPrimaryGenerator()
{

    delete fparticleGun;
}

void MyPrimaryGenerator::GeneratePrimaries(G4Event *anEvent)
{
    G4ParticleDefinition *particle = fparticleGun->GetParticleDefinition();

    if(particle == G4Geantino::Geantino())
    {
        G4int Z = 27 ;
        G4int A = 60 ;

        //Charge
        G4double charge = 0.*eplus ;
        
        G4double energy = 0.*keV  ;

        G4ParticleDefinition *ion = G4IonTable::GetIonTable()->GetIon(Z,A,energy);

        fparticleGun->SetParticleDefinition(ion);
        fparticleGun->SetParticleCharge(charge);
    }

    //tell Geant4 to generate 
    fparticleGun->GeneratePrimaryVertex(anEvent);




}