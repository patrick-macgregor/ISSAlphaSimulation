#ifndef _ALPHA_SOURCE_HH_
#define _ALPHA_SOURCE_HH_

#include <vector>
#include "AlphaNuclide.hh"

class AlphaSource{
	private:
		std::vector<AlphaNuclide*> fAlphaNuclide;	// Holds information about all the nuclides
		std::vector<double> fCumulativeProbability;
		
	public:
		AlphaSource();
		~AlphaSource();
		
		inline void AddNuclide( AlphaNuclide *a ){ fAlphaNuclide.push_back( a ); }
		void CheckIntensities();
		AlphaNuclide* GetNuclide( unsigned int i);
		int GetNumberOfNuclidesInSource(){ return fAlphaNuclide.size(); }
		void CalculateCumulativeProbability();
		
		inline double GetCumulativeProbabilityOfSource( int i ){ return fCumulativeProbability[i]; }
		
};

#endif
