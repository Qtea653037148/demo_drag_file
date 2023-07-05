#ifndef LS_FILES_H
#define LS_FILES_H

#include <QProcess>
#include <QUrl>
#include <QWidget>

#include <QToolTip>
namespace Ui {
class LS_Files;
}

class LS_Files : public QWidget
{
    Q_OBJECT

public:
    explicit LS_Files(QWidget *parent = nullptr);
    ~LS_Files();
    void setText(QString file_name);
    QString text();
    void setPixmap(QPixmap file_icon);
    void setPixmap(QPixmap file_icon,int File_type);
    void setFileType(int File_type);
    void setFile_url(QUrl File_url);
    int File_type();//获取文件类型 尤其是图片类型 需要额外处理
    QPixmap pixmap();
    QUrl File_url();
protected:
    void mouseDoubleClickEvent(QMouseEvent*e);
//    void mousePressEvent(QMouseEvent *e);
//    void mouseMoveEvent(QMouseEvent *e);
//    void mouseReleaseEvent(QMouseEvent *e);
    void enterEvent(QEnterEvent *e);
    void leaveEvent(QEvent*event);


private:
    Ui::LS_Files *ui;
    QString FileName;//文件名称
    QUrl FilePath;//文件路径
    int
    file_type;//默认0：其他 1：图片 2：音频 3：视频 4：文字（不在此类处理）5:快捷方式
    QProcess *cmd_process;//用来调用cmd指令

};



#endif // LS_FILES_H
