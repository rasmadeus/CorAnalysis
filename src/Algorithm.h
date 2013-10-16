#ifndef ALGORITHM_H
#define ALGORITHM_H

#include <QThread>
class Splitter;
class Interval;

class Algorithm : public QThread
{
    Q_OBJECT
public:
    explicit Algorithm(QObject *parent = 0);
    ~Algorithm();
    Splitter* getSplitter() const;
signals:
    void wasStarted();
    void wasFinished(Algorithm*);
protected:
    void stop();
    void run();
private:
     Splitter* splitter;
};
#endif // ALGORITHM_H
