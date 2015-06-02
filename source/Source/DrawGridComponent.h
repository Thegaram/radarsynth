/*
  ==============================================================================

    DrawGridComponent.h
    Created: 2 Jun 2015 12:18:35pm
    Author:  s_garamv

  ==============================================================================
*/

#ifndef DRAWGRIDCOMPONENT_H_INCLUDED
#define DRAWGRIDCOMPONENT_H_INCLUDED


#include "../JuceLibraryCode/JuceHeader.h"

#include "MatrixValueStore.h"


class DrawGridComponent : public Component
{
//=============================== INNER TYPES ===============================
private:
	enum CellState : char
	{
		CELL_INACTIVE = 0, // normal state
		CELL_CLICKED,
		CELL_MOUSEOVER
	};

public:
	enum DrawingState : char
	{
		DRAWING = 0,
		ERASING
	};


//=============================== CONSTANTS ===============================
private:
	static const unsigned DEFAULT_GRID_WIDTH;
	static const unsigned DEFAULT_GRID_HEIGHT;
	static const float GRID_LINE_WIDTH_COEFF;


//=============================== PUBLIC FUNCTIONS ===============================
public:
	DrawGridComponent(unsigned gridWidth = DEFAULT_GRID_WIDTH, unsigned gridHeight = DEFAULT_GRID_HEIGHT);

	void paint(Graphics &g);
	void resized();

	void mouseMove(const MouseEvent& event);
	void mouseDown(const MouseEvent& event);
	void mouseDrag(const MouseEvent& event);

	void setDrawingState(DrawingState state) { m_drawingState = state; }
	DrawingState getDrawingState() const { return m_drawingState; }
	DrawingState changeDrawingState();

	void setGridSize(unsigned newWidth, unsigned newHeight);


//=============================== PRIVATE FUNCTIONS ===============================
private:
	unsigned convertMouseXCoordinateToCell(int mouseX);
	unsigned convertMouseYCoordinateToCell(int mouseY);

	bool isMouseEventInComponent(const MouseEvent& event);

	Rectangle<float> cellToRectangle(unsigned x, unsigned y);
	void drawCell(Graphics &g, unsigned x, unsigned y);


//=============================== MEMBER VARIABLES ===============================
private:
	DrawingState m_drawingState;			/*!< Current drawing state. */

	MatrixValueStore<char> m_data;			/*!< Matrix storing the cell values. */
	HashMap<char, Colour> m_colourMapping;  /*!< Cell-value -> Colour mappings. */

	float m_gridLineThickness;

	//==============================================================================
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(DrawGridComponent)
};


#endif  // DRAWGRIDCOMPONENT_H_INCLUDED
