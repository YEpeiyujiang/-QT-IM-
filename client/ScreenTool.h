#ifndef SCREENTOOL_H
#define SCREENTOOL_H

#include <QObject>
#include <QWidget>
#include <QPushButton>
#include <QDialog>

class ScreenTool : public QDialog
{
    Q_OBJECT
public:
    explicit ScreenTool(QWidget *parent = 0);

public:
    void setShowPos(int x,int y);

    int getToolWidth(){return this->width();}
    int getToolHeight(){return this->height();}

protected:
    void keyPressEvent(QKeyEvent *)override;

signals:
    void sigSave();
    void sigCancel();
    void sigExit();
    void sigOk();
public slots:
private:
    void initUi();
    void initData();
    void initConnect();

private:
    QPushButton *m_pOk;
    QPushButton *m_pExit;
    QPushButton *m_pCancel;
    QPushButton *m_pSave;
};

#endif // SCREENTOOL_H
