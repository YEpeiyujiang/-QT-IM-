#ifndef BQDIALOG_H
#define BQDIALOG_H
#include <QMouseEvent>
#include <QDialog>
#include <QMap>
#include <QString>
#include <QImage>
#include <QPixmap>
namespace Ui {
class BqDialog;
}

class BqDialog : public QDialog
{
    Q_OBJECT

public:

    void mousePressEvent(QMouseEvent *ev);//鼠标按下事件重写
    explicit BqDialog(QWidget *parent = nullptr);
    ~BqDialog();

public slots:
    void show_prodcut_sets(int row,int col);
signals:
    void Bq_sig(QString imagePath);

private:
    Ui::BqDialog *ui;
    QMap<QString,QString> m_Bqmap;

};

#endif // BQDIALOG_H
