#ifndef _ALPHA_SPECTRUM_SIMULATOR_HH_
#define _ALPHA_SPECTRUM_SIMULATOR_HH_

#include "AlphaSource.hh"
#include "AlphaSpectrumSimulatorGlobals.hh"
#include "ProgressBar.hh"

#include <TCanvas.h>
#include <TF1.h>
#include <TFile.h>
#include <TH1F.h>
#include <TMath.h>
#include <TRandomGen.h>
#include <TStyle.h>

#include <fstream>
#include <iostream>

class AlphaSpectrumSimulator{
	private:
		long fNumEvents;				// Number of decays to simulate
		TH1F* fHist;					// Histogram to store the spectrum
		AlphaSource *fAlphaSource;		// Pointer to the alpha source used in the simulation
		TF1* fTotalFit;					// Total fit to extract centroids
		ProgressBar *fProgressBar;		// Progress bar
		
		void Initialise();				// Common features to both constructors
	public:
		AlphaSpectrumSimulator();
		AlphaSpectrumSimulator( AlphaSource *a );
		~AlphaSpectrumSimulator();
		void Generate();
		void FindCentroids();
		void PrintCentroids();
		void Print();
		
		inline long GetNumberOfEvents(){ return fNumEvents; }
		inline void SetNumberOfEvents( int n ){ fNumEvents = n; }
};

//TODO -- include more physical effects (energy loss in layers etc.), and fit with Crystal Ball function

#endif
