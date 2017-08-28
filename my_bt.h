#ifndef MY_BT_H
#define MY_BT_H

#include "chino/framework/framework.h"
#include "chino/framework/resources/svcipcresourcebase.h"
#include "resource/phone/Interface/my_resource.h"

#include "v2/com/harman/btpres/ConnectionManagerProxy.hpp"
#include "v1/com/harman/btpresplugin/BtManagerExtProxy.hpp"
#include "v1/com/harman/btpresplugin/BTPresPluginTypes.hpp"
#include "v1/com/harman/btpres/CallManagerProxy.hpp"
#include "v2/com/harman/btpres/PimDbHandlerProxy.hpp"
#include "v4/com/harman/btpres/BTPresTypes.hpp"

#include "hmicomm.h"

class my_bt : public QObject
{
     Q_OBJECT
public:
    my_bt(Chino::Framework &fw, Chino::ResourceImpl &propHandler);
    virtual ~my_bt();
    void in_callback1(QVariant indx);
    void on_resource(int indx);


signals:
    void ResourceUpdate(QString propertyName, QVariant value);
    void SetReady();

public slots:
    void setResourceProperty(QString name, const QVariant value);

private:
    Chino::ResourceImpl &_propertyHandler;
    Chino::Framework& framework;
    QList<Chino::resourcePropertyListener> _listeners;

};







#endif // MY_BT_H
