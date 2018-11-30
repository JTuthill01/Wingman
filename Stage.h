#pragma once
#include "TileArray.h"


class Stage
{
public:
	Stage();
	virtual ~Stage();

	void draw(sf::RenderTarget &target, sf::View &view);

	void update(int fromColumn, int toColumn,
		int fromRow, int toRow);

	//Static Variables
	static int gridSize; 

	TileArray<TileArray<Tile>>& getTile() { return this->m_tiles; }
private:
	unsigned long m_stageSizeX;
	unsigned long m_stageSizeY;

	TileArray<TileArray<Tile>> m_tiles; 
	
	float m_deltaTimeMultiplier; 

	//Optimization
	int m_fromColumn, m_toColumn;
	int m_fromRow, m_toRow; 
};

