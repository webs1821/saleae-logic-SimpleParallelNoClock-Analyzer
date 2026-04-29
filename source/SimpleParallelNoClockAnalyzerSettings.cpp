#include "SimpleParallelNoClockAnalyzerSettings.h"
#include <AnalyzerHelpers.h>
#include <stdio.h>


#pragma warning( disable : 4996 ) //warning C4996: 'sprintf': This function or variable may be unsafe

SimpleParallelAnalyzerSettings::SimpleParallelAnalyzerSettings()
:
	mClockChannel( UNDEFINED_CHANNEL ),
	mClockEdge( AnalyzerEnums::PosEdge ),
	mMinSettlingTimeNs( 0 ),
	mSwapBytes( false )
{
	U32 count = 16;
	for( U32 i=0; i<count; i++ )
	{
		mDataChannels.push_back( UNDEFINED_CHANNEL );
		AnalyzerSettingInterfaceChannel* data_channel_interface = new AnalyzerSettingInterfaceChannel();
		
		char text[64];
		sprintf( text, "D%d", i );

		data_channel_interface->SetTitleAndTooltip( text, text );
		data_channel_interface->SetChannel( mDataChannels[i] );
		data_channel_interface->SetSelectionOfNoneIsAllowed( true );

		mDataChannelsInterface.push_back( data_channel_interface );
	}

	//The remainder of the old clock channel stuff provides a means to select
	//where the output goes in the waveform window. There is no clock anymore.
	mClockChannelInterface.reset( new AnalyzerSettingInterfaceChannel() );
	mClockChannelInterface->SetTitleAndTooltip( "Text above", "Analyzer displays output above this channel" );
	mClockChannelInterface->SetChannel( mClockChannel );

	mClockEdgeInterface.reset( new AnalyzerSettingInterfaceNumberList() );
	mClockEdgeInterface->SetTitleAndTooltip( "", "" );
	mClockEdgeInterface->AddNumber( AnalyzerEnums::PosEdge, "Data is valid on Clock rising edge", "Data is valid on Clock rising edge" );
	mClockEdgeInterface->AddNumber( AnalyzerEnums::NegEdge, "Data is valid on Clock falling edge", "Data is valid on Clock falling edge" );
	mClockEdgeInterface->SetNumber( mClockEdge );

	mMinSettlingTimeNsInterface.reset( new AnalyzerSettingInterfaceInteger() );
	mMinSettlingTimeNsInterface->SetTitleAndTooltip( "Glitch Filter (ns)", "Minimum settling time in nanoseconds" );
	mMinSettlingTimeNsInterface->SetMax( 1000000000 );
	mMinSettlingTimeNsInterface->SetMin( 0 );
	mMinSettlingTimeNsInterface->SetInteger( mMinSettlingTimeNs );

	mSwapBytesInterface.reset( new AnalyzerSettingInterfaceBool() );
	mSwapBytesInterface->SetCheckBoxText( "Swap Bytes (16-bit / Endianness)" );
	mSwapBytesInterface->SetValue( mSwapBytes );


	for( U32 i=0; i<count; i++ )
	{
		AddInterface( mDataChannelsInterface[i] );
	}

	AddInterface( mClockChannelInterface.get() );
	//AddInterface( mClockEdgeInterface.get() ); removes clock edge from the settings page
	AddInterface( mMinSettlingTimeNsInterface.get() );
	AddInterface( mSwapBytesInterface.get() );

	AddExportOption( 0, "Export as text/csv file" );
	AddExportExtension( 0, "text", "txt" );
	AddExportExtension( 0, "csv", "csv" );

	ClearChannels();
	for( U32 i=0; i<count; i++ )
	{
		char text[64];
		sprintf( text, "D%d", i );
		AddChannel( mDataChannels[i], text, false );
	}
}

SimpleParallelAnalyzerSettings::~SimpleParallelAnalyzerSettings()
{
	U32 count = (U32)mDataChannelsInterface.size();
	for( U32 i=0; i<count; i++ )
		delete mDataChannelsInterface[i];
}

bool SimpleParallelAnalyzerSettings::SetSettingsFromInterfaces()
{
	U32 count = (U32)mDataChannels.size();
	U32 num_used_channels = 0;
	for( U32 i=0; i<count; i++ )
	{
		if( mDataChannelsInterface[i]->GetChannel() != UNDEFINED_CHANNEL )
			num_used_channels++;
	}

	if( num_used_channels == 0 )
	{
		SetErrorText( "Please select at least one channel to use in the parallel bus" );
		return false;
	}

	for( U32 i=0; i<count; i++ )
	{
		mDataChannels[i] = mDataChannelsInterface[i]->GetChannel();
	}

	mClockChannel = mClockChannelInterface->GetChannel();
	mClockEdge = AnalyzerEnums::EdgeDirection( U32( mClockEdgeInterface->GetNumber() ) );
	mMinSettlingTimeNs = mMinSettlingTimeNsInterface->GetInteger();
	mSwapBytes = mSwapBytesInterface->GetValue();

	ClearChannels();
	for( U32 i=0; i<count; i++ )
	{
		char text[64];
		sprintf( text, "D%d", i );
		AddChannel( mDataChannels[i], text, mDataChannels[i] != UNDEFINED_CHANNEL );
	}

	return true;
}

void SimpleParallelAnalyzerSettings::UpdateInterfacesFromSettings()
{
	U32 count = (U32)mDataChannels.size();
	for( U32 i=0; i<count; i++ )
	{
		mDataChannelsInterface[i]->SetChannel( mDataChannels[i] );
	}

	mClockChannelInterface->SetChannel( mClockChannel );
	mClockEdgeInterface->SetNumber( mClockEdge );
	mMinSettlingTimeNsInterface->SetInteger( mMinSettlingTimeNs );
	mSwapBytesInterface->SetValue( mSwapBytes );
}

void SimpleParallelAnalyzerSettings::LoadSettings( const char* settings )
{
	SimpleArchive text_archive;
	text_archive.SetString( settings );

	U32 count = (U32)mDataChannels.size();

	for( U32 i=0; i<count; i++ )
	{
		text_archive >> mDataChannels[i];
	}

	text_archive >> mClockChannel;
	text_archive >> *(U32*)&mClockEdge;
	text_archive >> mMinSettlingTimeNs;
	text_archive >> mSwapBytes;

	ClearChannels();
	for( U32 i=0; i<count; i++ )
	{
		char text[64];
		sprintf( text, "D%d", i );
		AddChannel( mDataChannels[i], text, mDataChannels[i] != UNDEFINED_CHANNEL );
	}

	AddChannel( mClockChannel, "Clock", true );

	UpdateInterfacesFromSettings();
}

const char* SimpleParallelAnalyzerSettings::SaveSettings()
{
	SimpleArchive text_archive;

		U32 count = (U32)mDataChannels.size();

	for( U32 i=0; i<count; i++ )
	{
		text_archive << mDataChannels[i];
	}

	text_archive << mClockChannel;
	text_archive << mClockEdge;
	text_archive << mMinSettlingTimeNs;
	text_archive << mSwapBytes;

	return SetReturnString( text_archive.GetString() );
}
