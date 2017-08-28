#ifndef MY_RESOURCEIMPL_H
#define MY_RESOURCEIMPL_H

#include <QString>
#include <QVariant>
#include "chino/framework/framework.h"
#include "chino/core/resource/resourceimpl.h"
#include "resource/phone/Interface/my_resource.h"
#include "my_bt.h"

class my_resourceImpl : public my_resource,
                          public Chino::ResourceImpl
{
public:
    my_resourceImpl(Chino::Framework &fw);
    virtual ~my_resourceImpl(){}
    void on_med(int indx);

    virtual QVariant getProperty(QString propertyName);
    virtual bool setProperty(QString propertyName, QVariant value);

    Chino::resourcePropertyListener registerCallback(QString property, Chino::resourcePropertyCallback callback);
    Chino::resourcePropertyListener registerCallback(Chino::propNameList properties, Chino::resourcePropertyCallback callback);
    void removeCallback(Chino::resourcePropertyListener token);
    void TestMethod();


    void setReady();
    void if_win(int indx);

private:
    my_bt* _ptr_my_bt;
    bool is_rtrn;
    int cell_ind[3*3];

};

#endif // MY_RESOURCEIMPL_H
