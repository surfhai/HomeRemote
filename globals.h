#ifndef GLOBALS_H
#define GLOBALS_H

class Globals
{
public:
    ~Globals()
    {
        instanceFlag = false;
    }
    static Globals* getInstance()
    {
        if (!instanceFlag)
        {
            globals = new Globals();
            instanceFlag = true;
            return globals;
        }
        else
        {
            return globals;
        }
    }
    bool debug = false;

private:
    static bool instanceFlag;
    static Globals *globals;
    explicit Globals()
    {
        // private constructor
    }

};

#endif // GLOBALS_H
