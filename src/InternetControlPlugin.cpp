#include "InternetControlPlugin.h"
#include <QProcess>
#include <QDebug>

// Headers estándar del SDK de Veyon
#include "VeyonServerInterface.h"
#include "VeyonMasterInterface.h"
#include "ComputerControlInterface.h"
#include "VeyonCore.h"
#include "Computer.h"
#include "Feature.h"

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

const FeatureList &InternetControlPlugin::featureList() const
{
    return m_features;
}

bool InternetControlPlugin::startFeature(VeyonMasterInterface &master, const Feature &feature,
                                         const ComputerControlInterfaceList &computerControlInterfaces)
{
    if (computerControlInterfaces.isEmpty())
    {
        return false;
    }

    const FeatureMessage message(feature.uid());

    for (auto computerControl : computerControlInterfaces)
    {
        // Enviamos la orden al equipo del alumno
        computerControl->sendFeatureMessage(message);
    }

    return true;
}

bool InternetControlPlugin::handleFeatureMessage(VeyonServerInterface &server, const MessageContext &messageContext, const FeatureMessage &message)
{
    if (message.featureUid() == m_blockFeature.uid())
    {
        // 1. APLICAR REGLAS DE IPTABLES (BLOQUEO)
        QProcess::execute("iptables", {"-F"});
        QProcess::execute("iptables", {"-X"});
        QProcess::execute("iptables", {"-P", "INPUT", "DROP"});
        QProcess::execute("iptables", {"-P", "FORWARD", "DROP"});
        QProcess::execute("iptables", {"-P", "OUTPUT", "DROP"});

        // Permitir localhost
        QProcess::execute("iptables", {"-A", "INPUT", "-i", "lo", "-j", "ACCEPT"});
        QProcess::execute("iptables", {"-A", "OUTPUT", "-o", "lo", "-j", "ACCEPT"});

        // Permitir acceso a IPs de Profesores (AQUÍ TUS IPs)
        const QStringList profes = {"192.168.50.2", "192.168.50.50", "192.168.50.100", "192.168.50.11"};
        for (const QString &ip : profes)
        {
            QProcess::execute("iptables", {"-A", "OUTPUT", "-d", ip, "-j", "ACCEPT"});
            QProcess::execute("iptables", {"-A", "INPUT", "-s", ip, "-m", "state", "--state", "NEW,ESTABLISHED", "-j", "ACCEPT"});
        }

        // 2. INDICADOR VISUAL PARA EL VEYON MASTER
        // Lanzamos una notificación crítica. Como profesor, verás este cuadro en la miniatura del alumno.
        QProcess::startDetached("notify-send", {"-u", "critical", "-t", "0", "Veyon: INTERNET BLOQUEADO", "Acceso restringido por el profesor."});

        return true;
    }
    else if (message.featureUid() == m_allowFeature.uid())
    {
        // 1. RESTAURAR NAVEGACIÓN
        QProcess::execute("iptables", {"-P", "INPUT", "ACCEPT"});
        QProcess::execute("iptables", {"-P", "FORWARD", "ACCEPT"});
        QProcess::execute("iptables", {"-P", "OUTPUT", "ACCEPT"});
        QProcess::execute("iptables", {"-F"});

        // 2. QUITAR AVISO VISUAL
        // Cerramos las notificaciones para limpiar la pantalla del alumno y tu Master
        QProcess::execute("killall", {"notify-osd"}); // Para Ubuntu estándar
        QProcess::execute("killall", {"dunst"});      // Para otros entornos ligeros

        return true;
    }

    return false;
}

// Implementaciones obligatorias de la interfaz (vacías)
bool InternetControlPlugin::handleFeatureMessage(VeyonWorkerInterface &, const FeatureMessage &) { return false; }
bool InternetControlPlugin::controlFeature(Feature::Uid, Operation, const QVariantMap &, const ComputerControlInterfaceList &) { return false; }