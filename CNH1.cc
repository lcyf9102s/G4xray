#include <iostream>
#include "G4RunManager.hh"
#include "G4MTRunManager.hh"
#include <G4UIExecutive.hh>
#include <G4VisManager.hh>
#include <G4VisExecutive.hh>
#include <G4UImanager.hh>
#include <construction.hh>
#include "physics.hh"
#include "action.hh"

int main(int argc, char** argv)
{
    G4MTRunManager *runManager = new G4MTRunManager();
    runManager->SetUserInitialization(new MyDetectorConstruction());
    runManager->SetUserInitialization(new MyPhysicsList());
    runManager->SetUserInitialization(new MyActionInitialization());


    // runManager->Initialize();

    G4UIExecutive *ui = 0;
    if(argc == 1)
    {
        ui = new G4UIExecutive(argc, argv);
    }

    G4VisManager *visManager = new G4VisExecutive();
    visManager->Initialize();
    G4UImanager *UImanager = G4UImanager::GetUIpointer();

    if (ui)
    {
        runManager->Initialize();  
        UImanager->ApplyCommand("/control/execute vis.mac");
        ui->SessionStart(); 
    }
    else
    {
        G4String command = "/control/execute ";
        G4String fileName = argv[1];
        UImanager->ApplyCommand(command+fileName);
    }

    return 0;
}
