#include "my_resourceimpl.h"

#include <functional>

using namespace std::placeholders;

my_resourceImpl::my_resourceImpl(Chino::Framework &fw) :
    Chino::ResourceImpl(fw),
    _ptr_my_bt(nullptr)//,
    //is_rtrn(true)
{
    qDebug() << Q_FUNC_INFO;
    //_ptrBtResource = new BtResource(fw, *this);
    Chino::propNameList properties;

    properties << PROPERTY_CELL_COLOR();
    registerCustom(properties);
    _ptr_my_bt = new my_bt(fw, *this);

/*/////////////////////////////////////////////////prev_impl
    for(unsigned char i=0;i<3*3;i++){
        cell_ind[i]=-1;
    }
*/// //////////////////////////////////////////////prev_inpl_
}

/*////////////////////////////////////////////////prev_impl
void my_resourceImpl::if_win(int p_indx){
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

*/// ////////////////////////////////////prev_impl_

void my_resourceImpl::on_med(int indx){
    qDebug()<<Q_FUNC_INFO<<"index="<<indx;
    _ptr_my_bt->on_resource(indx);
/*///////////////////////////////////prev_impl
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
    if(setProperty(PROPERTY_CELL_COLOR(),vrnt)){
        qDebug()<<"red val";
    }
    else{
        qDebug()<<"err red val";
    }
*/// ////////////////////////////////////////prev_impl_

}

QVariant my_resourceImpl::getProperty(QString propertyName)
{
    return Chino::ResourceImpl::getProperty(propertyName);
}

bool my_resourceImpl::setProperty(QString propertyName, QVariant value)
{
    return Chino::ResourceImpl::setProperty(propertyName, value);
}

void my_resourceImpl::setReady()
{
    Chino::ResourceImpl::setReady();
}

Chino::resourcePropertyListener my_resourceImpl::registerCallback(QString property, Chino::resourcePropertyCallback callback)
{
    return Chino::ResourceImpl::registerCallback( property, callback );
}

Chino::resourcePropertyListener my_resourceImpl::registerCallback(Chino::propNameList properties, Chino::resourcePropertyCallback callback)
{
    return Chino::ResourceImpl::registerCallback(properties, callback);
}
void my_resourceImpl::removeCallback(Chino::resourcePropertyListener token)
{
    return Chino::ResourceImpl::removeCallback(token);
}

void my_resourceImpl::TestMethod()
{
    Chino::ResourceImpl::TestMethod();
}
