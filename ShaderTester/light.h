#ifndef LIGHT_H
#define LIGHT_H


class Light
{
public:
    virtual ~Light() {}
    virtual void paintGL() = 0;
    virtual int getLightId() = 0;
};

#endif // LIGHT_H
