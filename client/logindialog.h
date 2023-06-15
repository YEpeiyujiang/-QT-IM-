#ifndef LOGINDIALOG_H
#define LOGINDIALOG_H

#include <QDialog>
#include <QCloseEvent>
namespace Ui {
class LoginDialog;
}

class LoginDialog : public QDialog
{
    Q_OBJECT

public:
    explicit LoginDialog(QWidget *parent = nullptr);
    ~LoginDialog();

    void setUI();
signals:

    void SIG_loginCommit(QString tel,QString password);
    void SIG_registerCommit(QString tel,QString password,QString name);
    void SIG_close();

private slots:
    void on_pb_clear_clicked();

    void on_pb_login_clicked();

    void on_pb_clear_register_clicked();

    void on_pb_register_clicked();

    void closeEvebt(QCloseEvent * e);

private:
    Ui::LoginDialog *ui;
};

#endif // LOGINDIALOG_H
