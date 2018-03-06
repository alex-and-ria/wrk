#include "my_med.h"
#include <shared/model/ListItemFactory.h>



static const QUrl url2=QUrl(QStringLiteral("qrc:///FordLCIS/Screen/general_list2.qml"));

my_med::my_med(Chino::Framework &fw, ChromeController *chrome) :
    BaseMediator(fw, chrome, /*HMI::SCREEN_GENERAL_LIST()*/url2 ),
    qml_obj(nullptr)
{
//    QQmlComponent component(framework.getQmlEngine(), url2);
//    qml_obj = component.create();
//    connect(qml_obj,SIGNAL(qmlSignal(int)),this,SLOT(on_qml(int)));
    //ctxt = fw.getQmlContext();
    qDebug() << "constructor";

}

my_med::~my_med()
{
    delete qml_obj;
}

void my_med::screenIn(QVariantMap&)
{
    qDebug() << Q_FUNC_INFO;

//    if(component != nullptr)
//    {
//        qDebug() << Q_FUNC_INFO << "Component is here";
//        qml_obj = findChild("ticTacToe");
//    }
//    else
//    {
//        qDebug() << Q_FUNC_INFO << "Component is NULL";
//    }
//    qDebug() << "screenIn 1";

    if(component!= nullptr)
    {
        qDebug() << "screenIn: find child";
        connect(component.get(), SIGNAL(qmlSignal(int)),this,SLOT(on_qml(int)));
    }
    else
    {
        qDebug() << Q_FUNC_INFO << "QML object is NULL";
    }

    for(unsigned int i=0;i<3*3;i++){
        qDebug() << Q_FUNC_INFO << "append";
        mdl_list.append("white");
    }
    qDebug() << Q_FUNC_INFO << "after append";

    //setProperty(component,"mdl", QVariant::fromValue(mdl_list));
    if(component->setProperty("mdl", QVariant::fromValue(mdl_list))){
        qDebug() << Q_FUNC_INFO << "component->setProperty ok";
    }
    else{
        qDebug() << Q_FUNC_INFO << "not component->setProperty";
    }
/*//////////////////////////////////////////prev_impl
    is_rtrn=true;
    for(unsigned char i=0;i<3*3;i++){
        cell_ind[i]=-1;
    }
*/// /////////////////////////////////////////prev_impl_


    Chino::resourceReadyCallback cbkEnt(std::bind(&my_med::onMyResourceReady, this, _1, _2));
    resourceReadyPtr << framework.getResourceReady(my_resource::RESOURCE_NAME(), cbkEnt);
}

void my_med::screenOut(QVariantMap &)
{
    qDebug() << Q_FUNC_INFO;
}

//void my_med::onCreated(QVariantMap &context)
//{
//    Q_UNUSED(context);
//    qDebug() << Q_FUNC_INFO;
//}

/*///////////////////////////////////////prev_impl;
void my_med::if_win(int p_indx){
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
*/// ////////////////////////////////////////////prev_impl_


void my_med::on_qml(int r_ind){
    Q_UNUSED(r_ind);
    qDebug() << Q_FUNC_INFO;
    if(component != nullptr)
    {
        //QList<QObject*> cells=component->findChildren<QObject*>("my_rect");
        //QList<QObject*> cells=component->findChildren<QObject*>();

//        for( int i=0;i<cells.size();i++){
//            qDebug() << cells.at(i)->objectName();
//            cells.at(i)->setProperty("color","blue");
//        }
        qDebug() << "on_qml catch";

/*///////////////////////////////////////////////////prev_impl
        if(cell_ind[r_ind]==-1){
            if(is_rtrn){
                cell_ind[r_ind]=1;
                mdl_list.replace(r_ind,"blue");
                is_rtrn=!is_rtrn;
                if_win(r_ind);
            }
            else{
                cell_ind[r_ind]=0;
                mdl_list.replace(r_ind,"red");
                is_rtrn=!is_rtrn;
                if_win(r_ind);
            }
        }
        for(int i=0;i<mdl_list.size();i++){
            qDebug() << "on_qml replace color :"<<"mdl_list.at("<<i<<")="<<mdl_list.at(i);
        }
*/// ///////////////////////////////////////////prev_impl_

        /*if(!cells.empty())
        {
            cells.at(r_ind)->setProperty("color","blue");
            qDebug() << "on_qml working";
        }*/

        /*if(component->setProperty("mdl", QVariant::fromValue(mdl_list))){
            qDebug() << Q_FUNC_INFO << "component->setProperty ok";
        }
        else{
            qDebug() << Q_FUNC_INFO << "not component->setProperty";
        }*/

    my_res->on_med(r_ind);
    }
}

void my_med::onMyResourceReady(const std::string&, Chino::ResourcePtr)
{
    qDebug()<<Q_FUNC_INFO<<"onMyResourceReady";
    my_res = framework.getResource<my_resource>(my_resource::RESOURCE_NAME());
    if (my_res != nullptr)
    {
        qDebug()<<"my_res != nullptr";
        propertyListeners << my_res->registerCallback(my_resource::PROPERTY_CELL_COLOR(), std::bind(&my_med::in_callback, this, _2));
    }
    else{
        qDebug()<<"my_res == nullptr";
    }
}

void my_med::in_callback(QVariant indx){
    qDebug()<<Q_FUNC_INFO<<"my_med callback"<<" indx="<<indx.toInt();
    QVariant vrnt=my_res->getProperty(my_resource::PROPERTY_CELL_COLOR());
    if(vrnt.isValid()){
        qDebug()<<"vrnt.isValid()";
    }
    else{
        qDebug()<<"vrnt.isValid()";
    }
    qDebug()<<"here1";
    QMap<QString,QVariant> vrnt_map=vrnt.toMap();
    qDebug()<<"here2";
    if(!vrnt_map.isEmpty()){
        qDebug()<<"!vrnt_map.isEmpty()";
        //QMapIterator<QString, QVariant> iter(vrnt_map);
        qDebug()<<"here3";
        QMap<QString,QVariant>::iterator it=vrnt_map.begin();
        qDebug()<<"here4";

        //for (it = vrnt_map.begin(); it != vrnt_map.end(); ++it) {
            qDebug()<<"it.key()="<<it.key()<<" it.value()="<<it.value().toString();
        //}
        int tmp_pos=it.value().toInt();
        qDebug()<<"tmp_pos="<<tmp_pos;
        mdl_list.replace(tmp_pos ,it.key());
    }
    else{
        qDebug()<<"vrnt_map.isEmpty()";
    }
    for(int i=0;i<mdl_list.size();i++){
        qDebug() << "on_qml replace color :"<<"mdl_list.at("<<i<<")="<<mdl_list.at(i);
    }
    if(component->setProperty("mdl", QVariant::fromValue(mdl_list))){
        qDebug() << Q_FUNC_INFO << "component->setProperty ok";
    }
    else{
        qDebug() << Q_FUNC_INFO << "not component->setProperty";
    }
}
