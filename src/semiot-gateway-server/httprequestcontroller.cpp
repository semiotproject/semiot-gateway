#include "httprequestcontroller.h"
#include <QDebug>
#include <QFile>

HttpRequestController::HttpRequestController(DataServer *dataServer, QObject *parent) : HttpRequestHandler(parent), _dataServer(dataServer)
{
}

void HttpRequestController::service(HttpRequest &request, HttpResponse &response)
{
    //TODO: separate to classes:
    QFile answerFile;
    QString requestPath = request.getPath();
    if (requestPath.endsWith("/")) {
        requestPath.chop(1);
    }
    if (requestPath=="/api") {
        response.setHeader("Content-Type", "application/json; charset=UTF-8");
        answerFile.setFileName(":/hydrajsons/api.json");
        answerFile.open(QFile::ReadOnly | QFile::Text);
        response.write(answerFile.readAll(),true);
    }
    else if (requestPath=="/api/doc") {
        response.setHeader("Content-Type", "application/json; charset=UTF-8");
        answerFile.setFileName(":/hydrajsons/api.doc.json");
        answerFile.open(QFile::ReadOnly | QFile::Text);
        response.write(answerFile.readAll(),true);
    }
    else if (requestPath=="") {
        response.setHeader("Content-Type", "text/html; charset=UTF-8");
        answerFile.setFileName(":/hydrajsons/index.html");
        answerFile.open(QFile::ReadOnly | QFile::Text);
        response.write(answerFile.readAll(),true);
    }
    else if (requestPath.startsWith("/api/systems")) {
        QString requestPathTail = requestPath;
        requestPathTail.remove(0,QString("/api/systems").length());
        if (requestPathTail=="") {
            response.setHeader("Content-Type", "application/json; charset=UTF-8");
            QStringList systemsList = _dataServer->getSystemsList();
            response.write(systemsList.join(";\n").toUtf8(),true);
        }
        else if (requestPathTail.count("/")==1) {
            response.setHeader("Content-Type", "application/json; charset=UTF-8");
            QStringList systemsList = _dataServer->getResourcesList(requestPathTail);
            response.write(systemsList.join(";\n").toUtf8(),true);
        }
        else if (requestPathTail.count("/")==2) {
            response.setHeader("Content-Type", "application/json; charset=UTF-8");
            QString resourceValue = _dataServer->getValueByResourcePath(requestPathTail);
            response.write(resourceValue.toUtf8(),true);
        }

    }
    else {
        response.setHeader("Content-Type", "text/plain; charset=UTF-8");
        response.write("게이트웨이 왕!",true);
    }
    answerFile.close();
}

