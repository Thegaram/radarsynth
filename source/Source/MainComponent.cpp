/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/

#include "MainComponent.h"


//==============================================================================
MainContentComponent::MainContentComponent()
{
	addAndMakeVisible(m_grid);
    setSize (800, 800);
}

void MainContentComponent::resized()
{
	m_grid.setSize(getWidth(), getHeight());
	//m_grid.resized();
    
	// This is called when the MainContentComponent is resized.
    // If you add any child components, this is where you should
    // update their positions.
}
