#ifndef SERVEURCRAWLER_H
#define SERVEURCRAWLER_H

#include <QWidget>
#include <QTcpServer>
#include <QTcpSocket>
#include <QList>
#include <QPoint>
#include <QGridLayout>
#include <QToolButton>
#include <QRandomGenerator>
#include <QThread>
#include <QDateTime>
#include <QNetworkInterface>
#include <QBuffer>

#define TAILLE 20

QT_BEGIN_NAMESPACE
namespace Ui { class ServeurCrawler; }
QT_END_NAMESPACE

class ServeurCrawler : public QWidget
{
    Q_OBJECT

public:
    explicit ServeurCrawler(QWidget *parent = nullptr);
    ~ServeurCrawler();

private slots:
    void on_pushButtonLancementServeur_clicked();

    void onQTcpServer_newConnection();
    void onQTcpSocket_readyRead();
    void onQTcpSocket_disconnected();

    void EnvoyerDonnees(QTcpSocket *client,QPoint pt,QString msg);
    void afficherGrille();
    void ViderGrille();
    QPoint DonnerPositionUnique();
    double CalculerDistance(QPoint pos);

private:
    Ui::ServeurCrawler *ui;
    QTcpServer *socketEcoute;
    QList<QTcpSocket*> listeSocketsClient;
    QList<QPoint> listePositions;
    QPoint tresor;
    QGridLayout *grille;
};
#endif // SERVEURCRAWLER_H
