#ifndef _ALPHA_PARTICLE_HH_
#define _ALPHA_PARTICLE_HH_

#include <AlphaDecay.hh>
#include <vector>

class AlphaNuclide{
	private:
		std::vector<AlphaDecay> fAlphaDecay;			// Holds information about one nuclide decaying via alpha decay
		double fIntensityInSource;						// Holds information about how intense it is within the source
		std::vector<double> fCumulativeProbability;		// Calculates the cumulative probability
		
		inline void AddDecay( AlphaDecay a ){ fAlphaDecay.push_back( a ); }
		void AddDecay( double e, double i );
		void CheckIntensitySum();										// Check the intensities of the decays add to 100
		void CalculateCumulativeProbability();							// Calculates the boundaries for random numbers to select from
		
	public:
		AlphaNuclide();
		~AlphaNuclide();
	
		void AddDecayArray( int n, const double arr[][2], double in );	// Add an array (from NNDC) of energies and intensities (%)
		double GetMinEnergy();
		double GetMaxEnergy();

		inline double GetIntensityInSource(){ return fIntensityInSource; }
		inline void SetIntensityInSource( double i ){ fIntensityInSource = i; }
		inline double GetCumulativeProbabilityOfDecays( int i ){ return fCumulativeProbability[i]; }
		inline int GetNumberOfDecays(){ return fAlphaDecay.size(); }
		inline AlphaDecay GetDecay( int i ){ return fAlphaDecay[i]; }
		double GetMostProbableDecayEnergy();
		
		
};

#endif
