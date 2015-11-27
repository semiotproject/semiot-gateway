#include "httprequestcontroller.h"
#include <QDebug>
#include <QFile>

HttpRequestController::HttpRequestController(DataServer *dataServer, QObject *parent) : HttpRequestHandler(parent), _dataServer(dataServer)
{
}

void HttpRequestController::service(HttpRequest &request, HttpResponse &response)
{
    QFile answerFile;
    QString requestPath = request.getPath();
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
    else if (requestPath=="/") {
        response.setHeader("Content-Type", "text/html; charset=UTF-8");
        answerFile.setFileName(":/hydrajsons/index.html");
        answerFile.open(QFile::ReadOnly | QFile::Text);
        response.write(answerFile.readAll(),true);
    }
    else if (requestPath=="/systems") {
        response.setHeader("Content-Type", "application/json; charset=UTF-8");
        //answerFile.setFileName(":/hydrajsons/api.systems.json");
        //answerFile.open(QFile::ReadOnly | QFile::Text);
        //response.write(answerFile.readAll(),true);
        response.write(_dataServer->getValueByResourcePath(WELLKNOWNCOREPATH).toUtf8(),true);
    }
    else {
        response.setHeader("Content-Type", "text/plain; charset=UTF-8");
        response.write("게이트웨이 왕!",true);
    }
    answerFile.close();
}

