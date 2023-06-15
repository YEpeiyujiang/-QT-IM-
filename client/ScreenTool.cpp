#if _MSC_VER >= 1600    // VC2010
#pragma  execution_character_set("UTF-8")
#endif

#include "ScreenTool.h"
#include <QHBoxLayout>
#include <QKeyEvent>

#define W 32
#define H 48
ScreenTool::ScreenTool(QWidget *parent)
    : QDialog(parent)
{

    initUi();
    initData();
    initConnect();
}

void ScreenTool::setShowPos(int x, int y)
{
    this->move(x,y);
    this->exec();
}



void ScreenTool::initUi()
{
    m_pOk = new QPushButton();
    m_pCancel = new QPushButton;
    m_pExit = new QPushButton;
    m_pSave = new QPushButton;

    QHBoxLayout *layout = new QHBoxLayout(this);
    layout->setMargin(0);
    layout->setSpacing(0);
    layout->addWidget(m_pSave);
    layout->addWidget(m_pCancel);
    layout->addWidget(m_pExit);
    layout->addWidget(m_pOk);
}

void ScreenTool::initData()
{
    this->setWindowFlags(windowFlags() | Qt::WindowStaysOnTopHint | Qt::FramelessWindowHint);  //在顶层无边框
    this->setFixedSize(W*7,H);

    m_pSave->setFixedSize(W,W);
    m_pCancel->setFixedSize(W,W);
    m_pExit->setFixedSize(W,W);
    m_pOk->setFixedSize(W,W);


    m_pSave->setStyleSheet("QPushButton{background-image: url(://images/Save32.png);}""QPushButton{border:none;}");
    m_pCancel->setStyleSheet("QPushButton{background-image: url(://images/Cancel32.png);}""QPushButton{border:none;}");
    m_pExit->setStyleSheet("QPushButton{background-image: url(://images/Exit32.png);}""QPushButton{border:none;}");
    m_pOk->setStyleSheet("QPushButton{background-image: url(://images/Ok32.png);}""QPushButton{border:none;}");

    m_pSave->setToolTip("保存Ctrl+S");
    m_pCancel->setToolTip("重新截图");
    m_pExit->setToolTip("退出截图");
    m_pOk->setToolTip("完成截图");

    m_pSave->setShortcut(QKeySequence(QLatin1String("Ctrl+s")));
    m_pOk->setFocus();  //设置为焦点，按回车就可以完成截图
}

void ScreenTool::initConnect()
{
    QObject::connect(m_pSave,&QPushButton::clicked,this,[=]{
        emit sigSave();
    });

    QObject::connect(m_pCancel,&QPushButton::clicked,this,[=]{
        this->close();
        emit sigCancel();
    });

    QObject::connect(m_pExit,&QPushButton::clicked,this,[=]{
        this->close();
        emit sigExit();
    });

    QObject::connect(m_pOk,&QPushButton::clicked,this,[=]{
        this->close();
        emit sigOk();
    });
}

void ScreenTool::keyPressEvent(QKeyEvent *event)
{
    // Esc 键退出截图;
    if (event->key() == Qt::Key_Escape)
    {
        this->close();
        emit sigExit();
    }
}
