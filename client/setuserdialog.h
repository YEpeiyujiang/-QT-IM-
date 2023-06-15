#ifndef SETUSERDIALOG_H
#define SETUSERDIALOG_H

#include <QDialog>

namespace Ui {
class setUserDialog;
}

class setUserDialog : public QDialog
{
    Q_OBJECT
signals:
    void SIG_userSetCommit( int iconm , QString name , QString feeling);

public:
    explicit setUserDialog(QWidget *parent = nullptr);
    ~setUserDialog();

private slots:
    void on_pb_commit_clicked();

    void on_pb_clear_clicked();
public slots:
    void slot_setInfo(int iconid , QString name,QString feeling = "比较懒，什么也没写");
    void slot_setGroupInfo(int iconid , QString name,QString feeling = "比较懒，什么也没写");
    void slot_classA(int classA);

private:
    Ui::setUserDialog *ui;

    int m_iconid;
    QString m_name ;
    QString m_feeling;
};

#endif // SETUSERDIALOG_H
