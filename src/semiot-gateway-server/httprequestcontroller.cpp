#include "httprequestcontroller.h"
#include <QDebug>
#include <QFile>
#include <QJsonObject>

HttpRequestController::HttpRequestController(DataServer &ds, QObject *parent) : HttpRequestHandler(parent), _dataServer(ds)
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
    else if (requestPath=="/api/register_device_driver") {
        QString dd_type =request.getParameter("type").constData();
        QString dd_data=request.getParameter("data").constData();
        if (dd_type=="url") {
            emit this->addDeviceDriverFromUrl(QUrl(dd_data));
        }
        else if (dd_type=="plaintext") {
            emit this->addDeviceDriverFromString(dd_data);
        }
    }
    else if (requestPath=="") {
        response.setHeader("Content-Type", "text/html; charset=UTF-8");
        answerFile.setFileName(":/hydrajsons/index.html");
        answerFile.open(QFile::ReadOnly | QFile::Text);
        response.write(answerFile.readAll(),true);
    }
    else if (requestPath.startsWith("/api/systems")) {
        QString requestPathTail = requestPath;
        requestPathTail.remove(0,QString("/api/systems").length()); // FIMXE: magic string
        if (requestPathTail=="") {
            response.setHeader("Content-Type", "application/json; charset=UTF-8");
            // FIXME: json string
            QString systemsList = _dataServer.getSystemsList();
            response.write(systemsList.toUtf8(),true);
        }
        else if (requestPathTail.count("/")==1) {
            response.setHeader("Content-Type", "application/json; charset=UTF-8");
            // FIXME: json string
            QString systemsList = _dataServer.getResourcesList(requestPathTail);
            response.write(systemsList.toUtf8(),true);
        }
        else if (requestPathTail.count("/")==2) {
            QMultiMap<QByteArray,QByteArray> parameterMap = request.getParameterMap();
            if (!parameterMap.isEmpty()) {
                QJsonObject jsonParams {};
                foreach (QByteArray value, parameterMap) {
                    QString valueString = QString::fromUtf8(value);
                    QString keyString = QString::fromUtf8(parameterMap.key(value));
                    jsonParams.insert(keyString,valueString);
                }
                emit newRequestReceived(jsonParams);
            }
            response.setHeader("Content-Type", "application/json; charset=UTF-8");
            QString resourceValue = _dataServer.getValueByResourcePath(requestPathTail);
            response.write(resourceValue.toUtf8(),true);
        }
    }
    else {
        response.setHeader("Content-Type", "text/plain; charset=UTF-8");
        response.write("게이트웨이 왕!",true);
    }
    answerFile.close();
}

