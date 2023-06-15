#include "maindialog.h"

#include <QApplication>
#include "CKernel.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
//    MainDialog w;
//    w.show();
    CKernel::GetInstance();//获取静态单例对象
//    ChatroomDialog w;
//    w.showNormal();
    return a.exec();
}
