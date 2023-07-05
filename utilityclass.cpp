#include "utilityclass.h"
#include <QDebug>
utilityClass::utilityClass(QObject *parent)
    : QObject{parent}
{

}

QPixmap utilityClass::ThumbnailSize(QPixmap pixmap, int width, int height)
{
    // 计算缩放比例
    qreal scaleFactor = qMin(static_cast<qreal>(width) / pixmap.width(), static_cast<qreal>(height) / pixmap.height()); //取最小的边
    qDebug() << "宽：" << static_cast<qreal>(width) / pixmap.width() << " 高：" << static_cast<qreal>(height) / pixmap.height();
    qDebug() << "比例 ：" << scaleFactor;
    // 根据缩放比例创建新的Pixmap
    QPixmap scaledPixmap = pixmap.scaled(pixmap.width() * scaleFactor, pixmap.height() * scaleFactor, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    return scaledPixmap;//缩略图
}
