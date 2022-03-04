#include "ProgressBar.hh"

ProgressBar::ProgressBar( int max_entries ){
	fMaxEntries = max_entries;
	fProgressPercentage = 1;		// Change every 1% as default
	ResetProgress();
}
// --------------------------------------------------------------------------------------------- //
ProgressBar::ProgressBar( int max_entries, float progress_percentage ){
	fMaxEntries = max_entries;
	fProgressPercentage = progress_percentage;
	ResetProgress();
}
// --------------------------------------------------------------------------------------------- //
ProgressBar::~ProgressBar(){}
// --------------------------------------------------------------------------------------------- //
void ProgressBar::ResetProgress(){
	fCurrentPercentage = 0;
	fProgressDivisor = fProgressPercentage*0.01*fMaxEntries;
	return;
}
// --------------------------------------------------------------------------------------------- //
void ProgressBar::UpdateProgress( int i ){
	if ( i % fProgressDivisor == 0 ){
		PrintProgress();
		fCurrentPercentage += fProgressPercentage;
	}
	return;
}
// --------------------------------------------------------------------------------------------- //
TString ProgressBar::GetProgressBar(){
	int bar_width = 50;		// Change this to change how your bar looks
	TString bar = "[";
	int num_bars = (int)( fCurrentPercentage*bar_width/100 );
	
	// Append progress
	bar.Append('=', num_bars );
	bar.Append(' ', bar_width - num_bars );
	bar.Append(']');
	return bar;
}
// --------------------------------------------------------------------------------------------- //
// Change precision if you want more precise printed percentage
void ProgressBar::PrintProgress(){
	std::cout << GetProgressBar() << std::setw(3) << std::setprecision(0) << std::fixed << fCurrentPercentage << " %\r" << std::flush;
	return;
}

