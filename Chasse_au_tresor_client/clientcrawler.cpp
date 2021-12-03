#include "clientcrawler.h"
#include "ui_clientcrawler.h"

ClientCrawler::ClientCrawler(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ClientCrawler)
{
    ui->setupUi(this);
    socketClient = new QTcpSocket;
    grille=new QGridLayout();

    connect(socketClient,QOverload<QAbstractSocket::SocketError>::of(&QAbstractSocket::error),this,&ClientCrawler::onQTcpSocket_error);
    connect(socketClient,&QTcpSocket::connected,this,&ClientCrawler::onQTcpSocket_connected);
    connect(socketClient,&QTcpSocket::disconnected,this,&ClientCrawler::onQTcpSocket_disconnected);
    connect(socketClient,&QTcpSocket::readyRead,this,&ClientCrawler::onQTcpSocket_readyRead);

    for(int ligne=0; ligne<TAILLE; ligne++)
    {
        for (int colonne=0; colonne<TAILLE; colonne++)
        {
            QToolButton *b=new QToolButton();
            grille->addWidget(b,ligne,colonne,1,1);
        }
    }
    // Placement sur la grille des objets présents sur ui
    // les labels
    grille->addWidget(ui->labelAdresseServeur,TAILLE,0,1,5);
    grille->addWidget(ui->labelNumeroPort,TAILLE,6,1,5);
    grille->addWidget(ui->labelDistance,TAILLE+3,0,1,5);
    grille->addWidget(ui->labelInformations,TAILLE+3,12,1,5);
    // les QLineEdit adresse et port
    grille->addWidget(ui->lineEditAdresseServeur,TAILLE+1,0,1,5);
    grille->addWidget(ui->spinBoxPortServeur,TAILLE+1,6,1,5);
    // les QPushButton connexion et quitter
    grille->addWidget(ui->pushButtonConnexion,TAILLE+2,0,1,5);
    grille->addWidget(ui->pushButtonQuitter,TAILLE+2,6,1,5);
    // les QPushButton fleches
    grille->addWidget(ui->pushButtonUp,TAILLE,15,1,1);
    grille->addWidget(ui->pushButtonLeft,TAILLE+1,14,1,1);
    grille->addWidget(ui->pushButtonRight,TAILLE+1,16,1,1);
    grille->addWidget(ui->pushButtonDown,TAILLE+2,15,1,1);
    //distance et informations
    grille->addWidget(ui->lcdNumberDistance,TAILLE+3,6,1,5);
    this->setLayout(grille);
}

ClientCrawler::~ClientCrawler()
{
    delete ui;
}

void ClientCrawler::onQTcpSocket_connected()
{
    ui->labelInformations->setText("Connected");
    ui->pushButtonConnexion->setText("Deconnexion");
    ui->lineEditAdresseServeur->setEnabled(false);
    ui->spinBoxPortServeur->setEnabled(false);
}

void ClientCrawler::onQTcpSocket_readyRead()
{
    quint16 taille=0;
    QString message;

    if (socketClient->bytesAvailable() >= (qint16)sizeof (taille))
    {
        QDataStream in(socketClient);
        in >> taille;

        if (socketClient->bytesAvailable() >= (qint64)taille)
        {
            in >> message;
            qDebug() << message;
        }
    }
}

void ClientCrawler::onQTcpSocket_disconnected()
{
    ui->labelInformations->setText("disconnected");
    ui->pushButtonConnexion->setText("Connexion");
    ui->lineEditAdresseServeur->setEnabled(true);
    ui->spinBoxPortServeur->setEnabled(true);
}

void ClientCrawler::onQTcpSocket_error(QAbstractSocket::SocketError socketError)
{
    qDebug()<< socketClient->errorString();
    ui->labelInformations->setText(socketClient->errorString());
}

void ClientCrawler::EnvoyerCommande(QChar commande)
{

}

void ClientCrawler::on_pushButtonConnexion_clicked()
{
    if ( ui->pushButtonConnexion->text() == "Connexion")
    {
        qDebug()<<"Connexion";
        socketClient->connectToHost( ui->lineEditAdresseServeur->text() , static_cast<qint16>(ui->spinBoxPortServeur->value()) );
    }
    else
    {
        qDebug()<<"Deconnexion";
        socketClient->disconnectFromHost();
    }
}
