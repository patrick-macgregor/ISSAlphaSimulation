#include "AlphaNuclide.hh"

void AlphaNuclide::AddDecay( double e, double i ){
	AlphaDecay a = AlphaDecay(e,i);
	fAlphaDecay.push_back(a);
	return;
}
// --------------------------------------------------------------------------------------------- //
void AlphaNuclide::CheckIntensitySum(){
	double sum = 0;
	for ( unsigned int i = 0; i < fAlphaDecay.size(); i++ ){
		sum += fAlphaDecay[i].GetIntensity();
	}

	if ( sum != 100.0 ){
		double scale = 100.0/sum;
		sum = 0;
		for ( unsigned int i = 0; i < fAlphaDecay.size(); i++ ){
			fAlphaDecay[i].SetIntensity( fAlphaDecay[i].GetIntensity()*scale );
			sum += fAlphaDecay[i].GetIntensity();
		}
	}
	return;
}
// --------------------------------------------------------------------------------------------- //
void AlphaNuclide::CalculateCumulativeProbability(){
	fCumulativeProbability.clear();
	double sum = 0;
	for ( unsigned int i = 0; i < fAlphaDecay.size(); i++ ){
		sum += fAlphaDecay[i].GetIntensity()*0.01;
		fCumulativeProbability.push_back(sum);
	}
	if ( sum > 1 ){
		std::cout << "CUMULATIVE PROBABILITY > 1 ERROR" << std::endl;
	}
	return;
}
// ============================================================================================= //
AlphaNuclide::AlphaNuclide(){
	fIntensityInSource = 0;
}
// --------------------------------------------------------------------------------------------- //
AlphaNuclide::~AlphaNuclide(){}
// --------------------------------------------------------------------------------------------- //
void AlphaNuclide::AddDecayArray( int n, const double arr[][2], double in ){
	// Set relative intensity within source
	fIntensityInSource = in;	
	
	// Now add the decay array
	AlphaDecay d;
	for ( int i = 0; i < n; i++ ){
		d.SetEnergy( arr[i][0] );
		d.SetIntensity( arr[i][1] );
		this->AddDecay(d);
	}
	
	// Check intensities
	this->CheckIntensitySum();
	
	// Calculate cumulative probabilities
	this->CalculateCumulativeProbability();
	return;
}
// --------------------------------------------------------------------------------------------- //
double AlphaNuclide::GetMinEnergy(){
	double min = fAlphaDecay[0].GetEnergy();
	for ( unsigned int i = 1; i < fAlphaDecay.size(); i++ ){
		if ( fAlphaDecay[i].GetEnergy() < min ){ min = fAlphaDecay[i].GetEnergy(); }
	}
	return min;
}
// --------------------------------------------------------------------------------------------- //
double AlphaNuclide::GetMaxEnergy(){
	double max = fAlphaDecay[0].GetEnergy();
	for ( unsigned int i = 1; i < fAlphaDecay.size(); i++ ){
		if ( fAlphaDecay[i].GetEnergy() > max ){ max = fAlphaDecay[i].GetEnergy(); }
	}
	return max;
}
// --------------------------------------------------------------------------------------------- //
double AlphaNuclide::GetMostProbableDecayEnergy(){
	int index = 0;
	double max_intensity = 0;
	
	for ( unsigned int i = 0; i < fAlphaDecay.size(); i++ ){
		if ( fAlphaDecay[i].GetIntensity() > max_intensity ){
			max_intensity = fAlphaDecay[i].GetIntensity();
			index = i;
		}
	}
	return fAlphaDecay[index].GetEnergy();
}















































