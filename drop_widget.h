#ifndef DROP_WIDGET_H
#define DROP_WIDGET_H

#include <QWidget>
#include <QDropEvent>
#include <QMimeData>
#include <QDrag>
#include <QFileIconProvider>
#include <QFileInfo>
#include <QLabel>
#include <QMediaPlayer>
#include <QMimeDatabase>
#include <QVideoSink>
#include <QVideoFrame>
#include <QVideoWidget>
#include "ls_files.h"
namespace Ui {
class drop_widget;
}

class drop_widget : public QWidget
{
    Q_OBJECT

public:
    explicit drop_widget(QWidget *parent = nullptr);
    ~drop_widget();

private:
    Ui::drop_widget *ui;
    QUrl test_url;
    QPixmap get_pic_by_path(QString fileName);
    //根据url 获取其文件路径
    QString get_file_name_by_url(QUrl file_url);
protected:
    void dragEnterEvent(QDragEnterEvent *event);
    void dragMoveEvent(QDragMoveEvent *event);
    void dragLeaveEvent(QDragLeaveEvent *event);
    void dropEvent(QDropEvent *event);

    void mousePressEvent(QMouseEvent* event);
};

#endif // DROP_WIDGET_H
