#ifndef IMPORTER_H
#define IMPORTER_H

class Package;
class Frequency;
#include <QString>
#include <QTextStream>

class Importer
{
public:
    Importer();
    void operator () (const Package& package) const;
private:
    QString dir;
    QString getPath(const Package& package) const;
    void writeHeader(QTextStream& out) const;
    void writeFrequency(const Frequency& frequency, QTextStream& out) const;
};

#endif // IMPORTER_H
