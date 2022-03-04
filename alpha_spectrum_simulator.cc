#include "AlphaSpectrumSimulatorGlobals.hh"
#include "AlphaNuclide.hh"
#include "AlphaSource.hh"
#include "AlphaSpectrumSimulator.hh"

int main(){
	// Create alpha particles
	AlphaNuclide Gd148, Am241, Pu239, Cm244;
	
	// Add decays to the particles
	Gd148.AddDecayArray( N_Gd148, Gd148_decay_data, 25 );
	Am241.AddDecayArray( N_Am241, Am241_decay_data, 25 );
	Pu239.AddDecayArray( N_Pu239, Pu239_decay_data, 25 );
	Cm244.AddDecayArray( N_Cm244, Cm244_decay_data, 25 );
	
	// Create an alpha source out of all these nuclides
	AlphaSource *a = new AlphaSource();
	a->AddNuclide( &Gd148 );
	a->AddNuclide( &Am241 );
	a->AddNuclide( &Pu239 );
	a->AddNuclide( &Cm244 );
	
	// Check the intensities are all fine (i.e. add to 100)
	a->CheckIntensities();
	
	// Generate alpha spectrum
	AlphaSpectrumSimulator *al_spec_sim = new AlphaSpectrumSimulator( a );
	al_spec_sim->SetNumberOfEvents( 1E6 );
	al_spec_sim->Generate();

	// Find positions of centroids and print results to console and to a file
	al_spec_sim->FindCentroids();
	al_spec_sim->PrintCentroids();
		
	// Print to canvas (and file and root file)
	al_spec_sim->Print();
	
	
	return 0;
}
