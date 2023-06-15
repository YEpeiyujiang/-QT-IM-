#include "bqdialog.h"
#include "ui_bqdialog.h"
#include <QLabel>

void BqDialog::mousePressEvent(QMouseEvent *ev)
{

}

BqDialog::BqDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::BqDialog)
{
    ui->setupUi(this);
    //表情包

    ui->tbw_bq->setMouseTracking(true);
    ui->tbw_bq->setColumnCount(9);
    ui->tbw_bq->setRowCount(10);
    ui->tbw_bq->verticalHeader()->setVisible(false);
    ui->tbw_bq->horizontalHeader()->setVisible(false);
    ui->tbw_bq->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tbw_bq->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tbw_bq->setShowGrid(false);
    ui->tbw_bq->setObjectName("tbw_bq");
    ui->tbw_bq->setStyleSheet("QTableWidget::item:selected{background-color:white;outline:0px;}");
    int columncount = 9;
    int emojiIndex = 0;
    int rowIndex = 0;
    int columnIndex = 0;
    for(int i=0;i<=89;i++)
    {
        rowIndex = emojiIndex / columncount;
        columnIndex = emojiIndex % columncount;
        QLabel *label = new QLabel(ui->tbw_bq);
        QString path1 = QString(":/bq/0%1.png").arg(i,2,10,QLatin1Char('0'));
        QString Bqkey = QString("%1%2").arg(rowIndex).arg(columnIndex);
        m_Bqmap[Bqkey] = path1;
        label->setPixmap(QPixmap(path1));
        label->setScaledContents(true);
        label->setAlignment(Qt::AlignCenter);
        label->setToolTip(QString("000"));
        label->setMaximumSize(28,28);
        label->setMinimumSize(28,28);
        ui->tbw_bq->setCellWidget(rowIndex,columnIndex,label);
        ++emojiIndex;
    }

    connect(ui->tbw_bq,SIGNAL(cellClicked(int,int)),this,SLOT(show_prodcut_sets(int,int)));
}

BqDialog::~BqDialog()
{
    delete ui;
}
#include <QDebug>
void BqDialog::show_prodcut_sets(int row, int col)
{
    qDebug() << "所点击的单元格的row=" << row << ",col=" << col;
    //根据获得的坐标 得到路径
    QString Bqkey = QString("%1%2").arg(row).arg(col);
    //根据路径得到图片image
//    QPixmap pixmap(m_Bqmap[Bqkey]);
//    QImage image = pixmap.toImage();
    //发射信号
    Q_EMIT Bq_sig( m_Bqmap[Bqkey]);
}
