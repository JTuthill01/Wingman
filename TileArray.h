#pragma once
#include "Tile.h"

template<typename T>
class TileArray
{
public:
	TileArray(unsigned cap);
	TileArray(const TileArray& obj);
	virtual ~TileArray();

	T& operator[](const unsigned index);
	void operator=(const TileArray& obj);

	void push(const T tile, const unsigned index);
	void remove(const unsigned index);
	
	inline unsigned size()const { return this->m_cap; }
	inline unsigned getNrOfTiles()const { return this->m_nrOfTiles; }
	bool isNull(const unsigned index);

private:
	unsigned m_cap;
	unsigned m_nrOfTiles;

	T* *m_tiles;

	void init(unsigned from);
};

template<typename T>

TileArray<T>::TileArray(unsigned cap)
{
	this->m_cap = cap;
	this->m_nrOfTiles = 0;

	this->m_tiles = new T*[this->m_cap];

	this->init(0);
}

template<typename T>
TileArray<T>::TileArray(const TileArray & obj)
{
	this->m_cap = obj.m_cap;
	this->m_nrOfTiles = obj.m_nrOfTiles;

	this->m_tiles = new Tile*[this->m_cap];

	for (size_t i = 0; i < this->m_cap; i++)
	{
		if (obj.m_tiles[i] != nullptr)
			this->m_tiles[i] = new Tile(*obj.m_tiles[i]);
		else
			this->m_tiles[i] = nullptr;
	}
}

template<typename T>
TileArray<T>::~TileArray()
{
	for (size_t i = 0; i < this->m_cap; i++)
	{
		delete this->m_tiles[i];
	}

	delete[]this->m_tiles;
}

template<typename T>
T&  TileArray<T>::operator[](const unsigned index)
{
	if (index < 0 || index >= this->m_cap)
		throw "Operator is out of fucking bounds!!!";

	return *this->m_tiles[index];
}

template<typename T>
void TileArray<T>::operator=(const TileArray & obj)
{
	for (size_t i = 0; i < this->m_cap; i++)
	{
		delete this->m_tiles[i];
	}

	delete[]this->m_tiles;

	this->m_cap = obj.m_cap;
	this->m_nrOfTiles = obj.m_nrOfTiles;

	this->m_tiles = new Tile*[this->m_cap];

	for (size_t i = 0; i < this->m_cap; i++)
	{
		if (obj.m_tiles[i] != nullptr)
			this->m_tiles[i] = new Tile(*obj.m_tiles[i]);
		else
			this->m_tiles[i] = nullptr;
	}
}

template<typename T>
bool TileArray<T>::isNull(const unsigned index) 
{
	if (index < 0 || index >= this->m_cap)
		throw "Tile Array Push is out of fucking bounds!!!!!";

	return this->m_tiles[index] == nullptr;
}


template<typename T>
void TileArray<T>::push(const T tile, const unsigned index)
{
	if (index < 0 || index >= this->m_cap)
		throw "Tile Array Push is out of fucking bounds!!!!!";

	this->m_tiles[index] = new T(tile);
	this->m_nrOfTiles++;
}

template<typename T>
void TileArray<T>::remove(const unsigned index)
{
	if (index < 0 || index >= this->m_cap)
		throw "Tile Array Remove is out of fucking bounds!!!!!";

	delete this->m_tiles[index];
	this->m_tiles[index] = nullptr;
	this->m_nrOfTiles--;
}

template<typename T>
void TileArray<T>::init(unsigned from)
{
	for (size_t i = from; i < this->m_cap; i++)
	{
		this->m_tiles[i] = nullptr;
	}
}

