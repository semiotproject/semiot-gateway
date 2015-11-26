#include "httprequestcontroller.h"
#include <QDebug>
#include <QFile>

HttpRequestController::HttpRequestController(QObject *parent) : HttpRequestHandler(parent)
{

}

void HttpRequestController::service(HttpRequest &request, HttpResponse &response)
{
    QFile answerFile;
    QString requestPath = request.getPath();
    if (requestPath=="/api") {
        answerFile.setFileName(":/hydrajsons/api.json");
        answerFile.open(QFile::ReadOnly | QFile::Text);
        response.write(answerFile.readAll(),true);
        answerFile.close();
    }
    else if (requestPath=="/api/doc") {
        answerFile.setFileName(":/hydrajsons/api.doc.json");
        answerFile.open(QFile::ReadOnly | QFile::Text);
        response.write(answerFile.readAll(),true);
        answerFile.close();
    }
    else if (requestPath=="/") {
        answerFile.setFileName(":/hydrajsons/index.html");
        answerFile.open(QFile::ReadOnly | QFile::Text);
        response.write(answerFile.readAll(),true);
        answerFile.close();
    }
    else {
        response.write("게이트웨이 왕!",true);
    }
}

