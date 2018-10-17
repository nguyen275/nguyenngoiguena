#ifndef PIECEFACTORY_HH
#define PIECEFACTORY_HH

#include <string>
#include "ITetromino.hh"
#include "JTetromino.hh"
#include "LTetromino.hh"
#include "OTetromino.hh"
#include "STetromino.hh"
#include "TTetromino.hh"
#include "ZTetromino.hh"

using namespace std;

class PieceFactory
{
    public:
        const static string I;
        const static string J;
        const static string L;
        const static string O;
        const static string S;
        const static string T;
        const static string Z;
    public:
        PieceFactory();
        virtual ~PieceFactory();

        static Piece *Create(const string& str);
    protected:

    private:
};

#endif // PIECEFACTORY_HH
