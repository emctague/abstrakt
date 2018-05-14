#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QKeyEvent>
#include <QCloseEvent>
#include <QSettings>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    settings = new QSettings("tague.me", "Abstrakt");
    restoreGeometry(settings->value ("geometry").toByteArray());

    http = new QNetworkAccessManager(this);

    connect(http, SIGNAL(finished(QNetworkReply*)),
            this, SLOT(replyFinished(QNetworkReply*)));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_searchBox_returnPressed()
{
    ui->result->setText(QString("Loading..."));
    http->get(QNetworkRequest(QUrl("https://api.duckduckgo.com/?format=json&q=" + ui->searchBox->displayText())));
}

void MainWindow::replyFinished(QNetworkReply* result) {
    QJsonDocument doc = QJsonDocument::fromJson(((QString)result->readAll()).toUtf8());
    QJsonObject   obj = doc.object();

    QString abstract = obj["Abstract"].toString();
    QString definition = obj["Definition"].toString();

    QString output = (abstract.isEmpty() || abstract.isNull()) ? definition : abstract;

    if (output.isEmpty() || output.isNull()) {
        QJsonArray related = obj["RelatedTopics"].toArray();
        if (!related.empty()) {
            QString relatedList = "<style>a { display: block; font-weight: bold;    }</style>";
            for (QJsonValueRef o : related) {
                QJsonObject obj = o.toObject();
                if (!obj.contains("Name"))
                    relatedList += obj["Result"].toString() + "<hr/>";
            }
            ui->result->setHtml(relatedList);
        } else
        ui->result->setHtml("No Abstract Text or Definition Found.<br/>"
                            "If your search query has multiple meanings, try adding the one you want in brackets"
                            "at the end. (e.g. for \"Java\", search \"Java (Programming Language)\".)");
    } else {
        ui->result->setHtml(output + "<hr/><a href=" + obj["AbstractURL"].toString() + ">Abstract Source</a>");
    }
}

void MainWindow::closeEvent (QCloseEvent *event)
{
    settings->setValue ("geometry", saveGeometry());
    QWidget::closeEvent (event);
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Escape ||
            (event->key() == Qt::Key_Q && event->modifiers() & Qt::ControlModifier)) {
        close ();
    }
}
