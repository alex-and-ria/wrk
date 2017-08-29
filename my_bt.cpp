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
    for(unsigned char i=0;i<3*3;i++){
        cell_ind[i]=-1;
    }
    is_rtrn=true;
    emit SetReady();
}

void my_bt::setResourceProperty(QString name, const QVariant value)
{
    qDebug() <<Q_FUNC_INFO<< "Function call - setResourceProperty" << name;
    _propertyHandler.setProperty(name, value);
}

void my_bt::if_win(int p_indx){
    bool is_win=false;
    int indxx=p_indx/3;
    int indxy=p_indx%3;
    qDebug()<<"indxx="<<indxx<<"indxy="<<indxy<<" is_win="<<is_win;
    if(cell_ind[indxx*3]==1&&cell_ind[indxx*3+1]==1&&cell_ind[indxx*3+2]==1){
        is_win=true;
        qDebug()<<"win 1 horisontal";
    }
    if(cell_ind[indxy]==1&&cell_ind[3+indxy]==1&&cell_ind[2*3+indxy]==1){
        is_win=true;
        qDebug()<<"win 1 vertical";
    }
    if(p_indx==8||p_indx==4||p_indx==0){
        if(cell_ind[8]==1&&cell_ind[4]==1&&cell_ind[0]==1){
            is_win=true;
            qDebug()<<"win 1 diagonal1";
        }
    }
    if(p_indx==6||p_indx==4||p_indx==2){
        if(cell_ind[6]==1&&cell_ind[4]==1&&cell_ind[2]==1){
            is_win=true;
            qDebug()<<"win 1 diagonal2";
        }
    }

    if(cell_ind[indxx*3]==0&&cell_ind[indxx*3+1]==0&&cell_ind[indxx*3+2]==0){
        is_win=true;
        qDebug()<<"win 2 horisontal";
    }
    if(cell_ind[indxy]==0&&cell_ind[3+indxy]==0&&cell_ind[2*3+indxy]==0){
        is_win=true;
        qDebug()<<"win 2 vertical";
    }
    if(p_indx==8||p_indx==4||p_indx==0){
        if(cell_ind[8]==0&&cell_ind[4]==0&&cell_ind[0]==0){
            is_win=true;
            qDebug()<<"win 2 diagonal1";
        }
    }
    if(p_indx==6||p_indx==4||p_indx==2){
        if(cell_ind[6]==0&&cell_ind[4]==0&&cell_ind[2]==0){
            is_win=true;
            qDebug()<<"win 1 diagonal2";
        }
    }

    if(is_win){
        for(unsigned char i=0;i<3*3;i++){
            cell_ind[i]=2;
        }
    }
}

void my_bt::f_thread(int indx){
    qDebug()<<Q_FUNC_INFO<<" f_thread: index="<<indx;
    QMap<QString, QVariant> val_map;
    if(cell_ind[indx]==-1){
        if(is_rtrn){
            cell_ind[indx]=1;
            val_map["blue"]=indx;
            //mdl_list.replace(r_ind,"blue");
            is_rtrn=!is_rtrn;
            if_win(indx);
        }
        else{
            cell_ind[indx]=0;
            val_map["red"]=indx;
            //mdl_list.replace(r_ind,"red");
            is_rtrn=!is_rtrn;
            if_win(indx);
        }
    }
    QVariant vrnt=QVariant::fromValue(val_map);
    /*if(setProperty(PROPERTY_CELL_COLOR(),vrnt)){
        qDebug()<<"red val";
    }
    else{
        qDebug()<<"err red val";
    }*/

    //QMap<QString, QVariant> val_map;
    //val_map["red"]=indx;
    //QVariant vrnt=QVariant::fromValue(val_map);
    qDebug()<<Q_FUNC_INFO<<" emit ResourceUpdate(my_resource::PROPERTY_CELL_COLOR() , vrnt);";
    emit ResourceUpdate(my_resource::PROPERTY_CELL_COLOR() , vrnt);
    std::this_thread::sleep_for(std::chrono::seconds(5));
}

void my_bt::on_resource(int indx){
    qDebug()<<Q_FUNC_INFO<<" on_resource: index="<<indx;
    threads.push_back(std::thread(&my_bt::f_thread,this,indx));
    /*QMap<QString, QVariant> val_map;
    if(cell_ind[indx]==-1){
        if(is_rtrn){
            cell_ind[indx]=1;
            val_map["blue"]=indx;
            //mdl_list.replace(r_ind,"blue");
            is_rtrn=!is_rtrn;
            if_win(indx);
        }
        else{
            cell_ind[indx]=0;
            val_map["red"]=indx;
            //mdl_list.replace(r_ind,"red");
            is_rtrn=!is_rtrn;
            if_win(indx);
        }
    }
    QVariant vrnt=QVariant::fromValue(val_map);*/
    /*if(setProperty(PROPERTY_CELL_COLOR(),vrnt)){
        qDebug()<<"red val";
    }
    else{
        qDebug()<<"err red val";
    }*/

    //QMap<QString, QVariant> val_map;
    //val_map["red"]=indx;
    //QVariant vrnt=QVariant::fromValue(val_map);
    /*qDebug()<<Q_FUNC_INFO<<" emit ResourceUpdate(my_resource::PROPERTY_CELL_COLOR() , vrnt);";
    emit ResourceUpdate(my_resource::PROPERTY_CELL_COLOR() , vrnt);*/
}

my_bt::~my_bt(){
    qDebug()<<Q_FUNC_INFO<<" joining";
    for(auto& thr: threads){
        thr.join();
    }
}
