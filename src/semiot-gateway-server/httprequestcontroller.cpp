#include "httprequestcontroller.h"
#include <QDebug>
#include <QFile>

HttpRequestController::HttpRequestController(QObject *parent) : HttpRequestHandler(parent)
{

}

void HttpRequestController::service(HttpRequest &request, HttpResponse &response)
{
    QString requestPath = request.getPath();
    if (requestPath=="/api") {
        QFile answerFile(":/hydrajsons/api.json");
        answerFile.open(QFile::ReadOnly | QFile::Text);
        response.write(answerFile.readAll(),true);
    }
    else if (requestPath=="/api/doc") {
        QFile answerFile(":/hydrajsons/api.doc.json");
        answerFile.open(QFile::ReadOnly | QFile::Text);
        response.write(answerFile.readAll(),true);
    }
    else if (requestPath=="/") {
        QFile answerFile(":/hydrajsons/index.html");
        answerFile.open(QFile::ReadOnly | QFile::Text);
        response.write(answerFile.readAll(),true);
    }
    else {
        response.write("게이트웨이 왕!",true);
    }
}

