#include "Splitter.h"

Interval::Interval(int left, int right):
    left(left),
    right(right)
{
}

int Interval::getLeft() const
{
    return left;
}

int Interval::getRight() const
{
    return right;
}

Amplitude::Amplitude(double A):
    A(A)
{
}

double Amplitude::getAmplitude() const
{
    return A;
}

Frequency::Frequency(int left, int right, double T, double A):
    Interval(left, right),
    Amplitude(A),
    T(T)
{
}

double Frequency::getPeriod() const
{
    return T;
}

double Frequency::frequency() const
{
    return T ? 1 / T : 0;
}

#include <math.h>
double Frequency::angularSpeed() const
{
    return 2 * M_PI * frequency();
}

double Frequency::lgAngularSpeed() const
{
    const double angularSpeed = this->angularSpeed();
    return angularSpeed > 0 ? log10(angularSpeed) : 0;
}

Package::Package(double A):
    Amplitude(A)
{
}

const QList<Frequency>& Package::getFrequencies() const
{
    return fs;
}

void Package::insert(int left, int right, double T, double A)
{
    fs << Frequency(left, right, T, A);
}

#include "File.h"
#include "Curves.h"
#include <algorithm>
class SplitAlgorithm
{
public:
    SplitAlgorithm(QList<Package>* samples):
        samples(samples),
        j(0),
        k(0),
        findingA(0),
        right(0),
        iLeft(0),
        iRight(0),
        A(0),
        T(0),
        DA(0.25),
        DT(0.008),
        JMIN(3)
    {
        const int it = Curves::getInstance()->getArgIndex();
        t = File::file()->samples(it);
        const int iIn = Curves::getInstance()->getInputIndex();
        in = File::file()->samples(iIn);
    }

    void pushBack()
    {
        if(samples->isEmpty() || fabs(samples->last().getAmplitude() - A) > DA){
            samples->push_back(Package(A));
        }
        samples->last().insert(iLeft, iRight, T, A);
    }

    double findTime(int i) const
    {
        const double numenator   = t[i] * fabs(in[i+1]) + t[i+1] * fabs(in[i]);
        const double denomenator = fabs(in[i+1]) + fabs(in[i]);
        return numenator / denomenator;
    }

    bool isMoveToFurther(double findingT) const
    {
        const bool isFindingT = fabs(findingT - T) < DT;
        const bool isFindingA = fabs(findingA - A) < DA;
        return isFindingA && isFindingT;
    }

    void split()
    {
        for(int i = 0; i < in.size() - 1; ++i){
            findingA = qMax(findingA, fabs(in[i]));
            if((fabs(in[i+1])) && (in[i] * in[i+1] <= 0.)){
                const double findingTime = findTime(i);
                const int index = i;
                const double findingT    = 2 * (findingTime - right);
                if(isMoveToFurther(findingT)){
                    T = (j * T + findingT)/(j + 1);
                    A = (j * A + findingA)/(j + 1);
                    j++;
                }
                else{
                    if(j>JMIN){
                        pushBack();
                        k++;
                    }
                    iLeft = (j + k) ? iRight : index;
                    T = findingT;
                    A = findingA;
                    j = 0;
                }
                right    = findingTime;
                iRight = index;
                findingA = 0;
            }
            if(i == in.size() - 2){
                if(j > JMIN){
                    pushBack();
                }
            }
        }
    }
private:
    QList<Package>* samples;
    QVector<double> in;
    QVector<double> t;
    int j;
    int k;
    double findingA;
    double right;
    int iLeft;
    int  iRight;
    double A;
    double T;
    const double DA;
    const double DT;
    const int JMIN;
};

void Splitter::split()
{
    samples.clear();
    SplitAlgorithm algorithm(&samples);
    algorithm.split();
}

const QList<Package>& Splitter::getSamples() const
{
    return samples;
}
