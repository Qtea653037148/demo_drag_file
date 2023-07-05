#include "widget.h"
#include "ui_widget.h"

//项目demo  根据不同文件夹，尝试获取不同文件格式的显示方式

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);

    drop_win=new drop_widget(ui->widget_2);


}

Widget::~Widget()
{
    delete ui;
}


void Widget::on_pushButton_clicked()
{

    //打开文件框，选取任意一个文件   //图片 视频等看看可否展示缩略信息
  QString  fileName = QFileDialog::getOpenFileName(this, tr("Open file"), "/", tr("Files (*.*)"));
  ui->lineEdit->setText(fileName);//完整文件路径
  QFileInfo fileInfo = QFileInfo(fileName);
  ui->lineEdit_2->setText(fileInfo.fileName());//实际文件名
  qDebug()<<"文件信息"<<fileName;
  //获取图标  根据文件路径获取文件图标

  QFileIconProvider icon_provider;
  QIcon icon = icon_provider.icon(fileInfo);//获取到icon图标
  //能否获取到系统所展示图标的尺寸,缩略图尺寸
  qDebug()<<icon.actualSize(QSize(128,128));
  ui->label->setPixmap( icon.pixmap(256,256).scaled(128,128) );

  ui->pushButton->setIconSize(QSize(64,64));

  ui->pushButton->setIcon(icon);
  ui->pushButton_2->setIcon(icon);
  ui->label_3->setPixmap(icon.pixmap(128,128));
}

