#ifndef MY_BT_H
#define MY_BT_H

#include "chino/framework/framework.h"
#include "chino/framework/resources/svcipcresourcebase.h"
#include "resource/phone/Interface/my_resource.h"

#include <thread>
#include <chrono>
#include <vector>

#include "hmicomm.h"

class my_bt : public QObject
{
     Q_OBJECT
public:
    my_bt(Chino::Framework &fw, Chino::ResourceImpl &propHandler);
    virtual ~my_bt();
    void in_callback1(QVariant indx);
    void on_resource(int indx);
    void if_win(int indx);
    void f_thread(int indx);

signals:
    void ResourceUpdate(QString propertyName, QVariant value);
    void SetReady();

public slots:
    void setResourceProperty(QString name, const QVariant value);

private:
    Chino::ResourceImpl &_propertyHandler;
    Chino::Framework& framework;
    //QList<Chino::resourcePropertyListener> _listeners;
    bool is_rtrn;
    int cell_ind[3*3];
    std::vector<std::thread> threads;

};







#endif // MY_BT_H
