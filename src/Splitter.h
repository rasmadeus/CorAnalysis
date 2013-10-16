#ifndef SPLITTER_H
#define SPLITTER_H

#include <qglobal.h>
#include <QList>

class Interval
{
public:
    Interval(int left, int right);
    int getLeft() const;
    int getRight() const;
private:
    int left;
    int right;
};

class Amplitude
{
public:
    Amplitude(double A);
    double getAmplitude() const;
private:
    double A;
};

class Frequency: public Interval, public Amplitude
{
public:
    Frequency(int left, int right, double T, double A);
    double getPeriod() const;
    double frequency() const;
    double angularSpeed() const;
    double lgAngularSpeed() const;
private:
    double T;
};

class Package: public Amplitude
{
public:
    Package(double A);
    const QList<Frequency>& getFrequencies() const;
    void insert(int left, int right, double T, double A);
private:
    QList<Frequency> fs;
};

class Splitter
{
public:
    void split();
    const QList<Package>& getSamples() const;
private:
    QList<Package> samples;
};

#endif // SPLITTER_H
