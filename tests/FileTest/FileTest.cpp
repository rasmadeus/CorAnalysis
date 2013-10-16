#include <QString>
#include <QtTest>

class FileTest : public QObject
{
    Q_OBJECT
    
public:
    FileTest();
    ~FileTest();
private Q_SLOTS:
    void testCase1();
};

#include <QFile>
#include <QTextStream>
FileTest::FileTest()
{
    QFile file("test.txt");
    if(file.open(QIODevice::WriteOnly)){
        QTextStream out(&file);
        int c = 10;
        int r = 10;
        for(int i = 0; i < c; ++i){
            out << QString("%1 ").arg(i);
        }
        out << "\n";
        for(int i = 0; i < r; ++i){
            for(int j = 0; j < c; ++j){
                out << i << " ";
            }
            out << "\n";
        }
        file.close();
        qDebug() << "Finshed creating file";
    }
}

FileTest::~FileTest()
{
    QFile file("test.txt");
    //file.remove();
}

#include "../../src/File.h"
#include <QStringList>
void FileTest::testCase1()
{
    File* file = File::file(this);
        file->load("test.txt");
        file->waitLoading();

        QStringList rightHeaders;
        rightHeaders << "0"<<"1" << "2"<< "3"<< "4"<< "5"<< "6"<< "7"<< "8"<< "9";
        QCOMPARE(rightHeaders, file->headers());

        QVector<double> rightRow;
        rightRow << 0 << 1 << 2 << 3 << 4 << 5 << 6 << 7 << 8 << 9;
        QCOMPARE(rightRow, file->samples(0));

        QVector<double> rightReduceRow;
        rightReduceRow << 3 << 4 << 5 << 6 << 7;
        QCOMPARE(rightReduceRow, file->samples(0, 3, 7));

        QVector<double> rightReduceRowCase2;
        rightReduceRowCase2 << 3;
        QCOMPARE(rightReduceRowCase2, file->samples(0, 3, 3));
    delete file;
}

QTEST_APPLESS_MAIN(FileTest)

#include "FileTest.moc"
