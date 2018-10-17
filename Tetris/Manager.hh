#ifndef MANAGER_HH
#define MANAGER_HH
#include <string>

class Manager
{
    public:
        int _nFieldWidth = 12;
        int _nFieldHeight = 18;
        std::string _tetromino[7];
        unsigned char *_pField = nullptr;
    public:
        Manager();
        virtual ~Manager();
        int Rotate(int px, int py, int r);
        bool DoesPieceFit(int nTetromino, int nRotation, int nPosX, int nPosY);

    protected:

    private:
};

#endif // MANAGER_HH
