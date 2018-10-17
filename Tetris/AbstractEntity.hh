#ifndef ABSTRACTENTITY_HH
#define ABSTRACTENTITY_HH


class AbstractEntity
{
    public:
        virtual void Draw();
        virtual void Update();

    protected:
        AbstractEntity();
        virtual ~AbstractEntity();
    private:
};

#endif // ABSTRACTENTITY_HH
