#ifndef SIMPLEPARALLEL_ANALYZER_SETTINGS
#define SIMPLEPARALLEL_ANALYZER_SETTINGS

#include <AnalyzerSettings.h>
#include <AnalyzerTypes.h>

class SimpleParallelAnalyzerSettings : public AnalyzerSettings
{
public:
	SimpleParallelAnalyzerSettings();
	virtual ~SimpleParallelAnalyzerSettings();

	virtual bool SetSettingsFromInterfaces();
	void UpdateInterfacesFromSettings();
	virtual void LoadSettings( const char* settings );
	virtual const char* SaveSettings();

	
	std::vector< Channel > mDataChannels;
	Channel mClockChannel;

	AnalyzerEnums::EdgeDirection mClockEdge;

	int mMinSettlingTimeNs;

protected:
	std::vector< AnalyzerSettingInterfaceChannel* > mDataChannelsInterface;

	std::auto_ptr< AnalyzerSettingInterfaceChannel >	mClockChannelInterface;
	std::auto_ptr< AnalyzerSettingInterfaceNumberList >	mClockEdgeInterface;
	std::auto_ptr< AnalyzerSettingInterfaceInteger > mMinSettlingTimeNsInterface;
};

#endif //SIMPLEPARALLEL_ANALYZER_SETTINGS
