#include "AlphaSource.hh"

AlphaSource::AlphaSource(){}
AlphaSource::~AlphaSource(){}
// --------------------------------------------------------------------------------------------- //
void AlphaSource::CheckIntensities(){
	if ( fAlphaNuclide.size() > 0 ){
		double sum = 0;
		for ( unsigned int i = 0; i < fAlphaNuclide.size(); i++ ){
			sum += fAlphaNuclide[i]->GetIntensityInSource();
		}

		if ( sum != 100.0 ){
			double scale = 100.0/sum;
			sum = 0;
			for ( unsigned int i = 0; i < fAlphaNuclide.size(); i++ ){
				fAlphaNuclide[i]->SetIntensityInSource( fAlphaNuclide[i]->GetIntensityInSource()*scale );
				sum += fAlphaNuclide[i]->GetIntensityInSource();
			}
		}
	}
	else{
		std::cout << "Alpha source has no nuclides!" << std::endl;
	}
	return;
}
// --------------------------------------------------------------------------------------------- //
AlphaNuclide* AlphaSource::GetNuclide( unsigned int i){
	if ( i < fAlphaNuclide.size() ){
		return fAlphaNuclide[i];
	}
	std::cout << "Cannot access nuclide with index" << i << " as number of nuclides is " << fAlphaNuclide.size() << std::endl;
	return NULL;
}
// --------------------------------------------------------------------------------------------- //
void AlphaSource::CalculateCumulativeProbability(){
	fCumulativeProbability.clear();
	double sum = 0;
	for ( unsigned int i = 0; i < fAlphaNuclide.size(); i++ ){
		sum += fAlphaNuclide[i]->GetIntensityInSource()*0.01;
		fCumulativeProbability.push_back(sum);
	}
	if ( sum > 1 ){
		std::cout << "CUMULATIVE PROBABILITY > 1. ERROR." << std::endl;
	}

	return;
}

