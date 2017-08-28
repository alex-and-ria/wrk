#include "my_bt.h"

#include "resource/phone/Interface/my_resource.h"
#include <chino/core/messaging/holla.h>
#include <QtConcurrent/QtConcurrent>
#include <QTime>
#include "v1/com/harman/btpresplugin/BtManagerExt.hpp"


my_bt::my_bt(Chino::Framework &fw, Chino::ResourceImpl &propHandler) :
    _propertyHandler(propHandler),
    framework(fw)
{
    connect(this,SIGNAL(ResourceUpdate(QString , QVariant )), this, SLOT(setResourceProperty(QString , QVariant )), Qt::QueuedConnection);
    connect(this, &my_bt::SetReady, this, [this](){((Chino::Resource&)_propertyHandler).setReady();}, Qt::QueuedConnection);

    //_listeners << _propertyHandler.registerCallback(my_resource::PROPERTY_CELL_COLOR(), std::bind(&my_bt::in_callback1,this,std::placeholders::_2));
}

void my_bt::setResourceProperty(QString name, const QVariant value)
{
    qDebug() <<Q_FUNC_INFO<< "Function call - setResourceProperty" << name;
    _propertyHandler.setProperty(name, value);
}


void my_bt::on_resource(int indx){
    qDebug()<<Q_FUNC_INFO<<" on_resource: index="<<indx;
    QMap<QString, QVariant> val_map;
    val_map["red"]=indx;
    QVariant vrnt=QVariant::fromValue(val_map);
    emit ResourceUpdate(my_resource::PROPERTY_CELL_COLOR() , vrnt);
}
