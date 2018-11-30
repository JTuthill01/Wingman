#include "Stage.h"

//Save Format
//rect, rect, rect, rect, position, position, bool, bool
//textureRect, position, collider, damage

int Stage::gridSize = 50; 

Stage::Stage() : m_stageSizeX(100), m_stageSizeY(100), m_tiles(m_stageSizeX)
{
	
	this->m_fromColumn = 0;
	this->m_toColumn = 0;
	this->m_fromRow = 0;
	this->m_toRow = 0;

	for (unsigned i = 0; i < this->m_stageSizeX; i++)
	{
		this->m_tiles.push(TileArray<Tile>(m_stageSizeY), i);
	}
}

Stage::~Stage()
{
}

void Stage::draw(sf::RenderTarget & target, sf::View &view)
{
	this->m_fromColumn = (view.getCenter().x - view.getCenter().x / 2) / Stage::gridSize;
	if (this->m_fromColumn <= 0)
		this->m_fromColumn = 0;
	if (this->m_fromColumn >= this->m_stageSizeX)
		this->m_fromColumn = this->m_stageSizeX;
	

	this->m_toColumn = (view.getCenter().x + view.getCenter().x / 2) / Stage::gridSize + 1;
	if (this->m_toColumn <= 0)
		this->m_toColumn = 0;
	if (this->m_toColumn >= this->m_stageSizeX)
		this->m_toColumn = this->m_stageSizeX;
	

	this->m_fromRow = (view.getCenter().y - view.getCenter().y / 2) / Stage::gridSize;
	if (this->m_fromRow <= 0)
		this->m_fromRow = 0;
	if (this->m_fromRow >= this->m_stageSizeY)
		this->m_fromRow = this->m_stageSizeY;
	

	this->m_toRow = (view.getCenter().y + view.getCenter().y / 2) / Stage::gridSize + 1;
	if (this->m_toRow <= 0)
		this->m_toRow = 0;
	if (this->m_toRow >= this->m_stageSizeY)
		this->m_toRow = this->m_stageSizeY;
	

	for (int i = m_fromColumn; i < this->m_toColumn; i++)
	{
		for (int k = m_fromRow; k < m_toRow; k++ )
		{
			if(!this->m_tiles[i].isNull(k))
				this->m_tiles[i][k].draw(target);
		}
	}
}

void Stage::update(int fromColumn, int toColumn,
	int fromRow, int toRow)
{
}
