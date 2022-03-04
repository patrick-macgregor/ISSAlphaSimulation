#ifndef _ALPHA_DECAY_HH_
#define _ALPHA_DECAY_HH_

#include <iostream>

class AlphaDecay{
	private:
		double fEnergy;
		double fIntensity;
	
	public:
		AlphaDecay()
		 	: fEnergy( 0.0 ), fIntensity( 0.0 ){}
		AlphaDecay( double e, double i )
			: fEnergy( e ), fIntensity( i ){}
		~AlphaDecay(){}
		
		void SetEnergy( double e ){ 
			if ( fEnergy >= 0 ){ fEnergy = e; }
			else{ std::cout << "Energy value " << e << " is invalid." << std::endl; }
		}
		void SetIntensity( double i ){
			if ( fIntensity >= 0 && fIntensity <= 100 ){ fIntensity = i; }
			else{ std::cout << "Intensity value " << i << " is invalid." << std::endl; }
		}
		
		inline double GetEnergy(){ return fEnergy; }
		inline double GetIntensity(){ return fIntensity; }
};

#endif
