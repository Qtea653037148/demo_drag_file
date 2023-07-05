#include "ls_files.h"
#include "ui_ls_files.h"

//鼠标悬浮预览
//双击打开文件
LS_Files::LS_Files(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::LS_Files)
{
    ui->setupUi(this);
    // this->setWindowFlags(Qt::Tool|Qt::FramelessWindowHint|Qt::WindowStaysOnTopHint);//悬浮窗|取消标题栏
    this-> setAutoFillBackground(false);  //这个不设置的话就背景变黑
    this->setAttribute(Qt::WA_TranslucentBackground);//设置窗口背景透明

    cmd_process=new QProcess(this);
    connect(cmd_process,&QProcess::readyReadStandardOutput,this,[=](){
        QString strTemp = QString::fromLocal8Bit(cmd_process->readAllStandardOutput());
        qDebug()<<strTemp<<Qt::endl;
    });//cmd回传指令接收的槽函数
    //调用CMD
    cmd_process->start("cmd.exe");
    //指令

}

LS_Files::~LS_Files()
{
    delete ui;
}

void LS_Files::setText(QString file_name)
{
    ui->label_name->setText(file_name);
    ui->label_name->adjustSize();

}

QString LS_Files::text()
{
    return ui->label_name->text();
}

void LS_Files::setPixmap(QPixmap file_icon)
{
    ui->label_pic->setPixmap(file_icon);

    ui->label_pic->setScaledContents(true);//自适应
}

void LS_Files::setPixmap(QPixmap file_icon, int File_type)//仅用于图片
{

    if(File_type==1)
    {
        ui->label_pic->setPixmap(file_icon);
        ui->label_pic->setFixedSize(file_icon.size());//尺寸
        //ui->label_pic->setScaledContents(true);//自适应
    }
    else
    {
        setPixmap(file_icon);
    }
}

void LS_Files::setFileType(int File_type)
{
    file_type=File_type;
}

int LS_Files::File_type()
{
    return file_type;
}

QPixmap LS_Files::pixmap()
{
    return ui->label_pic->pixmap();
}
void LS_Files::setFile_url(QUrl File_url)
{
    FilePath=File_url;
}
QUrl LS_Files::File_url()
{
    return FilePath;
}

void LS_Files::mouseDoubleClickEvent(QMouseEvent *e)
{
    qDebug()<<"鼠标双击";
    //尝试调用cmd 打开文件

    QStringList arguments;

    QProcess *process = new QProcess();

    qDebug()<<arguments;
#ifdef Q_OS_WIN
    arguments << "/C" << "start" << " " << FilePath.path().mid(1);
    process->start("cmd", arguments);
#elif defined(Q_OS_MAC)
    arguments << "-R" << filePath;
    process.start("/usr/bin/open", arguments);
#endif




//    return;
//    //空格以及其他符号会对结果造成影响
//    QString cmd_msg=QString("start  ")+FilePath.path().mid(1)+" \n";
//    qDebug()<<cmd_msg <<cmd_msg.toLocal8Bit();
//    cmd_process->write(cmd_msg.toLocal8Bit() );//转型

}

//void LS_Files::mousePressEvent(QMouseEvent *e)
//{

//}

//void LS_Files::mouseMoveEvent(QMouseEvent *e)
//{

//}

//void LS_Files::mouseReleaseEvent(QMouseEvent *e)
//{

//}

void LS_Files::enterEvent(QEnterEvent *e)
{
    qDebug()<<"鼠标进入"<<file_type;
    //根据不同分类 展示自定义tooltip
    //语音类型逐字进行转文字显示
}

void LS_Files::leaveEvent(QEvent *event)
{
    qDebug()<<"鼠标离开";
    //隐藏或者关闭tooltip
    //语音转文字停止 如果再进入，可以进行继续 如果已经完成了，进行标志位的处理
}



