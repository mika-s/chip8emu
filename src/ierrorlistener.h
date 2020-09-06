#ifndef IERRORLISTENER_H
#define IERRORLISTENER_H


namespace Emu {

class IErrorListener {

public:
    virtual ~IErrorListener() = default;
    virtual void latestErrorChanged() = 0;
};

}

#endif // IERRORLISTENER_H
