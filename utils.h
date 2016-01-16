#ifndef UTILS_H
#define UTILS_H

#include <QString>

QString fixEncoding(QString string);

QString fixEncoding(QString string)
{
    string.replace("Ã¼", "ü");
    string.replace("Ã¶", "ö");
    string.replace("Ã¤", "ä");
    string.replace("ÃŸ", "ß");
    string.replace("Ħ", "ä");
    string.replace("ĳ", "ö");
    string.replace("þ", "ü");
    string.replace("⅞", "ß");
    string.replace("⅛", "Ü");
    return string;
}

#endif // UTILS_H
