#include "Piece.hh"

Piece::Piece(string shape)
{
    this->shape = shape;
}

Piece::~Piece()
{
    //dtor
}

void Piece::Update()
{
  AbstractEntity::Update();
}

void Piece::Draw()
{
  AbstractEntity::Draw();
}

const string &Piece::GetShape() const
{
  return this->shape;
}

void Piece::SetShape(const string &shape)
{
  this->shape = shape;
}

int Piece::Rotate(int px, int py, int r)
{
	int pi = 0;
	switch (r % 4)
	{
	case 0: pi = py * 4 + px; // 0
        break;

	case 1: pi = 12 + py - (px * 4); // 90
		break;

	case 2: pi = 15 - (py * 4) - px; // 180
		break;

	case 3: pi = 3 - py + (px * 4); // 270
		break;
	}

	return pi;
}
