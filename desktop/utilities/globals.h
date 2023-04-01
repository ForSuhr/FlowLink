#ifndef GLOBALS_H
#define GLOBALS_H

struct Device
{
    QString name;
    QString address;

    bool operator==(const Device &other) const
    {
        return name == other.name && address == other.address;
    }
};

#endif // GLOBALS_H