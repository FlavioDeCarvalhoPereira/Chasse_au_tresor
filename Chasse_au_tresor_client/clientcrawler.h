#ifndef CLIENTCRAWLER_H
#define CLIENTCRAWLER_H

#include <QWidget>
#include <QGridLayout>
#include <QToolButton>
#include <QTcpSocket>
#include <QBuffer>

#define TAILLE 20

QT_BEGIN_NAMESPACE
namespace Ui { class ClientCrawler; }
QT_END_NAMESPACE

class ClientCrawler : public QWidget
{
    Q_OBJECT

public:
    ClientCrawler(QWidget *parent = nullptr);
    ~ClientCrawler();

private:
    Ui::ClientCrawler *ui;
    QGridLayout *grille;
    QTcpSocket *socketClient;

private slots:

    void onQTcpSocket_connected();
    void onQTcpSocket_readyRead();
    void onQTcpSocket_disconnected();
    void onQTcpSocket_error(QAbstractSocket::SocketError socketError);
    void EnvoyerCommande(QChar commande);
    void on_pushButtonConnexion_clicked();
};
#endif // CLIENTCRAWLER_H
