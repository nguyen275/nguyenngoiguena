#include "PieceFactory.hh"

PieceFactory::PieceFactory()
{
    //ctor
}

PieceFactory::~PieceFactory()
{
    //dtor
}

const string PieceFactory::I = "ITetromino";
const string PieceFactory::J = "JTetromino";
const string PieceFactory::L = "LTetromino";
const string PieceFactory::O = "OTetromino";
const string PieceFactory::S = "STetromino";
const string PieceFactory::T = "TTetromino";
const string PieceFactory::Z = "ZTetromino";

Piece *PieceFactory::Create(const std::string& str)
{
    if (str == PieceFactory::I) {
        return new ITetromino;
    } else if(str == PieceFactory::J) {
        return new JTetromino;
    } else if(str == PieceFactory::L) {
        return new LTetromino;
    } else if(str == PieceFactory::O) {
        return new OTetromino;
    } else if(str == PieceFactory::S) {
        return new STetromino;
    } else if(str == PieceFactory::T) {
        return new JTetromino;
    } else if(str == PieceFactory::J) {
        return new TTetromino;
    } else if(str == PieceFactory::Z) {
        return new ZTetromino;
    }else {
        return nullptr;
    }
}

