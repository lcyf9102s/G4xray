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
#include "G4HadronicParameters.hh"
#include "TH1.h"
#include "TFile.h"
#include "TGraph.h"
#include "TCanvas.h"
#include "TStyle.h"
#include <cstdlib>
#include <fstream>

void dot() {
    // 打开 ROOT 文件
    TFile *file = new TFile("test3.root", "READ");

    if (!file || file->IsZombie()) {
        std::cerr << "Error opening file!" << std::endl;
        
    }

    // 获取直方图
    TH1D *hist = (TH1D*)file->Get("Edep");
    if (!hist) {
        std::cerr << "Error: Histogram 'Edep' not found!" << std::endl;
        file->Close();
        
    }

    // 打开输出文件
    std::ofstream outFile("output.dat");
    if (!outFile) {
        std::cerr << "Error opening output file!" << std::endl;
        file->Close();
        
    }

    // 写入数据点
    int nPoints = hist->GetNbinsX();
    for (int i = 1; i <= nPoints; ++i) {
        double x = hist->GetBinCenter(i);
        double y = hist->GetBinContent(i);
        outFile << x << " " << y << "\n";
    }

    // 关闭文件
    outFile.close();
    file->Close();
    delete file;

    std::cout << "Data points have been written to output.dat" << std::endl;

}

void nn()
{
    using namespace std;
    TGraph *graph = new TGraph();
    graph->SetMarkerStyle(kFullCircle);

    fstream file;
    file.open("output.dat", ios::in);

    while(true)
    {
        double x, y;
        file >> x >> y;
        graph->SetPoint(graph->GetN(), x, y);
        if(file.eof()) break;
    }
    file.close();
    
    graph->GetXaxis()->SetTitle("Channel");
    graph->GetXaxis()->CenterTitle();
    graph->GetYaxis()->SetTitle("Counts");
    graph->GetYaxis()->CenterTitle();

    TCanvas *canvas = new TCanvas("canvas", "Exponential Decay Fit", 800, 600);
    graph->Draw("AL");

    graph->GetXaxis()->SetRangeUser(0, 2047);
    canvas->Update();
    canvas->SaveAs("EnergyDeposition.png");

}

int main(int argc, char** argv)
{
    system("rm *.root *.png *.dat");
    G4MTRunManager *runManager = new G4MTRunManager();
    runManager->SetUserInitialization(new MyDetectorConstruction());
    runManager->SetUserInitialization(new MyPhysicsList());
    runManager->SetUserInitialization(new MyActionInitialization());


    // runManager->Initialize();
    //G4HadronicParameters::Instance()->SetTimeThresholdForRadioactiveDecay( 1.0e+60*CLHEP::year );

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
        system("hadd test3.root *.root");
        dot();
        nn();
    }

    return 0;
}
