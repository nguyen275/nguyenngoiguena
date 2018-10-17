#ifndef PIECE_HH
#define PIECE_HH

#include "AbstractEntity.hh"
#include <string>

using namespace std;

class Piece: public AbstractEntity
{
    protected:
        string shape;
    public:
        Piece(string shape);
        virtual ~Piece();

        void Update() override;
        void Draw() override;

        const string &GetShape() const;
        void SetShape(const string &shape);

        static int Rotate(int px, int py, int r);
};

#endif // PIECE_HH
