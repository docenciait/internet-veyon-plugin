#include "InternetControlPlugin.h"

InternetControlPlugin::InternetControlPlugin(QObject* parent) :
    QObject(parent),
    m_internetControlFeature(
        QStringLiteral("InternetControl"),
        Feature::Flag::Action | Feature::Flag::Master,
        Feature::Uid(QStringLiteral("66666666-7777-8888-9999-000000000000")),
        Feature::Uid(),
        tr("Control Internet"),
        tr("Control Internet"),
        tr("Bloquea o permite el acceso a internet"),
        QStringLiteral(":/icon.svg")
    ),
    m_features({ m_internetControlFeature })
{
}

const FeatureList& InternetControlPlugin::featureList() const
{
    return m_features;
}

bool InternetControlPlugin::startFeature(VeyonMasterInterface& master, const Feature& feature, 
                                         const ComputerControlInterfaceList& computerControlInterfaces)
{
    if (feature.uid() == m_internetControlFeature.uid()) {
        QMessageBox::information(nullptr, tr("¡Éxito!"), tr("¡Has pulsado el botón de Control de Internet!"));
        return true;
    }
    return false;
}

// --- Implementaciones obligatorias de relleno ---

bool InternetControlPlugin::controlFeature(Feature::Uid featureUid, Operation operation, const QVariantMap& arguments,
                                           const ComputerControlInterfaceList& computerControlInterfaces)
{
    return false;
}

bool InternetControlPlugin::handleFeatureMessage(VeyonServerInterface& server, const MessageContext& messageContext,
                                                 const FeatureMessage& message)
{
    return false;
}

bool InternetControlPlugin::handleFeatureMessage(VeyonWorkerInterface& worker, const FeatureMessage& message)
{
    return false;
}