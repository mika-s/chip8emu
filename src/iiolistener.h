#ifndef IIOLISTENER_H
#define IIOLISTENER_H


namespace Emu {

class IIoListener {

public:
    virtual ~IIoListener() = default;
    virtual void beep() = 0;
    virtual void draw() = 0;
    virtual void statusChanged() = 0;
};

}

#endif // IIOLISTENER_H

