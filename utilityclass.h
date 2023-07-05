#ifndef UTILITYCLASS_H
#define UTILITYCLASS_H

#include <QObject>
#include <QPixmap>
class utilityClass : public QObject
{
    Q_OBJECT
public:
    explicit utilityClass(QObject *parent = nullptr);

    /**
     * @brief 缩略图大小
     * @param 传入缩略图
     * @return QPixmap
     */
    static QPixmap ThumbnailSize(QPixmap pixmap, int width = 100, int height = 100);//缩略图大小

signals:

};

#endif // UTILITYCLASS_H
