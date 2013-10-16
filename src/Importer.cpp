#include "Importer.h"
class Integrator
{
public:
    Integrator():
        prevIn(0),
        summ(0)
    {
    }

    void add(double in, double step){
        summ += 0.5 * (in + prevIn) * step;
        prevIn = in;
    }

    double getSumm() const{
        return summ;
    }

    double sqrSumm() const{
        return getSumm() * getSumm();
    }
private:
    double prevIn;
    double summ;
};

#include <math.h>
#include <QVector>
class Afc
{
public:
    Afc(
        const QVector<double>& in,
        const QVector<double>& out,
        const QVector<double>& t,
        double Ain,
        double angularSpeedIn
    ):
        prevSinW0t(0),
        ac(0),
        fc(0),
        in(in),
        out(out),
        t(t),
        Ain(Ain),
        angularSpeedIn(angularSpeedIn)
    {
        const double d = t.last() - t.first();
        k = d ? 2 / d : d;
        calculate();
    }

    double getAc() const{
        const double Aout = k * sqrt(one.sqrSumm() + two.sqrSumm());
        return Ain ? 20 * log10(Aout / Ain) : 0;
    }

    double getFc() const{
        return getDt() * 360;
    }

    double getDt() const{
        return angularSpeedIn ? -phaseOut() / angularSpeedIn : 0;
    }

    double phaseOut() const
    {
        double phase = atan2(two.getSumm(), one.getSumm());
        if(phase > 0){
            phase -= 2* M_PI;
        }
        return phase;
    }

private:
    void calculate()
    {
        for(int i = 0; i < in.size(); ++i){
            const double sinW0t = this->sinW0t(i);
            const double cosW0t = this->cosW0t(sinW0t);
            const double step   = i ? t[i] - t[i - 1] : 0;
            one.add(out[i] * sinW0t, step);
            two.add(out[i] * cosW0t, step);
            prevSinW0t = sinW0t;
        }
    }

    double sinW0t(int i) const
    {
        double res = Ain ? in[i] / Ain : 0;
        if(fabs(res) > 1){
            res = res > 0 ? 1 : -1;
        }
        return res;
    }

    double cosW0t(double sinW0t) const
    {
        const double dSinW0t = sinW0t - prevSinW0t;
        const double cosW0t = sqrt(1 - sinW0t * sinW0t);
        return dSinW0t > 0 ? cosW0t : - cosW0t;
    }

    Integrator one;
    Integrator two;
    double prevSinW0t;
    double ac;
    double fc;
    const QVector<double> in;
    const QVector<double> out;
    const QVector<double> t;
    const double Ain;
    const double angularSpeedIn;
    double k;
};

#include "File.h"
Importer::Importer():
    dir(File::file()->mkdir())
{
}

#include <math.h>
static double roundOneDigit(double value, int precision){
    double mult = pow(10, precision);
    if(!mult){
        mult = 0.0000001;
    }
    const double argument = value * mult + 0.5;
    return (value > 0 ? floor(argument) : ceil(argument)) / mult;
}
static double round(double value, int precision){
    double result = value;
    if(value < 0){
        result *= -1;
    }
    for(int i = 10; i >= precision; --i){
        result = roundOneDigit(result, i);
    }
    return value < 0 ? -result : result;
}

#include "Splitter.h"
QString Importer::getPath(const Package& package) const{
    return dir + QString("/A=%1.txt").arg(round(package.getAmplitude(), 1));
}

#include <QFile>
#include <QTextStream>
void Importer::operator ()(const Package& package) const
{
    QFile file(getPath(package));
    if(file.open(QIODevice::WriteOnly)){
        QTextStream out(&file);
        writeHeader(out);
        foreach(Frequency frequency, package.getFrequencies()){
            writeFrequency(frequency, out);
        }
    }
}

#include "File.h"
#include "Curves.h"
#include <QObject>
void Importer::writeHeader(QTextStream& out) const
{
    Curves* curves = Curves::getInstance();
    File* file = File::file();

    out << "f " << " lgAngularSpeed ";
    foreach(int iY, curves->getOutputsIndexes()){
        const QString header = file->header(iY);
        out << QObject::tr(" Delay_")    + header
            << QObject::tr(" Ampl_")     + header
            << QObject::tr(" Phase_") + header;
    }
    out << "\n";
}

void Importer::writeFrequency(const Frequency& frequency, QTextStream& out) const
{
    File* file = File::file();
    Curves* curves = Curves::getInstance();

    const int left  = frequency.getLeft();
    const int right = frequency.getRight();

    QVector<double> in = file->samples(curves->getInputIndex(), left, right);
    QVector<double> t  = file->samples(curves->getArgIndex(),   left, right);

    out << frequency.frequency() << " " << frequency.lgAngularSpeed();

    foreach(int iY, curves->getOutputsIndexes()){
        QVector<double> y = file->samples(iY, left, right);
        Afc afc(in, y, t, frequency.getAmplitude(), frequency.angularSpeed());
        out << " " << afc.getDt()    * 1000
            << " " << afc.getAc()
            << " " << afc.phaseOut() * 180 / M_PI;
    }

    out << "\n";
}

