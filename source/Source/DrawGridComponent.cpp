/*
  ==============================================================================

    DrawGridComponent.cpp
    Created: 2 Jun 2015 12:18:35pm
    Author:  s_garamv

  ==============================================================================
*/

#include "DrawGridComponent.h"

#include <algorithm>


//==============================================================================
const unsigned DrawGridComponent::DEFAULT_GRID_WIDTH  = 50;
const unsigned DrawGridComponent::DEFAULT_GRID_HEIGHT = 50;
const float DrawGridComponent::GRID_LINE_WIDTH_COEFF  = 20;


DrawGridComponent::DrawGridComponent(unsigned gridWidth, unsigned gridHeight)
	: m_data(gridWidth, gridHeight), m_drawingState(DRAWING)
{
	// set colour mappings
	m_colourMapping.set(CELL_INACTIVE, Colours::white); // empty
	m_colourMapping.set(CELL_CLICKED, Colours::red);	// filled
	m_colourMapping.set(CELL_MOUSEOVER, Colours::blue); // hover

	// set component size
	setSize(400, 400);

	// initialize grid (matrix, cell dimensions, line thickness)
	m_gridLineThickness = GRID_LINE_WIDTH_COEFF / std::max(m_data.width(), m_data.height());
}

void DrawGridComponent::paint(Graphics &g)
{
	// draw each cell
	for (unsigned y = 0; y < m_data.height(); ++y)
		for (unsigned x = 0; x < m_data.width(); ++x)
			drawCell(g, x, y);
}

void DrawGridComponent::resized()
{
	// EMPTY
}

void DrawGridComponent::setGridSize(unsigned newWidth, unsigned newHeight)
{
	m_data.resize(newWidth, newHeight);
}

Rectangle<float> DrawGridComponent::cellToRectangle(unsigned x, unsigned y)
{
	// calculate cell sizes
	unsigned cellWidth = (float)getWidth() / m_data.width();
	unsigned cellHeight = (float)getHeight() / m_data.height();

	// calculate rectangle
	return Rectangle<float>(x * cellWidth, y * cellHeight, cellWidth, cellHeight);
}

void DrawGridComponent::drawCell(Graphics &g, unsigned x, unsigned y)
{
	// get rectangle
	Rectangle<float> rec = cellToRectangle(x, y);

	// fill rectangle
	g.setColour(m_colourMapping[m_data(x, y)]);
	g.fillRect(rec);

	// outline
	g.setColour(Colours::black);
	g.drawRect(rec, m_gridLineThickness);
}

DrawGridComponent::DrawingState DrawGridComponent::changeDrawingState()
{
	return m_drawingState = (m_drawingState == DRAWING) ? ERASING : DRAWING;
}

unsigned DrawGridComponent::convertMouseXCoordinateToCell(int mouseX)
{
	return ((float)mouseX / getWidth()) * m_data.width();
}

unsigned DrawGridComponent::convertMouseYCoordinateToCell(int mouseY)
{
	return ((float)mouseY / getHeight()) * m_data.height();
}

bool DrawGridComponent::isMouseEventInComponent(const MouseEvent& event)
{
	return((event.position.x >= 0.f) &&
		   (event.position.y >= 0.f) &&
		   (event.position.x < getWidth()) &&
		   (event.position.y < getHeight()));
}

void DrawGridComponent::mouseDown(const MouseEvent &event)
{
	// check location
	if (!isMouseEventInComponent(event))
		return;

	// get cell
	unsigned x = convertMouseXCoordinateToCell(event.x);
	unsigned y = convertMouseYCoordinateToCell(event.y);

	// change cell state
	char& element = m_data(x, y);
	if (m_drawingState == ERASING || element == CELL_CLICKED)
		element = CELL_MOUSEOVER;
	else
		element = CELL_CLICKED;

	repaint();
}

void DrawGridComponent::mouseMove(const MouseEvent &event)
{
	static unsigned prevX = convertMouseXCoordinateToCell(event.x);
	static unsigned prevY = convertMouseYCoordinateToCell(event.y);

	// check location
	if (!isMouseEventInComponent(event))
		return;

	// get current cell
	unsigned currentX = convertMouseXCoordinateToCell(event.x);
	unsigned currentY = convertMouseYCoordinateToCell(event.y);

	// update current
	char& current = m_data(currentX, currentY);
	if (current == CELL_INACTIVE)
		current = CELL_MOUSEOVER;

	// update previous
	char& prev = m_data(prevX, prevY);
	if ((currentX != prevX) || (currentY != prevY))
		if (prev == CELL_MOUSEOVER)
			prev = CELL_INACTIVE;

	// save previous cell
	prevX = currentX;
	prevY = currentY;

	repaint();
}

void DrawGridComponent::mouseDrag(const MouseEvent &event)
{
	// check location
	if (!isMouseEventInComponent(event))
		return;

	// get cell
	unsigned x = convertMouseXCoordinateToCell(event.x);
	unsigned y = convertMouseYCoordinateToCell(event.y);

	// update cell
	m_data(x, y) = (m_drawingState == DRAWING) ? CELL_CLICKED : CELL_MOUSEOVER;

	repaint();
}