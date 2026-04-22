#include "InternetControlPlugin.h"
#include <QProcess>
#include <QDebug>

// Encabezados estándar del SDK de Veyon
#include "VeyonServerInterface.h"
#include "VeyonMasterInterface.h"
#include "ComputerControlInterface.h"
#include "VeyonCore.h"

// En Veyon 4.x, Computer suele estar en el namespace Veyon o requerir su header específico
#include "Computer.h"

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

    const bool esBloqueo = (feature.uid() == m_blockFeature.uid());
    const FeatureMessage message(feature.uid());

    for (auto computerControl : computerControlInterfaces)
    {
        // --- GESTIÓN DE ICONOS (MÉTODO COMPATIBLE 4.10) ---
        // Si setFeatureStatus falla en el objeto master, lo omitimos para asegurar la compilación
        // de la lógica principal. En 4.10, muchas veces el Master gestiona el icono
        // basándose en el mensaje de respuesta del servidor (el alumno).

        // Enviar la orden real al equipo del alumno
        computerControl->sendFeatureMessage(message);
    }

    return true;
}

bool InternetControlPlugin::handleFeatureMessage(VeyonServerInterface &server, const MessageContext &messageContext, const FeatureMessage &message)
{
    if (message.featureUid() == m_blockFeature.uid())
    {
        // 1. Limpieza total de reglas anteriores
        QProcess::execute("iptables", {"-F"});
        QProcess::execute("iptables", {"-X"});

        // 2. Establecer política por defecto: BLOQUEAR TODO (DROP)
        QProcess::execute("iptables", {"-P", "INPUT", "DROP"});
        QProcess::execute("iptables", {"-P", "FORWARD", "DROP"});
        QProcess::execute("iptables", {"-P", "OUTPUT", "DROP"});

        // 3. Permitir Loopback (Crítico para el sistema)
        QProcess::execute("iptables", {"-A", "INPUT", "-i", "lo", "-j", "ACCEPT"});
        QProcess::execute("iptables", {"-A", "OUTPUT", "-o", "lo", "-j", "ACCEPT"});

        // 4. Permitir tráfico a IPs específicas de Profesores
        const QStringList profes = {"192.168.50.2", "192.168.50.50", "192.168.50.100"};

        for (const QString &ip : profes)
        {
            // Salida hacia el profesor
            QProcess::execute("iptables", {"-A", "OUTPUT", "-d", ip, "-j", "ACCEPT"});
            // Entrada desde el profesor (permitiendo conexiones establecidas)
            QProcess::execute("iptables", {"-A", "INPUT", "-s", ip, "-m", "state", "--state", "NEW,ESTABLISHED", "-j", "ACCEPT"});
        }

        return true;
    }
    else if (message.featureUid() == m_allowFeature.uid())
    {
        // 1. Restaurar políticas a ACCEPT (Permitir todo)
        QProcess::execute("iptables", {"-P", "INPUT", "ACCEPT"});
        QProcess::execute("iptables", {"-P", "FORWARD", "ACCEPT"});
        QProcess::execute("iptables", {"-P", "OUTPUT", "ACCEPT"});

        // 2. Limpiar reglas para dejar el sistema limpio
        QProcess::execute("iptables", {"-F"});

        return true;
    }

    return false;
}

bool InternetControlPlugin::handleFeatureMessage(VeyonWorkerInterface &, const FeatureMessage &) { return false; }
bool InternetControlPlugin::controlFeature(Feature::Uid, Operation, const QVariantMap &, const ComputerControlInterfaceList &) { return false; }