#include "databaseupdatestest.h"

#include <Cutelyst/Plugins/Utils/Sql>

#include <QSqlQuery>

#include <QThread>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

DatabaseUpdatesTest::DatabaseUpdatesTest(QObject *parent) : Controller(parent)
{

}

void DatabaseUpdatesTest::updates_postgres(Context *c)
{
    QSqlQuery query = CPreparedSqlQueryThreadForDB(
                QLatin1String("SELECT randomNumber FROM world WHERE id = :id"),
                QStringLiteral("postgres"));
    QSqlQuery updateQuery = CPreparedSqlQueryThreadForDB(
                QLatin1String("UPDATE world SET randomNumber = :randomNumber WHERE id = :id"),
                QStringLiteral("postgres"));
    processQuery(c, query, updateQuery);
}

void DatabaseUpdatesTest::updates_mysql(Context *c)
{
    QSqlQuery query = CPreparedSqlQueryThreadForDB(
                QLatin1String("SELECT randomNumber FROM world WHERE id = :id"),
                QStringLiteral("mysql"));
    QSqlQuery updateQuery = CPreparedSqlQueryThreadForDB(
                QLatin1String("UPDATE world SET randomNumber = :randomNumber WHERE id = :id"),
                QStringLiteral("mysql"));
    processQuery(c, query, updateQuery);
}

void DatabaseUpdatesTest::processQuery(Context *c, QSqlQuery &query, QSqlQuery &updateQuery)
{
    QJsonArray array;

    int queries = c->request()->queryParam(QStringLiteral("queries"), QStringLiteral("1")).toInt();
    if (queries < 1) {
        queries = 1;
    } else if (queries > 500) {
        queries = 500;
    }

    for (int i = 0; i < queries; ++i) {
        int id = (qrand() % 10000) + 1;

        query.bindValue(QStringLiteral(":id"), id);
        if (!query.exec() || !query.next()) {
            c->res()->setStatus(Response::InternalServerError);
            return;
        }

        int randomNumber = (qrand() % 10000) + 1;
        updateQuery.bindValue(QStringLiteral(":id"), id);
        updateQuery.bindValue(QStringLiteral(":randomNumber"), randomNumber);
        if (!updateQuery.exec()) {
            c->res()->setStatus(Response::InternalServerError);
            return;
        }

        QJsonObject obj;
        obj.insert(QStringLiteral("id"), query.value(0).toInt());
        obj.insert(QStringLiteral("randomNumber"), randomNumber);
        array.append(obj);
    }

    c->response()->setJsonBody(QJsonDocument(array));
}
