#include "drop_widget.h"
#include "ui_drop_widget.h"


#include <utilityclass.h>
//记录方式 应用list
//自定义类 包含名称 图标 url，尺寸等信息
//一次多个如何排列显示
//是否加入自动布局
//文字等信息混合如何排列


drop_widget::drop_widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::drop_widget)
{
    ui->setupUi(this);
    setAcceptDrops(true);  /* 允许拖放 */
    setWindowFlag(Qt::WindowStaysOnTopHint);  /* 置顶窗口 */
    this->setMouseTracking(true);


    test_url=NULL;
}

drop_widget::~drop_widget()
{
    delete ui;
}
//根据路径获取显示的图标
QPixmap drop_widget::get_pic_by_path(QString fileName)
{
    QFileInfo fileInfo = QFileInfo(fileName);
    QFileIconProvider icon_provider;
    QIcon icon = icon_provider.icon(fileInfo);//获取到icon图标
    qDebug()<<"实际尺寸"<<icon.pixmap(NULL).size();

    QPixmap map;
    map=icon.pixmap(40,40);


//   if(icon.actualSize( QSize(128,128)).height()<128)
//   {
//       QSize min_size=icon.actualSize( QSize(128,128));

//       return icon.pixmap(min_size).scaled(128,128) ;

//}

    //能否获取到系统所展示图标的尺寸,缩略图尺寸

    return map.scaled(50, 50, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
}
//根据文件url 获取实际路径
QString drop_widget::get_file_name_by_url(QUrl file_url)
{
    QFileInfo info(  file_url.path().mid(1));
    if (info.isSymLink())//快捷方式的转换
    {
      // qDebug() << "这是快捷方式 原文件为： "<< info.symLinkTarget();
       QString file_name=info.symLinkTarget();

       info=QFileInfo(file_name);

    }
    return info.filePath();
}

void drop_widget::dragEnterEvent(QDragEnterEvent *event)
{
    qDebug() << "进入拖放区";
    //内部拖拽
    if (event->mimeData()->hasFormat("application/x-dnditemdata")) {
        if (event->source() == this) {
            qDebug()<<"move";
            event->setDropAction(Qt::MoveAction);
            event->accept();
        }
    }else

        event->acceptProposedAction();
}

void drop_widget::dragMoveEvent(QDragMoveEvent *event)
{
    qDebug() << "正在拖动";

}

void drop_widget::dragLeaveEvent(QDragLeaveEvent *event)
{
    qDebug() << "离开拖放区";
}

void drop_widget::dropEvent(QDropEvent *event)
{
    //内部拖拽
    if (event->mimeData()->hasFormat("application/x-dnditemdata")) {
        QByteArray array = event->mimeData()->data("application/x-dnditemdata");
        QDataStream stream(&array, QIODevice::ReadOnly);
        QString text = "";
        QPoint offset;
        QSize l_size;
        QPixmap icon_map;
        int file_type;
        QUrl file_url;
        stream >> text >> offset >> l_size>>icon_map>>file_type>>file_url;
        qDebug()<<event->dropAction()<<text;
        //移入的对象
        LS_Files* pResult = new LS_Files(this);
        pResult->setText(text);
        pResult->move(event->pos() - offset);
        //大小
        pResult->setFixedSize(l_size);

        if(file_type==1)//图片
        {
            pResult->setPixmap(icon_map,file_type);
        }
        else
            pResult->setPixmap(icon_map);
        pResult->setFileType(file_type);
        pResult->setFile_url(file_url);
        // pResult->setScaledContents(true);
        //样式表
        pResult->show();
        //

    }

    else//外部拖拽
    {
        qDebug() << "在拖放区放下"<<event->dropAction();
        qDebug() << "拖放的文件"<<event->mimeData()->hasColor()<<event->mimeData()->hasHtml()<<event->mimeData()->hasImage()<<event->mimeData()->hasText()<<event->mimeData()->hasUrls();
        if(event->mimeData()->hasText() )
            qDebug()<<"拖拽的文字信息"<<event->mimeData()->text();
        if(event->mimeData()->hasUrls())
        {
            //总操作文件总数需要考虑
            foreach (const QUrl &url, event->mimeData()->urls())
            {
                QString FileName=get_file_name_by_url(url);
                qDebug() << FileName;
                //=========================
                QFileInfo info( FileName);

                //文件大小，字节为单位
                qint64 size = info.size();
                int kb = size / 1024;

                //根据文件大小判断是否复制/移动/建立快捷方式
                QString show_text=info.fileName();
                qDebug() << " "<<show_text << "  size:" << info.size()<<"K  "<<kb<<"kb";
                test_url=url;

                //尝试使用一个新的类作为承接 单个label不能满足需求
                LS_Files* pLabel = new LS_Files(this);
              //   pLabel->setText(show_text);//带文件后缀的文件名
                QString base = info.completeBaseName();
               pLabel->setText(base);//不带文件后缀的文件名

                QString suffix = info.suffix().toLower();
                qDebug() << "文件后缀:" <<suffix;


                QMimeDatabase db;
                int file_type=0;
                QMimeType mime = db.mimeTypeForFile(info.fileName());
                if (mime.name().startsWith("image/")) {//图片


                    pLabel->setPixmap(utilityClass::ThumbnailSize(FileName, 50, 50),1);
                    file_type=1;


                }else if(mime.name().startsWith("text/")){//文字
                    qDebug()<<"文字";
                    file_type=4;
                }else if(mime.name().startsWith("audio/")){//音频
                    qDebug()<<"音频";
                    pLabel->setPixmap(get_pic_by_path(info.filePath() ));
                    file_type=2;
                }else if(mime.name().startsWith("video/")){//视频
                    qDebug()<<"视频";
                    //测试获取视频第一帧图片
                    //方式尝试使用python 调用cv包
                      pLabel->setPixmap(get_pic_by_path(info.filePath() ));
                    qDebug()<<"ok";
                    file_type=3;
                }else if(mime.name().startsWith("application/")){//软件
                    qDebug()<<"application";
                    if(suffix == "lnk"){//快捷方式

                        file_type=5;
                    }
                    pLabel->setPixmap(get_pic_by_path(info.filePath() ));
                } else{

                    if(suffix == "lnk"){//快捷方式
                         //可以尝试读取原文件
                        qDebug()<<"这是一个快捷方式";
                        pLabel->setPixmap(get_pic_by_path(info.filePath() ));
                        file_type=5;
                    }
                    else//其他 使用系统图标
                    {
                        pLabel->setPixmap(get_pic_by_path(info.filePath() ));
                    }
                }

                pLabel->setFileType(file_type);
                pLabel->setFile_url(url);
                pLabel->move(event->pos());
                //根据外部拖拽的图标进行简单显示？

                pLabel->setFixedSize(75,120);
                pLabel->show();

                qDebug()<<"???"<<pLabel->text()<<show_text;



            }
        }


    }
    if (event->source() == this)
    {
        event->setDropAction(Qt::MoveAction);
        event->accept();
    }
    else
        event->acceptProposedAction();
}
//界面向外拖拽
void drop_widget::mousePressEvent(QMouseEvent *event)
{
    qDebug()<<"press";
    QWidget* pItem = childAt(event->pos());
    if(!pItem) return;//点击其他界面

    if(pItem->objectName()!="label_pic"&&pItem->objectName()!="label_name")
    {
        qDebug()<<pItem->objectName();
        return;
    }

    LS_Files* pDragButton = static_cast<LS_Files*>(pItem->parentWidget());
    if (pDragButton == nullptr)
        return;
    else
        qDebug()<<"选到了"<<pDragButton->size();
    qDebug()<<"press"<<pItem->parentWidget();
    QPixmap image =  pDragButton->grab();

    //获取移动控件截图
    //   QPixmap image = QPixmap::grabWidget(pDragButton, pDragButton->rect());
    QString text = pDragButton->text();

    QByteArray array;
    QDataStream stream(&array, QIODevice::WriteOnly);
    QPixmap icon_map=pDragButton->pixmap();

    stream << text << QPoint(event->pos() - pDragButton->pos()) << pDragButton->size()<<icon_map<<pDragButton->File_type()<<pDragButton->File_url();
    test_url=pDragButton->File_url();
    QMimeData* pMimeData = new QMimeData;
    pMimeData->setData("application/x-dnditemdata", array);
    QList <QUrl> url_list;
    url_list.append(test_url);
    pMimeData->setUrls(url_list );
    //将拖动的控件截图跟着鼠标光标移动
    QDrag* drag = new QDrag(this);
    drag->setMimeData(pMimeData);

    drag->setPixmap(image);
    drag->setHotSpot(event->pos() - pDragButton->pos());

    if (drag->exec(Qt::CopyAction | Qt::MoveAction, Qt::CopyAction) == Qt::MoveAction) {
        pDragButton->close();
        qDebug()<<"close";
    }
    else {
        qDebug()<<"exec";
        pDragButton->show();
        pDragButton->setText(text);
    }
}
