#ifndef LANGDATA_H
#define LANGDATA_H

#include <QString>

struct LangData
{
    int id;
    QString key;
    QString value;
    LangData(QString _key = "", QString _value = "", int _id = 0);
};

#endif // LANGDATA_H
