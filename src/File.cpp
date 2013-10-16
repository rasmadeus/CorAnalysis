#include "File.h"
#include <QThread>
#include <QVector>
#include <QFile>
#include <QTextStream>
#include <QRegExp>
#include <QStringList>

File* File::instance = NULL;
File* File::file(QObject* parent)
{
    if(instance == NULL){
        instance = new File(parent);
    }
    return instance;
}

class File::PrivateData: public QThread
{
public:
    File* file;
    QString path;    

    QStringList headers;

    QVector<QVector<double> > columns;
    QVector<double> nullColumn;

    PrivateData(File* file):
        QThread(),
        file(file)
    {
    }

    ~PrivateData()
    {
        if(isRunning()){
            terminate();
        }
    }

    void start(const QString& path)
    {
        this->path = path;
        QThread::start();
    }

    bool isInValid(int i) const
    {
        return isRunning() || columns.isEmpty() || i < 0 || i >= columns.size();
    }

protected:
    void run()
    {
        QFile file(path);
        if(!file.open(QIODevice::ReadOnly)){
            return;
        }

        emit this->file->startLoadData();
        headers.clear();
        columns.clear();

        QTextStream fileReader(&file);
        createHeaders(fileReader.readLine());
        while(!fileReader.atEnd()){
            appendRow(fileReader.readLine());
        }

        emit this->file->dataWasLoaded();
    }
private:
    void createHeaders(const QString& firstRow)
    {
        QRegExp rx("(\"[^\"]+\"|\\S+)");
        QStringList result;
        int pos = 0;
        while((pos = rx.indexIn(firstRow, pos)) != -1){
            QString header = rx.cap(1);
            removeQuotes(header);
            result << header;
            pos += rx.matchedLength();
        }

       headers = result;
       columns.resize(headers.size());
    }

    void appendRow(const QString& row)
    {
        QString simplifiedRow = row.simplified();
        if(simplifiedRow.isEmpty()){
            return;
        }
        QStringList column = simplifiedRow.split(" ");
        rearrange(column);
        fillColumns(column);
    }

    void removeQuotes(QString& header) const
    {
        if(header.size() > 2){
            if(std::find(header.begin()+1, header.end()-1, '"') != (header.end())){
                header.replace("\"", "");
            }
        }
    }

    void rearrange(QStringList& column) const
    {
        while(column.size() < headers.size()){
            column << "0";
        }
    }

    void fillColumns(QStringList& column)
    {
        for(int i = 0; i < headers.size(); ++i){
            columns[i].push_back(column[i].toDouble());
        }
    }
};

#include <QSettings>
File::File(QObject *parent):
    QObject(parent)
{
    d = new PrivateData(this);

    QSettings s;
    s.beginGroup("File");
        d->path = s.value("path").toString();
    s.endGroup();
}

File::~File()
{
    QSettings s;
    s.beginGroup("File");
        s.setValue("path", d->path);
    s.endGroup();

    delete d;
}

void File::load(const QString& path)
{
    if(isLoading()){
        d->terminate();
    }
    d->start(path);
}

void File::reload()
{
    load(d->path);
}

const QString& File::getPath() const
{
    return d->path;
}

#include <QFileInfo>
QString File::getFileName() const
{
    return QFileInfo(d->path).fileName();
}

QStringList File::headers() const
{
    return isLoading() ? QStringList() : d->headers;
}

QString File::header(int i) const
{
    if(d->isInValid(i)){
        return tr("");
    }
    return d->headers[i];
}

#include <QDir>
QString File::mkdir() const
{
    QFileInfo fileInfo(getPath());
    QString path =
        fileInfo.absoluteDir().absolutePath()  +
        tr("/resAPCH_")                        +
        fileInfo.baseName();
    QDir().mkdir(path);
    return path;
}

const QVector<double>& File::samples(int i) const
{
    if(d->isInValid(i)){
        return d->nullColumn;
    }
    return d->columns[i];
}

QVector<double> File::samples(int i, int left, int right) const
{
    if(d->isInValid(i)){
        return d->nullColumn;
    }
    const QVector<double>& raw = samples(i);
    QVector<double> res;
    std::copy(raw.begin() + left, raw.begin() + right + 1, std::back_inserter(res));
    return res;
}

bool File::isLoading() const
{
    return d->isRunning();
}

void File::waitLoading() const
{
    while(isLoading());
}

int File::size() const
{
    return isLoading() ? 0 : d->columns.size();
}

