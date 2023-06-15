#ifndef ROOMITEM_H
#define ROOMITEM_H

#include <QDialog>

namespace Ui {
class RoomItem;
}
class CKernel;
class RoomItem : public QDialog
{
    Q_OBJECT

public:
    explicit RoomItem(QWidget *parent = nullptr);
    ~RoomItem();

    void setUI();
    void setInfo(int roomid );

signals:
    void SIG_JoinRoom(int id);

private slots:
    void on_pushButton_clicked();

    void setRoomItem( int num);

private:
    Ui::RoomItem *ui;
    int m_roomid;
    friend class CKernel;
};

#endif // ROOMITEM_H
