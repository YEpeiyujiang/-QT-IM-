#include "usershow.h"
#include "ui_usershow.h"

UserShow::UserShow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::UserShow),m_id(0)
{
    ui->setupUi(this);
}

UserShow::~UserShow()
{
    delete ui;
}

void UserShow::slot_setInfo(int id, QString name)
{
    m_id = id;
    m_name = name;

    ui->lb_name->setText( QString("用户[%1]").arg(m_name) );
}

void UserShow::slot_setImage(QImage &img)
{
    m_img = img;
    //update(); // 会触发绘图事件  更新
    repaint();// 会触发绘图事件   立刻触发
}

#include<QPainter>
void UserShow::paintEvent(QPaintEvent *event)
{
    //画背景
    QPainter painter( this );  //在这个控件上画, 需要这个对象指针
    painter.setBrush( Qt::black ); //设置黑色画刷
    painter.drawRect( -1 , 0, this->width() +1 , this->height() );//画一个矩形 , 会按照画刷填充

    //画图 (人物或桌面)
    //画图片
    if( m_img.size().height()<= 0 ) return;
    // 加载图片用 QImage , 画图使用 QPixmap
    // 图片缩放 scaled  fromImage  对视频进行收缩变化m_img是QImage对象 使用QImage  画图则使用QT中的QPixmap
    QPixmap pixmap = QPixmap::fromImage( m_img.scaled( QSize( this->width() ,
    this->height() - ui->lb_name->height()), Qt::KeepAspectRatio ));
    //画的位置
    int x = this->width() - pixmap.width();
    int y = this->height() - pixmap.height() - ui->lb_name->height();
    x = x /2;
    y = ui->lb_name->height() + y/2;
    painter.drawPixmap( QPoint(x,y) , pixmap );
    painter.end();

    //    event->accept();
}

void UserShow::mousePressEvent(QMouseEvent *event)
{
    Q_EMIT SIG_userisClicked(m_id , m_name);
    event->accept();
}
