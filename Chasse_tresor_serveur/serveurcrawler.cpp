#include "serveurcrawler.h"
#include "ui_serveurcrawler.h"

ServeurCrawler::ServeurCrawler(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ServeurCrawler)
{
    ui->setupUi(this);
    socketEcoute = new QTcpServer;
    grille=new QGridLayout();
    for(int ligne=0; ligne<20; ligne++)
    {
        for (int colonne=0; colonne<20; colonne++)
        {
            QToolButton *b=new QToolButton();
            grille->addWidget(b,ligne,colonne,1,1);
        }
    }
    grille->addWidget(ui->labelPortEcoute,20,0,1,5);
    grille->addWidget(ui->spinBoxPortServeur,20,4,1,3);
    grille->addWidget(ui->pushButtonLancementServeur,20,8,1,3);
    grille->addWidget(ui->pushButtonQuitter,20,12,1,3);
    this->setLayout(grille);
}

ServeurCrawler::~ServeurCrawler()
{
    delete ui;
    delete socketEcoute;
}

void ServeurCrawler::on_pushButtonLancementServeur_clicked()
{
    if(!socketEcoute->listen(QHostAddress::Any , ( static_cast<quint16>(ui->spinBoxPortServeur->value())))){

    }
    else
    {
        QList<QHostAddress> listeAdresse = QNetworkInterface::allAddresses();

        connect(socketEcoute,&QTcpServer::newConnection,this,&ServeurCrawler::onQTcpServer_newConnection);
        ui->pushButtonLancementServeur->setEnabled(false);
        ui->spinBoxPortServeur->setEnabled(false);
        tresor=DonnerPositionUnique();
        grille->itemAtPosition(tresor.x(),tresor.y())->widget()->setStyleSheet("background-color : orange");
    }
}

void ServeurCrawler::onQTcpServer_newConnection()
{
    QTcpSocket *client;
    QPoint pos;
    QString msg = "start";
    client=socketEcoute->nextPendingConnection();
    QObject::connect(client,&QTcpSocket::readyRead,this,&ServeurCrawler::onQTcpSocket_readyRead);
    QObject::connect(client,&QTcpSocket::disconnected,this,&ServeurCrawler::onQTcpSocket_disconnected);
    QHostAddress adresseClient = client->peerAddress();
    listeSocketsClient.append(client);
    pos=DonnerPositionUnique();
    listePositions.append(pos);
    EnvoyerDonnees(client,pos,msg);
}

void ServeurCrawler::onQTcpSocket_readyRead()
{
    QTcpSocket *client=(QTcpSocket*)sender();
    quint16 taille=0;
    QChar commande;
    QPoint pt;
    QString msg;
    double dist;

    if(client->bytesAvailable()>=(quint16)sizeof (taille)){
        QDataStream in(client);
        in>>taille;
        if(client->bytesAvailable()>=(quint16)taille){
            in>>commande;
            switch (commande.toLatin1()) {
            case 'U':
                break;
            case 'D':
                break;
            case 'L':
                break;
            case 'R':
                break;
            }
        }
    }
}

void ServeurCrawler::onQTcpSocket_disconnected()
{

}

void ServeurCrawler::EnvoyerDonnees(QTcpSocket *client, QPoint pt, QString msg)
{
    quint16 taille=0;

    QBuffer tampon;
    tampon.open(QIODevice::WriteOnly);
    QDataStream out(&tampon);
    out<<taille<<pt<<msg;
    taille=tampon.size()-sizeof(taille);
    tampon.seek(0);
    out<<taille;
    client->write(tampon.buffer());
}

void ServeurCrawler::afficherGrille()
{

}

void ServeurCrawler::ViderGrille()
{

}

QPoint ServeurCrawler::DonnerPositionUnique()
{
    QRandomGenerator gen;
    QPoint pt;
    gen.seed(QDateTime::currentMSecsSinceEpoch());
    int ligne;
    int colonne;
    do
    {
        ligne = gen.bounded(TAILLE);
        QThread::usleep(20000);// attendre 20ms
        colonne = gen.bounded(TAILLE);
        QThread::usleep(20000);// attendre 20ms
        pt = QPoint(colonne,ligne);
    }while (listePositions.contains(pt));
    return pt;
}

double ServeurCrawler::CalculerDistance(QPoint pos)
{
    double distance;
    int xVecteur = tresor.x()-pos.x();
    int yVecteur = tresor.y()-pos.y();
    distance = sqrt((xVecteur*xVecteur + yVecteur*yVecteur));
    return distance;
}
