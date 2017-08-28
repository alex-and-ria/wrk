#ifndef MY_RESOURCE_H
#define MY_RESOURCE_H

#include <QString>
#include <QVariant>
#include <QVariantMap>
#include "chino/framework/framework.h"
#include "chino/core/resource/resourceif.h"
#include "chino/framework/resources/svcipcresourcebase.h"
#include "resource/phone/Interface/BtAlertHandler.h"
#include "resource/phone/Interface/btdevice.h"
#include "resource/phone/Interface/phonecallinfo.h"
#include "resource/phone/Interface/phoneresourcetypes.h"
#include "util/Callbacker.h"
#include "util/SimpleGuardedCallback.h"
#include <hmicomm_global.h>

class HMICOMMSHARED_EXPORT my_resource : public Chino::ResourceIf
{
public:
    my_resource(){}
    virtual ~my_resource(){}
    virtual void on_med(int indx)=0;

    static const char *PROPERTY_CELL_COLOR() { return "cellcolor"; }
    static const char *RESOURCE_NAME() { return "my_resource"; }

};

#endif // MY_RESOURCE_H
/*

    class CORE_SHARED_EXPORT ResourceIf {
     public:
      ResourceIf();
      virtual ~ResourceIf();
      virtual QVariant getProperty(QString propertyName) = 0;
      virtual bool setProperty(QString propertyName, QVariant value) = 0;
      virtual resourcePropertyListener registerCallback(
          QString property,
          resourcePropertyCallback callback) = 0;
      virtual resourcePropertyListener registerCallback(
          Chino::propNameList properties,
          resourcePropertyCallback callback) = 0;
      virtual void removeCallback(resourcePropertyListener token) = 0;
      virtual void TestMethod() = 0;
    };
*/
