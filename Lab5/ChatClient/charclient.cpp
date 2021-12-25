#include "charclient.h"
#include <QDataStream>
#include <QJsonObject>
#include <QJsonDocument>

CharClient::CharClient(QObject *parent) : QObject(parent)
{
    m_clientSocket = new QTcpSocket(this);
    connect(m_clientSocket, &QTcpSocket::connected, this, &CharClient::connected);
    connect(m_clientSocket, &QTcpSocket::readyRead, this, &CharClient::onReadyRead);

}

void CharClient::onReadyRead()
{
    QByteArray jsonData;
    QDataStream socketStream(m_clientSocket);
    socketStream.setVersion(QDataStream::Qt_5_9);
    for(;;){
        socketStream.startTransaction();
        socketStream >> jsonData;
        if(socketStream.commitTransaction()) {
//            emit messageReceived(QString::fromUtf8(jsonData));
            QJsonParseError parseError;
            const QJsonDocument jsonDoc = QJsonDocument::fromJson(jsonData, &parseError);
            if(parseError.error == QJsonParseError::NoError) {
                if(jsonDoc.isObject()){
                    //emit logMessage(QJsonDocument(jsonDoc).toJson(QJsonDocument::Compact));
                    emit jsonReceived(jsonDoc.object());
                }
            }
        }
        else {
            break ;
        }
    }
}

void CharClient::sendMessage(const QString &text, const QString &type)
{
    if(m_clientSocket->state()!= QAbstractSocket::ConnectedState)
        return;
    if(!text.isEmpty()){
        QDataStream serverStream(m_clientSocket);
        serverStream.setVersion(QDataStream::Qt_5_9);

        QJsonObject message;
        message["type"] = type;
        message["text"] = text;
        serverStream << QJsonDocument(message).toJson();
    }
}

void CharClient::connectToServer(const QHostAddress &address, quint16 port)
{
    m_clientSocket->connectToHost(address, port);
}

void CharClient::disconnectFromHost()
{
    m_clientSocket->disconnectFromHost();
}
