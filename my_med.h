#ifndef MY_MED_H
#define MY_MED_H

#include "chino/framework/elements/catalog.h"
#include <shared/mediator/BaseMediator.h>
#include <shared/mediator/ListAddon.h>
#include <shared/mediator/TitlebarAddon.h>

#include "util/SimpleGuardedCallback.h"
#include "resource/phone/Interface/phoneresourcetypes.h"
#include <resource/phone/Interface/my_resource.h>

#include <QQuickView>
#include <QQmlContext>


class my_med : public BaseMediator
{

    Q_OBJECT

public:

    static const char* MEDIATOR_NAME() {return "my_med";}
    QQuickItem* qml_obj;
    QStringList mdl_list;
    //QQmlContext *ctxt;
    //QQuickView view;
    //bool is_rtrn;
    //int cell_ind[3*3];

    my_med(Chino::Framework &fw, ChromeController *chrome);
    ~my_med();

    void screenIn(QVariantMap &);
    void screenOut(QVariantMap &);
    //void if_win(int indx);
//    void onCreated(QVariantMap &context);
    void onMyResourceReady(const std::string&, Chino::ResourcePtr);
    void in_callback(QVariant indx);

public slots:
    void on_qml(int r_ind);

private:
    std::shared_ptr<my_resource> my_res;
    Chino::resourceReadyCallback cbkEnt;

};



#endif // MY_MED_H
