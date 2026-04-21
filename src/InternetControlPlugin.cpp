#include "InternetControlPlugin.h"
#include <QProcess>

InternetControlPlugin::InternetControlPlugin(QObject *parent) : QObject(parent),
                                                                m_blockFeature(
                                                                    QStringLiteral("BlockInternet"),
                                                                    Feature::Flag::Action | Feature::Flag::Master,
                                                                    Feature::Uid(QStringLiteral("66666666-7777-8888-9999-000000000000")),
                                                                    Feature::Uid(),
                                                                    tr("Bloquear Internet"), tr("Bloquear"), tr("Corta la navegación"),
                                                                    QStringLiteral(":/icon_block.svg")),
                                                                m_allowFeature(
                                                                    QStringLiteral("AllowInternet"),
                                                                    Feature::Flag::Action | Feature::Flag::Master,
                                                                    Feature::Uid(QStringLiteral("77777777-7777-8888-9999-111111111111")),
                                                                    Feature::Uid(),
                                                                    tr("Permitir Internet"), tr("Permitir"), tr("Restaura la navegación"),
                                                                    QStringLiteral(":/icon_allow.svg")),
                                                                m_features({m_blockFeature, m_allowFeature})
{
}

const FeatureList &InternetControlPlugin::featureList() const { return m_features; }

bool InternetControlPlugin::startFeature(VeyonMasterInterface &master, const Feature &feature, const ComputerControlInterfaceList &computerControlInterfaces)
{
    FeatureMessage message(feature.uid());
    for (auto computer : computerControlInterfaces)
    {
        computer->sendFeatureMessage(message);
    }
    return true;
}

bool InternetControlPlugin::handleFeatureMessage(VeyonServerInterface &server, const MessageContext &messageContext, const FeatureMessage &message)
{
    if (message.featureUid() == m_blockFeature.uid())
    {
        // --- REGLAS DE BLOQUEO TOTAL ---

        // 1. Limpiar todo
        QProcess::execute("iptables", {"-F"});
        QProcess::execute("iptables", {"-X"});

        // 2. Políticas por defecto: DROP
        QProcess::execute("iptables", {"-P", "INPUT", "DROP"});
        QProcess::execute("iptables", {"-P", "FORWARD", "DROP"});
        QProcess::execute("iptables", {"-P", "OUTPUT", "DROP"});

        // 3. Loopback (Crítico para que el PC no se cuelgue internamente)
        QProcess::execute("iptables", {"-A", "INPUT", "-i", "lo", "-j", "ACCEPT"});
        QProcess::execute("iptables", {"-A", "OUTPUT", "-o", "lo", "-j", "ACCEPT"});

        // 4. Permitir Red Local (Para que Veyon siga conectado)
        // Salida a la red local
        QProcess::execute("iptables", {"-A", "OUTPUT", "-d", "192.168.0.0/16", "-j", "ACCEPT"});
        // Entrada desde la red local
        QProcess::execute("iptables", {"-A", "INPUT", "-s", "192.168.0.0/16", "-m", "state", "--state", "NEW,ESTABLISHED", "-j", "ACCEPT"});

        return true;
    }
    else if (message.featureUid() == m_allowFeature.uid())
    {
        // --- REGLAS PARA PERMITIR INTERNET ---

        // 1. Restaurar políticas a ACCEPT
        QProcess::execute("iptables", {"-P", "INPUT", "ACCEPT"});
        QProcess::execute("iptables", {"-P", "FORWARD", "ACCEPT"});
        QProcess::execute("iptables", {"-P", "OUTPUT", "ACCEPT"});

        // 2. Limpiar las reglas de bloqueo
        QProcess::execute("iptables", {"-F"});

        return true;
    }
    return false;
}

bool InternetControlPlugin::handleFeatureMessage(VeyonWorkerInterface &, const FeatureMessage &) { return false; }
bool InternetControlPlugin::controlFeature(Feature::Uid, Operation, const QVariantMap &, const ComputerControlInterfaceList &) { return false; }