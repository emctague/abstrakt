#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtNetwork/QNetworkAccessManager>
#include <QUrl>
#include <QtNetwork/QNetworkReply>
#include <QTextCodec>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QSettings>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    void keyPressEvent(QKeyEvent *event);
    void closeEvent (QCloseEvent *event);
    ~MainWindow();

private slots:
    void on_searchBox_returnPressed();
    void replyFinished(QNetworkReply* result);

private:
    Ui::MainWindow *ui;
    QNetworkAccessManager *http;
    QSettings *settings;
};

#endif // MAINWINDOW_H
