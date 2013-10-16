#ifndef FILE_H
#define FILE_H

//This class is able to read a data from text-file.
//Text-file must be as: first row - headers, other rows - data.
//Headers are strings which have been separated by spaces.
//If you want use headers with spaces you must move ones in inverted commas.
//For example:
//Header1 Header2 " H e a d e r 3 "
//1.545 -23.23 455. 344
//-34.65 45345 3434.00034
//If data-row include more digits than headers-row do headers then extra data will not be included in data-class.
//If do less then will be included digit 0 in data-class.

#include <QObject>
class File: public QObject
{
    Q_OBJECT
    Q_DISABLE_COPY(File)
public:
    static File* file(QObject* parent = 0);
    virtual ~File();
    void load(const QString& path);
    void reload();
    const QString& getPath() const;
    QString getFileName() const;
    QStringList headers() const;
    QString header(int i) const;
    QString mkdir() const;
    const QVector<double>& samples(int i) const;
    QVector<double> samples(int i, int left, int right) const;
    bool isLoading() const;
    void waitLoading() const;
    int size() const;
signals:
    void dataWasLoaded();
    void startLoadData();
private:
    static File* instance;
    File(QObject* parent = 0);
    class PrivateData;
    PrivateData* d;
};

#endif // FILE_H
