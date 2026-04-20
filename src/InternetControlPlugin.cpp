#include "InternetControlPlugin.h"
#include <QProcess>

InternetControlPlugin::InternetControlPlugin(QObject* parent) :
    QObject(parent),
    m_blockFeature(
        QStringLiteral("BlockInternet"),
        Feature::Flag::Action | Feature::Flag::Master, 
        Feature::Uid(QStringLiteral("66666666-7777-8888-9999-000000000000")),
        Feature::Uid(),
        tr("Bloquear Internet"), tr("Bloquear"), tr("Corta la navegación"),
        QStringLiteral("network-disconnect")
    ),
    m_allowFeature(
        QStringLiteral("AllowInternet"),
        Feature::Flag::Action | Feature::Flag::Master, 
        Feature::Uid(QStringLiteral("77777777-7777-8888-9999-111111111111")),
        Feature::Uid(),
        tr("Permitir Internet"), tr("Permitir"), tr("Restaura la navegación"),
        QStringLiteral("network-transmit-receive")
    ),
    m_features({ m_blockFeature, m_allowFeature })
{}

const FeatureList& InternetControlPlugin::featureList() const { return m_features; }

bool InternetControlPlugin::startFeature(VeyonMasterInterface& master, const Feature& feature, const ComputerControlInterfaceList& computerControlInterfaces)
{
    FeatureMessage message(feature.uid());
    for (auto computer : computerControlInterfaces) {
        computer->sendFeatureMessage(message);        
    }
    return true;
}

bool InternetControlPlugin::handleFeatureMessage(VeyonServerInterface& server, const MessageContext& messageContext, const FeatureMessage& message)
{
    // Función de limpieza para evitar duplicados
    auto clearRules = []() {
        while (QProcess::execute("/usr/sbin/iptables", {"-D", "OUTPUT", "-p", "tcp", "--dport", "80", "-j", "DROP"}) == 0);
        while (QProcess::execute("/usr/sbin/iptables", {"-D", "OUTPUT", "-p", "tcp", "--dport", "443", "-j", "DROP"}) == 0);
    };

    if (message.featureUid() == m_blockFeature.uid()) {
        clearRules(); 
        QProcess::execute("/usr/sbin/iptables", {"-I", "OUTPUT", "-p", "tcp", "--dport", "80", "-j", "DROP"});
        QProcess::execute("/usr/sbin/iptables", {"-I", "OUTPUT", "-p", "tcp", "--dport", "443", "-j", "DROP"});
        return true;
    } 
    else if (message.featureUid() == m_allowFeature.uid()) {
        clearRules();
        return true;
    }
    return false;
}

bool InternetControlPlugin::handleFeatureMessage(VeyonWorkerInterface&, const FeatureMessage&) { return false; }
bool InternetControlPlugin::controlFeature(Feature::Uid, Operation, const QVariantMap&, const ComputerControlInterfaceList&) { return false; }