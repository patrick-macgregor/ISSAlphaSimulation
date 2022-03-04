#include "AlphaSpectrumSimulator.hh"

void AlphaSpectrumSimulator::Initialise(){
	fNumEvents = 0;
	fTotalFit = NULL;
	
	// Now initialise spectrum --find bounds
	if ( fAlphaSource != NULL ){
		double min = fAlphaSource->GetNuclide(0)->GetMinEnergy();
		double max = fAlphaSource->GetNuclide(0)->GetMaxEnergy();
		for ( int i = 1; i < fAlphaSource->GetNumberOfNuclidesInSource(); i++ ){
			double new_min = fAlphaSource->GetNuclide(i)->GetMinEnergy();
			double new_max = fAlphaSource->GetNuclide(i)->GetMaxEnergy();
			
			if ( new_min < min ){ min = new_min; }
			if ( new_max > max ){ max = new_max; }
		}
		
		// Calculate max and number of bins
		double axis_max_decimal = TMath::Max( TMath::Min( min, 0.1*max ), 4*PEAK_WIDTH ) + max;
		int axis_max = (int)( TMath::Ceil( axis_max_decimal/ENERGY_AXIS_MAX_DIVISOR )*ENERGY_AXIS_MAX_DIVISOR );
		
		double bin_width = PEAK_WIDTH/PEAK_WIDTH_IN_CHANNELS;
		int num_channels = TMath::Ceil( axis_max/bin_width );
		
		fHist = new TH1F( "h_alpha_spec_sim", "Alpha spectrum simulation; Excitation energy (keV); Counts", num_channels, 0, axis_max );
	}
	else{
		fHist = new TH1F( "h_alpha_spec_sim", "Alpha spectrum simulation; Excitation energy (keV); Counts", 1000, 0, 1000 );
	}
	
	// Format histogram HERE
	gStyle->SetOptStat(0);
	fHist->SetLineColor(kRed);
	
	
	return;
}
// --------------------------------------------------------------------------------------------- //
AlphaSpectrumSimulator::AlphaSpectrumSimulator(){
	fAlphaSource = NULL;
	Initialise();
}
// --------------------------------------------------------------------------------------------- //
AlphaSpectrumSimulator::AlphaSpectrumSimulator( AlphaSource *a ){
	fAlphaSource = a;
	Initialise();
}
// --------------------------------------------------------------------------------------------- //
AlphaSpectrumSimulator::~AlphaSpectrumSimulator(){}
// --------------------------------------------------------------------------------------------- //
void AlphaSpectrumSimulator::Generate(){
	// Define random numbers
	TRandomMixMax *r_pick_nuclide = new TRandomMixMax();
	TRandomMixMax *r_pick_decay = new TRandomMixMax();
	TRandomMixMax *r_generate_event = new TRandomMixMax();

	// Pre-calculations
	double sigma = PEAK_WIDTH/( 2*TMath::Sqrt( 2*TMath::Log(2) ) );

	// Have different seeds for them N.B. these can be any numbers, so long as they're different
	r_pick_nuclide->SetSeed(1);
	r_pick_decay->SetSeed(2);
	r_generate_event->SetSeed(3);
	
	// (Re)calculate cumulative probabilities in the source
	fAlphaSource->CalculateCumulativeProbability();
	
	// Progress bar
	fProgressBar = new ProgressBar( fNumEvents );
	
	for ( long i = 0; i < fNumEvents; i++ ){
		double rand = 0;
		int nuclide_in_source = 0;
		int decay_in_nuclide = 0;
		
		// Choose nuclide
		rand = r_pick_nuclide->Uniform(0,1);

		
		for ( int j = 1; j < fAlphaSource->GetNumberOfNuclidesInSource(); j++ ){
			if ( j == 1 && rand < fAlphaSource->GetCumulativeProbabilityOfSource(j-1) ){
				nuclide_in_source = 0;
				break;
			}
			else if ( rand < fAlphaSource->GetCumulativeProbabilityOfSource(j) && rand > fAlphaSource->GetCumulativeProbabilityOfSource(j-1) ){
				nuclide_in_source = j;
				break;
			}
		}

		
		// Choose decay
		rand = r_pick_decay->Uniform(0,1);
		AlphaNuclide *nuclide = fAlphaSource->GetNuclide(nuclide_in_source);
		
		for ( int j = 1; j < nuclide->GetNumberOfDecays(); j++ ){
			if ( j == 1 && rand < nuclide->GetCumulativeProbabilityOfDecays(j-1) ){
				decay_in_nuclide = 0;
				break;
			}
			else if ( rand < nuclide->GetCumulativeProbabilityOfDecays(j) && rand > nuclide->GetCumulativeProbabilityOfDecays(j-1) ){
				decay_in_nuclide = j;
				break;
			}
		}

		// Gaussian distributed state around particular energy
		double energy = nuclide->GetDecay( decay_in_nuclide ).GetEnergy();
		rand = r_generate_event->Gaus( energy, sigma );
		
		// Add to hist
		fHist->Fill( rand );
		
		// Progress
		fProgressBar->UpdateProgress(i);
	}
	
	return;
}
// --------------------------------------------------------------------------------------------- //
// // ADDITIONAL NON-CLASS FUNCTION // //
TString GenerateGaussianPeakString( int num_peaks ){
	TString a = "";
	for ( int i = 0; i < num_peaks; i++ ){
		a.Append( Form( "gaus(%i)%s", 3*i, ( i < num_peaks - 1 ? " + " : "" ) ) );
	}
	return a;
}
// --------------------------------------------------------------------------------------------- //
void AlphaSpectrumSimulator::FindCentroids(){
	// Fit [NUM NUCLIDES] Gaussians
	fTotalFit = new TF1( "totalfit", GenerateGaussianPeakString( fAlphaSource->GetNumberOfNuclidesInSource() ), fHist->GetXaxis()->GetXmin(), fHist->GetXaxis()->GetXmin() );
	fTotalFit->SetNpx(200);
	fTotalFit->SetLineColor(kBlack);
	fTotalFit->SetLineWidth(2);
	
	// Set initial conditions -- can use simulated parameters to guide
	for ( int i = 0; i < fAlphaSource->GetNumberOfNuclidesInSource(); i++ ){
		fTotalFit->SetParLimits( 3*i + 0, fNumEvents/10000, fNumEvents );// Amplitude
		fTotalFit->SetParLimits( 3*i + 1, fAlphaSource->GetNuclide(i)->GetMostProbableDecayEnergy() - 3*PEAK_WIDTH, fAlphaSource->GetNuclide(i)->GetMostProbableDecayEnergy() + 3*PEAK_WIDTH );// Mu
		fTotalFit->SetParLimits( 3*i + 2, PEAK_WIDTH/10, PEAK_WIDTH );// Sigma
		
		fTotalFit->SetParameter( 3*i + 0, fNumEvents/100.0 );// Amplitude
		fTotalFit->SetParameter( 3*i + 1, fAlphaSource->GetNuclide(i)->GetMostProbableDecayEnergy() );// Mu
		fTotalFit->SetParameter( 3*i + 2, PEAK_WIDTH/2.0 );// Sigma
		
		fTotalFit->SetParName( 3*i + 0, Form( "Amp_%i", i ) );// Amplitude
		fTotalFit->SetParName( 3*i + 1, Form( "Mu__%i", i ) );// Mu
		fTotalFit->SetParName( 3*i + 2, Form( "Sig_%i", i ) );// Sigma
	}
	
	fHist->Fit("totalfit", "Q");
	
	return;
}
// --------------------------------------------------------------------------------------------- //
// // ADDITIONAL NON-CLASS FUNCTION // //
template <typename T>
void PrintTableLine(std::vector<T> strings, std::vector<int> col_width, bool is_header = 0 ){
	// Check the vectors are the same length
	if ( strings.size() == col_width.size() ){
		char div = ( is_header ? '+' : '|' );
		char fill = ( is_header ? '-' : ' ' );
		
		// Setup
		std::cout << std::left << std::setfill(fill) << std::setprecision(4);
		
		// Print columns
		for ( unsigned int i = 0; i < strings.size(); i++ ){
			std::cout << div << fill << std::setw(col_width[i]) << (is_header ? fill : strings[i] ); 
		}
		
		// Print end of line
		std::cout << div << std::endl;
	}
	else{
		std::cout << "xxx" << std::endl;
	}
		return;
}
// --------------------------------------------------------------------------------------------- //
void AlphaSpectrumSimulator::PrintCentroids(){
	// Define columns and header line
	std::vector<int> col_width = {25,15,15};
	std::vector<char> header = {'-','-','-'};
	
	// Print top lines
	PrintTableLine<char>( header, col_width, 1 );
	PrintTableLine<TString>( {"Most Probable Decay Line", "Centroid", "Difference" }, col_width );
	PrintTableLine<char>( header, col_width, 1 );
	
	// Write centroids to file
	std::ofstream file_out;
	file_out.open("centroids.dat");
	
	if ( !file_out.is_open() ){
		std::cout << "File failed to open!" << std::endl;
		std::exit(1);
	}

	// Print data
	for ( int i = 0; i < fAlphaSource->GetNumberOfNuclidesInSource(); i++ ){
		PrintTableLine<double>( {fAlphaSource->GetNuclide(i)->GetMostProbableDecayEnergy(), fTotalFit->GetParameter( 3*i + 1 ), fTotalFit->GetParameter( 3*i + 1 ) - fAlphaSource->GetNuclide(i)->GetMostProbableDecayEnergy() }, col_width );
		
		file_out << std::setprecision(8) << fTotalFit->GetParameter( 3*i + 1 ) << std::endl;
	}
	PrintTableLine<char>( header, col_width, 1 );
	
	file_out.close();
	
	

	return;
}

// --------------------------------------------------------------------------------------------- //
void AlphaSpectrumSimulator::Print(){
	// Draw canvas
	TCanvas *c = new TCanvas( "c_spectrum", "CANVAS", 1200, 900 );
	
	// Draw objects
	fHist->Draw();
	if ( fTotalFit != NULL ){ fTotalFit->Draw("SAME"); }
	
	// Print to png
	c->Print("alpha_spectrum_sim_linear.png");
	c->SetLogy();
	c->Print("alpha_spectrum_sim_log.png");
	
	// Write to ROOT file
	TFile *f = new TFile("alpha_spectrum_sim.root", "RECREATE" );
	fHist->Write();
	f->Close();
	return;
}











































