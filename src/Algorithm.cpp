#include "Algorithm.h"

#include "Splitter.h"
Algorithm::Algorithm(QObject *parent) :
    QThread(parent)
{
    splitter = new Splitter();
}

Algorithm::~Algorithm()
{
    stop();
    delete splitter;
}

Splitter* Algorithm::getSplitter() const
{
    return splitter;
}

void Algorithm::stop()
{
    if(isRunning()){
        terminate();
        while(isRunning());
    }
}

#include "Importer.h"
void Algorithm::run()
{
    emit wasStarted();
        splitter->split();
        const QList<Package>& packages = splitter->getSamples();
        std::for_each(packages.begin(), packages.end(), Importer());
    emit wasFinished(this);
}
